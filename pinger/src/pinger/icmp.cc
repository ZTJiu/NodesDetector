/******************************************
* File  : icmp.cc
* Date  : 2017-07-01
* Author: Zhang Tianjiu
* Email : zhangtianjiu@vip.qq.com
*******************************************/

#include "pinger/icmp.h"

#include <netdb.h>            // struct addrinfo
#include <sys/types.h>        // needed for socket(), uint8_t, uint16_t, uint32_t
#include <sys/socket.h>       // needed for socket()
#include <netinet/in.h>       // IPPROTO_RAW, IPPROTO_IP, IPPROTO_ICMP, INET_ADDRSTRLEN
#include <netinet/ip_icmp.h>  // struct icmp, ICMP_ECHO
#include <arpa/inet.h>        // inet_pton() and inet_ntop()
#include <sys/ioctl.h>        // macro ioctl is defined
#include <bits/ioctls.h>      // defines values for argument "request" of ioctl.
#include <net/if.h>           // struct ifreq
#include <sys/time.h>
#include <cstdio>
#include <cstring>

#include "glog/logging.h"

NAMESPACE_PG_BEGIN

// Computing the internet checksum (RFC 1071).
// Note that the internet checksum does not preclude collisions.
static uint16_t CheckSum(uint16_t * buffer, int len, uint16_t csum) {
	int count = len;
	register uint32_t sum = csum;
	uint16_t answer = 0;

	// Sum up 2-byte values until none or only one byte left.
	while (count > 1) {
		sum += *(buffer++);
		count -= 2;
	}

	// Add left-over byte, if any.
	if (count > 0) {
		sum += *(uint8_t *) buffer;
	}

	// Fold 32-bit sum into 16 bits; we lose information by doing this,
	// increasing the chances of a collision.
	// sum = (lower 16 bits) + (upper 16 bits shifted right 16 bits)
	while (sum >> 16) {
		sum = (sum & 0xffff) + (sum >> 16);
	}

	// Checksum is one's compliment of sum.
	answer = ~sum;

	return (answer);
}

static uint16_t TimeDiff(const struct timeval & recv_time, const struct timeval & send_time) {
    if (recv_time.tv_usec >= send_time.tv_usec) {
        return static_cast<uint16_t>((recv_time.tv_sec - send_time.tv_sec) * 1000 +
                (recv_time.tv_usec - send_time.tv_usec) / 1000);
    } else {
        return static_cast<uint16_t>((recv_time.tv_sec - send_time.tv_sec - 1) * 1000 +
                (recv_time.tv_usec + 1000 - send_time.tv_usec) / 1000);
    }
}

Icmp::Icmp(uint32_t src_ip, uint16_t process_id)
	: src_ip_(src_ip), icmp_id_(process_id) {
    LOG(INFO) << "process_id=" << icmp_id_;
}

bool Icmp::PackIcmpPacket(uint32_t dst_ip, uint16_t seq_num,
		uint64_t task_id, char * buffer, size_t buffer_len) {
    if (buffer_len < ICMP_TOTAL_LEN) {
        return false;
    }

	//struct ip iphdr;
	struct icmphdr * icmphdr = reinterpret_cast<struct icmphdr *>(buffer);
    size_t datalen = ICMP_DATALEN;

	// ICMP header
	// Message Type (8 bits): echo request
	icmphdr->type = ICMP_ECHO;
	// Message Code (8 bits): echo request
	icmphdr->code = 0;
	// Identifier (16 bits): usually pid of sending process - pick a number
	icmphdr->un.echo.id = icmp_id_;
	// Sequence Number (16 bits): starts at 0
	icmphdr->un.echo.sequence = htons(seq_num);
	// ICMP header checksum (16 bits): set to 0 when calculating checksum
	icmphdr->checksum = 0;

    memset(icmphdr + 1, 0, sizeof(struct timeval));
    icmphdr->checksum = CheckSum(reinterpret_cast<uint16_t *>(buffer), ICMP_TOTAL_LEN, 0);

    // ICMP data
    // time stamp
    struct timeval * now = reinterpret_cast<struct timeval *>(icmphdr + 1);
    gettimeofday(now, nullptr);
    // task_id
    *(reinterpret_cast<uint64_t *>(now + 1)) = task_id;

	// Calculate ICMP header checksum
	icmphdr->checksum = CheckSum(reinterpret_cast<uint16_t *>(buffer),
		ICMP_HDRLEN + datalen, ~icmphdr->checksum);

    return true;
}

bool Icmp::ParseIcmpPakcet(const char * packet,
        size_t len, PingInfo * info, bool *not_my_pkt) {
    const struct ip * ip = reinterpret_cast<const struct ip *>(packet);
    size_t ip_hdr_len = ip->ip_hl << 2;

    if (ip->ip_p != IPPROTO_ICMP)
        return false;

    const struct icmphdr * icmp_hdr = reinterpret_cast<const struct icmphdr *>(packet + ip_hdr_len);
    size_t icmp_len = len - ip_hdr_len;
    if (icmp_len < ICMP_TOTAL_LEN)
        return false; // not our ICMP packet

    if (icmp_hdr->type != ICMP_ECHOREPLY ||
            icmp_hdr->un.echo.id != icmp_id_) {
        /*LOG(ERROR) << "check failed: icmp_hdr->type=" << (int)icmp_hdr->type
		<< ", echo.id=" << icmp_hdr->un.echo.id << ", icmp_id_=" << icmp_id_;*/
	*not_my_pkt = true;
        return false; // not our ICMP reply packet
    }

    //(packet + ip_hdr_len + ICMP_HDRLEN);
    //struct timeval send_time = *(reinterpret_cast<const struct timeval *>(packet + ip_hdr_len + ICMP_HDRLEN));
    const struct timeval * send_time = reinterpret_cast<const struct timeval *>(icmp_hdr + 1);
    struct timeval recv_time;
    gettimeofday(&recv_time, nullptr);
    info->cost = TimeDiff(recv_time, *send_time);
    info->task_id = *(reinterpret_cast<const uint64_t *>(send_time + 1));

    struct in_addr addr = {info->dst_ip};
    // FIXME: inet_ntoa not thread safe
    LOG(INFO) << "ping " << inet_ntoa(addr) << " task_id=" << info->task_id
	<< " cost=" << info->cost << "ms";

    return true;
}

NAMESPACE_PG_END
