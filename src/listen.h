#ifndef __LISTENER_H__
#define __LISTENER_H__

#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include "logger.h"
#include "socket_util.h"

class Listen {
public:
    Listen(string & ip, uint16_t port, Logger * logger, bool use_tcp = true, bool use_nonblock = true)
        : _ip(ip), _port(port), _logger(logger), _use_tcp(use_tcp), _use_nonblock(use_nonblock) {}
    bool InitListen() {
        _listen_fd = socket(AF_INET, _use_tcp ? SOCK_STREAM : SOCK_DGRAM);
        if (_listen_fd == -1) {
            _logger->LOG_ERR("socket failed, errno = %d", (int)errno);
            return false;
        }
        do {
            if (_use_nonblock && SetNonblock(_listen_fd) == false) {
                _logger->LOG_ERR("SetNonblock failed, errno = %d", (int)errno);
                break;
            }
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = inet_addr(_ip.c_str());
            addr.sin_port = htons(_port);
            if (bind(_listen_fd, (struct sockaddr)&addr, sizeof(addr)) == -1) {
                _logger->LOG_ERR("bind failed, errno = %d", (int)errno);
                break;
            }
            if (listen(_listen_fd, MAX_LISTEN) == -1) {
                logger->LOG_ERR("listen failed, errno = %d", (int)errno);
                break;
            }
            return true;
        } while(0);
        close(_listen_fd);
        return false;
    }

private:
    int _listen_fd;
    uint16_t _port;
    Logger * _logger;
    bool _use_tcp;
    bool _use_nonblock;
};



#endif

