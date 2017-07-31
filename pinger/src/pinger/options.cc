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

std::string Options::LocalIp() const {
    return FLAGS_local_ip;
}

std::string Options::LogPropertiesConfPath() const {
    return FLAGS_log_properties_conf_path;
}

std::string Options::ToString() const {
    std::stringstream ss;
    ss << "local_ip         = " << FLAGS_local_ip << std::endl;
    ss << "log_properties_conf_path     = " << FLAGS_log_properties_conf_path;
    return ss.str();
}

NAMESPACE_PG_END
