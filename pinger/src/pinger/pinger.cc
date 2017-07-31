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

#include "slog/slog.h"
#include "pinger/icmp.h"

NAMESPACE_PG_BEGIN


Pinger::Pinger(uint32_t src_ip): icmp_(src_ip) {
    loop_ = ev_loop_new(EVFLAG_AUTO);
}

bool Pinger::Start() {
    ev_fd_ = eventfd(0, O_NONBLOCK);
    if (ev_fd_ == -1) {
        LOG_ERROR("eventfd error: %s", strerror(errno));
        return false;
    }
    stop_watcher_.data = this;
    ev_io_init(&stop_watcher_.io, &Pinger::Shutdown, ev_fd_, EV_READ);
    ev_io_start(loop_, &stop_watcher_.io);

    raw_sock_fd_ = socket(AF_INET, SOCK_RAW | SOCK_NONBLOCK, IPPROTO_ICMP);
    if (raw_sock_fd_ == -1) {
        LOG_ERROR("socket() failed: %s", strerror(errno));
        return false;
    }
    setuid(getuid());
    size_t buf_size = 1024 * 1024;
    if (setsockopt(raw_sock_fd_, SOL_SOCKET, SO_RCVBUF, &buf_size, sizeof(buf_size)) == -1) {
        LOG_ERROR("setsocketopt() failed: %s", strerror(errno));
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

bool Pinger::Stop() {
    uint64_t value;
    int ret = write(ev_fd_, &value, sizeof(value));
    if (ret != 8) {
        LOG_ERROR("write ev_fd_ failed: %s", strerror(errno));
        return false;
    }
    return true;
}

bool Pinger::Wait() {
    LOG_INFO("Start ev_run");
    ev_run(loop_, 0);
    return true;
}

void Pinger::Shutdown(struct ev_loop * loop, struct ev_io * watcher, int revents) {
    struct ev_data * ev_data = reinterpret_cast<struct ev_data *>(watcher);
    Pinger * pinger = reinterpret_cast<Pinger *>(ev_data->data);

    uint64_t value;
    int ret = read(pinger->ev_fd_, &value, sizeof(value));
    if (ret != 8) {
        LOG_ERROR("read ev_fd_ failed: %s", strerror(errno));
        return;
    }
    ev_break(pinger->loop_, EVBREAK_ONE);
    LOG_WARN("pinger is stopping...");
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
            LOG_ERROR("recvfrom error: %s", strerror(errno));
        }
        return;
    }

    if (ret < ICMP_TOTAL_LEN) {
        LOG_ERROR("NOT my icmp packet");
        return;
    }

    struct PingInfo info;
    info.dst_ip = src_addr.sin_addr.s_addr;
    if (!pinger->icmp_.ParseIcmpPakcet(buffer, ret, info)) {
        LOG_ERROR("ParseIcmpPakcet failed");
        return;
    }

}

void Pinger::HandleSend(struct ev_loop * loop, struct ev_io * watcher, int revents) {
    struct ev_data * ev_data = reinterpret_cast<struct ev_data *>(watcher);
    Pinger * pinger = reinterpret_cast<Pinger *>(ev_data->data);

    if (pinger->send_queue_.empty()) {
        return;
    }

    int batch_num = 300;
    std::lock_guard<std::mutex> lock(pinger->send_queue_mtx_);
    while (!pinger->send_queue_.empty() && batch_num > 0) {
        --batch_num;
        PingTask * task = pinger->send_queue_.front();
        if (!pinger->SendIcmp(task)) {
            break;
        }
        pinger->send_queue_.pop();
    }
}

bool Pinger::SendIcmp(PingTask * task) {
    char packet[128];
    size_t len = 64;

    if (!icmp_.PackIcmpPacket(task->dst_ip, task->seq_num,
                task->task_id, packet, len)) {
        LOG_ERROR("PackIcmpPacket() failed");
        return false;
    }

    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = task->dst_ip;

    if (sendto(raw_sock_fd_, packet, len, 0, (struct sockaddr *)&sockaddr,
                sizeof(struct sockaddr)) < 0)  {
        LOG_ERROR("sendto() failed: %s", strerror(errno));
        return false;
    }

    return true;
}

bool Pinger::PostTask(PingTask * task) {
    std::lock_guard<std::mutex> lock(send_queue_mtx_);
    send_queue_.push(task);
    return true;
}

NAMESPACE_PG_END
