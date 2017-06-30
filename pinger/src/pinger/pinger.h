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


NAMESPACE_PG_BEGIN

class Pinger {
public:
    Pinger();

    bool Start();
    bool Stop();
    bool Wait();

private:
    int ev_fd_;
    std::thread thread_;
    struct ev_loop *loop_;

};

NAMESPACE_PG_END
