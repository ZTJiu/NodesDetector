/******************************************
* File  : main.cc
* Date  : 2017-06-03
* Author: Zhang Tianjiu
* Email : zhangtianjiu@vip.qq.com
*******************************************/


#include "pinger/pinger.h"
#include "pinger/options.h"

#include "glog/logging.h"

#include <arpa/inet.h>

using pinger::Pinger;
using pinger::Options;

int main(int argc, char **argv) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    LOG(INFO) << "options:\n" << Options::Inst().ToString();

    uint32_t src_ip = 0;
    inet_pton(AF_INET, "192.168.242.133", &src_ip);
    Pinger pinger(src_ip);
    pinger.Start();
    pinger.SendRequest();
    pinger.Wait();

    return 0;
}
