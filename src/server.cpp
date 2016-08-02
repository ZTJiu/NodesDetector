#include "server.h"

Server::Server()
{}

Server::~Server()
{}

Server::Init(string& ip, uint16_t port)
{
   if (ip.empty())
    {
       
