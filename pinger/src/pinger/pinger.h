/******************************************
* File  : pinger.h
* Date  : 2017-06-03
* Author: Zhang Tianjiu
* Email : zhangtianjiu@vip.qq.com
*******************************************/

#pragma once

#include "pinger/common.h"

#include <ev.h>

#include <thread>
#include <queue>
#include <mutex>

#include "pinger/icmp.h"
#include "proto/src/ping_task.pb.h"
#include "common/concurrentqueue.h"

NAMESPACE_PG_BEGIN

using nodes::detector::PingRequest;
using moodycamel::ConcurrentQueue;

struct ev_data {
    struct ev_io io;
    void * data;
};

class Pinger {
public:
    explicit Pinger(uint32_t src_ip);

    bool Start();
    bool Stop();
    bool Wait();

    void SendRequest();
    bool PostTask(PingRequest && request);

private:
    void SendIcmp(PingRequest & task);

    static void HandleSend(struct ev_loop * loop,
            struct ev_io * watcher, int revents);
    static void HandleRecv(struct ev_loop * loop,
            struct ev_io * watcher, int revents);
    static void Shutdown(struct ev_loop * loop,
            struct ev_io * watcher, int revents);
    static void HandleTimeout(struct ev_loop * loop,
            ev_io * watcher, int revents);

private:
    //void AddTimer(k)
    int ev_fd_;
    int raw_sock_fd_;
    struct ev_data stop_watcher_;
    struct ev_data send_watcher_;
    struct ev_data recv_watcher_;
    std::thread thread_;
    struct ev_loop *loop_;
    //std::queue<PingRequest> send_queue_;
    //std::mutex send_queue_mtx_;
    ConcurrentQueue<PingRequest> task_queue_;
    Icmp icmp_;

    std::vector<std::string> ips_;
};

NAMESPACE_PG_END
