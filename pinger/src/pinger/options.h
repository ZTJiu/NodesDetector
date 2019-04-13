/******************************************
* File  : options.h
* Date  : 2017-07-23
* Author: Zhang Tianjiu
* Email : zhangtianjiu@vip.qq.com
*******************************************/

#pragma once

#include "gflags/gflags.h"

#include "pinger/common.h"

NAMESPACE_PG_BEGIN

class Options {
    public:
        static const Options & Inst() {
            static Options instance;
            return instance;
        }

        std::string ToString() const;
        const std::string & LocalIp() const;
        const std::string & LogPropertiesConfPath() const;
        int WorkerNum() const;
        int WorkerQlen() const;
        const std::string & HudpListenIp() const;
        int HudpListenPort() const;

    private:
        Options() {}
};


NAMESPACE_PG_END
