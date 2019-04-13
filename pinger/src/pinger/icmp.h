/******************************************
* File  : icmp.h
* Date  : 2017-07-01
* Author: Zhang Tianjiu
* Email : zhangtianjiu@vip.qq.com
*******************************************/

#pragma once

#include <cstdlib>

#include "pinger/common.h"

NAMESPACE_PG_BEGIN

#define ICMP_HDRLEN 8       // ICMP header length for echo request, excludes data
#define ICMP_DATALEN 56     // Include 8 bytes time stamp data
#define ICMP_TOTAL_LEN 64   // Just like a ping command

struct PingInfo {
    int16_t cost;
    uint32_t dst_ip;
    uint64_t task_id;
};

class Icmp final {
    public:
        Icmp(uint32_t src_ip, uint16_t process_id);
        bool PackIcmpPacket(uint32_t dst_ip, uint16_t seq_num,
				uint64_t task_id, char * buffer, size_t buffer_len);
        bool ParseIcmpPakcet(const char * packet, size_t len, PingInfo * info, bool *not_my_pkt);

    private:
        uint32_t src_ip_;
	uint16_t icmp_id_; // process id
};

NAMESPACE_PG_END
