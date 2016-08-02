#ifndef _SERVER_H_
#define _SERVER_H_

#include "nonecopyable.h"
#include <stdint.h>
#include <string>

class Server: public NoneCopyable
{
private:
    Server();
    ~Server();

public:
    Server* Instance();
    Init(string ip, uint16_t port);
    Run();

private:
    Server* _server;

    string _ip;
    uint16_t _port;

}
