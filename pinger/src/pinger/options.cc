/******************************************
* File  : options.cc
* Date  : 2017-07-23
* Author: Zhang Tianjiu
* Email : zhangtianjiu@vip.qq.com
*******************************************/

#include "pinger/options.h"

#include <gflags/gflags.h>
#include <sstream>

NAMESPACE_PG_BEGIN

DEFINE_string(local_ip,
            "127.0.0.1",
            "local ping src ip");
DEFINE_string(log_properties_conf_path,
            "./conf/pinger.properties",
            "logger properties configure file path");

DEFINE_int32(worker_num,
            2,
            "worker group size");
DEFINE_int32(worker_qlen,
            65535,
            "queue len of worker group");
DEFINE_string(hudp_listen_ip,
            "0.0.0.0",
            "HyperUdp listening ip");
DEFINE_int32(hudp_listen_port,
            8888,
            "HyperUdp listening port");

const std::string & Options::LocalIp() const {
    return FLAGS_local_ip;
}

const std::string & Options::LogPropertiesConfPath() const {
    return FLAGS_log_properties_conf_path;
}

int Options::WorkerNum() const {
    return FLAGS_worker_num;
}

int Options::WorkerQlen() const {
    return FLAGS_worker_qlen;
}

const std::string & Options::HudpListenIp() const {
    return FLAGS_hudp_listen_ip;
}

int Options::HudpListenPort() const {
    return FLAGS_hudp_listen_port;
}

std::string Options::ToString() const {
    std::stringstream ss;
    ss << "local_ip         = " << FLAGS_local_ip << std::endl;
    ss << "worker_num       = " << FLAGS_worker_num << std::endl;
    ss << "worker_qlen      = " << FLAGS_worker_qlen << std::endl;
    ss << "hudp_listen_ip   = " << FLAGS_hudp_listen_ip << std::endl;
    ss << "hudp_listen_port = " << FLAGS_hudp_listen_port << std::endl;
    ss << "log_properties_conf_path     = " << FLAGS_log_properties_conf_path;

    return ss.str();
}

NAMESPACE_PG_END
