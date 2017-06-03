#ifndef __SOCKET_UTIL_H__
#define __SOCKET_UTIL_H__

bool SetNonblock(int fd) {
    if (fd < 0) {
        return false;
    }
    if (fcntl(fd, F_SETFD, fcntl(fd, F_GETFD, 0) | O_NONBLOCK) == -1) {
        return false;
    }
    return true;
}
