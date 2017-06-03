#include "server.h"

Server::Server()
{}

Server::~Server()
{}

bool Server::Init(string& ip, uint16_t port)
{
   if (ip.empty())
    {
        return false;
    }
    _ip = ip;
    _port = port;
    _listener = new Listen(_ip,_ port);

