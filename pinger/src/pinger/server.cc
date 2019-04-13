/******************************************
* File  : server.cc
* Date  : 2017-10-21
* Author: Zhang Tianjiu
* Email : zhangtianjiu@vip.qq.com
*******************************************/

#include "pinger/server.h"

#include "pinger/options.h"

NAMESPACE_PG_BEGIN

Server::Server(): worker_num_(Options::Inst().WorkerNum()),
    worker_qlen_(Options::Inst().WorkerQlen()) {
}

bool Server::Start() {
    workers_.reset(new ccb::WorkerGroup(worker_num_, worker_qlen_));

    return true;
}


NAMESPACE_PG_END
