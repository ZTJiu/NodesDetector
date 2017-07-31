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

NAMESPACE_PG_BEGIN

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

private:
    bool SendIcmp(PingTask * task);
    bool PostTask(PingTask * task);

    static void HandleSend(struct ev_loop * loop,
            struct ev_io * watcher, int revents);
    static void HandleRecv(struct ev_loop * loop,
            struct ev_io * watcher, int revents);
    static void Shutdown(struct ev_loop * loop,
            struct ev_io * watcher, int revents);

private:
    int ev_fd_;
    int raw_sock_fd_;
    struct ev_data stop_watcher_;
    struct ev_data send_watcher_;
    struct ev_data recv_watcher_;
    std::thread thread_;
    struct ev_loop *loop_;
    std::queue<PingTask *> send_queue_;
    std::mutex send_queue_mtx_;
    Icmp icmp_;
};

NAMESPACE_PG_END
