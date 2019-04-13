/******************************************
* File  : server.h
* Date  : 2017-10-21
* Author: Zhang Tianjiu
* Email : zhangtianjiu@vip.qq.com
*******************************************/

#pragma once

#include<memory>

#include "ccbase/worker_group.h"

#include "pinger/common.h"

NAMESPACE_PG_BEGIN

class Server {
public:
    Server();

    bool Start();
    void Wait();
    void Stop();

private:
    int worker_num_;
    int worker_qlen_;

    std::unique_ptr<ccb::WorkerGroup> workers_;
};


NAMESPACE_PG_END
