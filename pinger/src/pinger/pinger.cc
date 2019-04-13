/******************************************
* File  : pinger.cc
* Date  : 2017-06-03
* Author: Zhang Tianjiu
* Email : zhangtianjiu@vip.qq.com
*******************************************/

#include "pinger/pinger.h"

#include <sys/eventfd.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h> // for inet_pton

#include "pinger/icmp.h"
#include "glog/logging.h"

NAMESPACE_PG_BEGIN

std::vector<std::string> ips = {
    "220.181.57.216",
    "123.125.115.110",
    "111.161.64.40",
    "111.161.64.48"
};

Pinger::Pinger(uint32_t src_ip): icmp_(src_ip, static_cast<uint16_t>(::getpid())),
        ips_(ips) {
    loop_ = ev_loop_new(EVFLAG_AUTO);

}

bool Pinger::Start() {
    ev_fd_ = eventfd(0, O_NONBLOCK);
    if (ev_fd_ == -1) {
        PLOG(ERROR) << "eventfd error";
        return false;
    }
    stop_watcher_.data = this;
    ev_io_init(&stop_watcher_.io, &Pinger::Shutdown, ev_fd_, EV_READ);
    ev_io_start(loop_, &stop_watcher_.io);

    raw_sock_fd_ = socket(AF_INET, SOCK_RAW | SOCK_NONBLOCK, IPPROTO_ICMP);
    if (raw_sock_fd_ == -1) {
        PLOG(ERROR) << "socket() failed";
        return false;
    }
    setuid(getuid());
    size_t buf_size = 1024 * 1024;
    if (setsockopt(raw_sock_fd_, SOL_SOCKET, SO_RCVBUF, &buf_size, sizeof(buf_size)) == -1) {
        PLOG(ERROR) << "setsocketopt() failed";
        return false;
    }

    send_watcher_.data = this;
    ev_io_init(&send_watcher_.io, &Pinger::HandleSend, raw_sock_fd_, EV_WRITE);
    ev_io_start(loop_, &send_watcher_.io);
    recv_watcher_.data = this;
    ev_io_init(&recv_watcher_.io, &Pinger::HandleRecv, raw_sock_fd_, EV_READ);
    ev_io_start(loop_, &recv_watcher_.io);

    return true;
}

void Pinger::SendRequest() {
    LOG(INFO) << "SendRequest: " << ips_.size();
    uint64_t id = 0;
    for (auto ip : ips_) {
        PingRequest request;
        request.set_ip(ip);
        request.set_count(5);
        request.set_task_id(id++);
        PostTask(std::move(request));
        LOG(INFO) << "SEND request, ip=" << ip << " task_id=" << (id - 1);
	google::FlushLogFiles(google::INFO);
    }
}

bool Pinger::Stop() {
    uint64_t value;
    int ret = write(ev_fd_, &value, sizeof(value));
    if (ret != 8) {
        PLOG(ERROR) << "write ev_fd_ failed";
        return false;
    }
    return true;
}

bool Pinger::Wait() {
    LOG(INFO) << "Start ev_run";
    ev_run(loop_, 0);
    return true;
}

void Pinger::Shutdown(struct ev_loop * loop, struct ev_io * watcher, int revents) {
    struct ev_data * ev_data = reinterpret_cast<struct ev_data *>(watcher);
    Pinger * pinger = reinterpret_cast<Pinger *>(ev_data->data);

    uint64_t value;
    int ret = read(pinger->ev_fd_, &value, sizeof(value));
    if (ret != 8) {
        PLOG(ERROR) << "read ev_fd_ failed";
        return;
    }
    ev_break(pinger->loop_, EVBREAK_ONE);
    LOG(WARNING) << "pinger is stopping...";
}

void Pinger::HandleRecv(struct ev_loop * loop, struct ev_io * watcher, int revents) {
    struct ev_data * ev_data = reinterpret_cast<struct ev_data *>(watcher);
    Pinger * pinger = reinterpret_cast<Pinger *>(ev_data->data);

    char buffer[512];
    struct sockaddr_in src_addr;
    socklen_t addr_len;
    ssize_t ret = recvfrom(pinger->raw_sock_fd_, buffer, sizeof(buffer), 0,
            reinterpret_cast<struct sockaddr *>(&src_addr), &addr_len);
    if (ret == -1) {
        if (errno != EAGAIN) {
            PLOG(ERROR) << "recvfrom error";
        }
        return;
    }

    if (ret < ICMP_TOTAL_LEN) {
        LOG(ERROR) << "NOT my icmp packet";
        return;
    }

    bool not_my_pkt = false;
    struct PingInfo info;
    info.dst_ip = src_addr.sin_addr.s_addr;
    if (pinger->icmp_.ParseIcmpPakcet(buffer, ret, &info, &not_my_pkt)) {
        // TODO: callback
    } else {
	if (!not_my_pkt)
        	LOG(ERROR) << "ParseIcmpPakcet failed";
    }
}

void Pinger::HandleSend(struct ev_loop * loop, struct ev_io * watcher, int revents) {
    struct ev_data * ev_data = reinterpret_cast<struct ev_data *>(watcher);
    Pinger * pinger = reinterpret_cast<Pinger *>(ev_data->data);

    int batch_num = 300;
    PingRequest request;
    while (batch_num-- > 0 && pinger->task_queue_.try_dequeue(request)) {
        pinger->SendIcmp(request);
    }
}

void Pinger::HandleTimeout(struct ev_loop * loop, ev_io * watcher, int revents) {
    struct ev_data * ev_data = reinterpret_cast<struct ev_data *>(watcher);
    Pinger * pinger = reinterpret_cast<Pinger *>(ev_data->data);
}

void Pinger::SendIcmp(PingRequest & request) {
    char packet[128] = {0};
    size_t len = 64;

    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    // FIXME: check error
    inet_pton(AF_INET, request.ip().c_str(), &sockaddr.sin_addr.s_addr);

    uint16_t seq_id = static_cast<uint16_t>(request.seq_id());
    request.set_seq_id(seq_id + 1);

    do {
        if (!icmp_.PackIcmpPacket(sockaddr.sin_addr.s_addr, seq_id,
                    request.task_id(), packet, len)) {
            LOG(ERROR) << "PackIcmpPacket() failed";
            break;
        }

        if (sendto(raw_sock_fd_, packet, len, 0, (struct sockaddr *)&sockaddr,
                    sizeof(struct sockaddr)) < 0)  {
            PLOG(ERROR) << "sendto() failed";
            break;
        }
    } while(0);

    // TODO: post task
}

bool Pinger::PostTask(PingRequest && request) {
    return task_queue_.enqueue(std::move(request));
}
/*
void Pinger::AddTimer(double after, void * tag) {
    ev_timer * timeout_watcher = new ev_timer;
    ev_timer_init(timeout_watcher, timeout_cb, after, 0.);
    ev_timer_start(loop_, timeout_watcher);
}
*/

NAMESPACE_PG_END
