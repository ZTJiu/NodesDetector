/******************************************
* File  : pinger.cc
* Date  : 2017-06-03
* Author: Zhang Tianjiu
* Email : zhangtianjiu@vip.qq.com
*******************************************/

#include "pinger/pinger.h"
#include "slog/log.h"

#include <sys/eventfd.h>

NAMESPACE_PG_BEGIN

Pinger::Pinger() {

}

bool Pinger::Start() {
    ev_fd_ = eventfd(0, O_NONBLOCK);
    if (ev_fd_ == -1) {
        LOG_ERROR("eventfd error: %s", strerror(errno));
        return false;
    }

    loop_ = EV_DEFAULT;
    watcher_.data = this;
    ev_io_init (&watcher_.io, HandleEvents, fd_, EV_READ);
    ev_io_start (loop_, &watcher_.io);

}

bool Pinger::Stop() {

}

bool Pinger::Wait() {

}

NAMESPACE_PG_END
