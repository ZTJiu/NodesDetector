/******************************************
* File  : main.cc
* Date  : 2017-06-03
* Author: Zhang Tianjiu
* Email : zhangtianjiu@vip.qq.com
*******************************************/


#include "pinger/pinger.h"


int main(int argc, char **argv) {
    Pinger pinger;
    pinger.Start();
    pinger.Wait();

    return 0;
}
