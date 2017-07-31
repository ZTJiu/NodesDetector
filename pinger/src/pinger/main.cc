/******************************************
* File  : main.cc
* Date  : 2017-06-03
* Author: Zhang Tianjiu
* Email : zhangtianjiu@vip.qq.com
*******************************************/


#include "pinger/pinger.h"
#include "pinger/options.h"
#include "slog/slog.h"

using pinger::Pinger;
using pinger::Options;

int main(int argc, char **argv) {
    google::ParseCommandLineFlags(&argc, &argv, true);

    Slog::SetLogProperties(Options::Inst().LogPropertiesConfPath());
    LOG_INFO("options:\n%s", Options::Inst().ToString().c_str());

    Pinger pinger(10000);
    pinger.Start();
    pinger.Wait();

    return 0;
}
