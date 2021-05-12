/**
 * SECTION
 * The server class is responsible to manage all server functionalities:
 *  - Process Client authentication requests
 *  - Forward Client challenge
 *  - Send information about security DH parameter to the peers
 *  - Maintain the online player list
 *  - Maintain the public key list associated to each registered user
 */
#include "Connection/network.hpp"
#include "Server/master.hpp"
#include <iostream>

Master::Master(/* args */)
{
}

Master::~Master()
{
}

int
Master::InitServer(int domain, int socktype, int protocol, int family, int level, int optname, int backlog_queue)
{   
    this->_serverfd = InitSocket(domain, socktype, protocol);
    SetSockOpt(this->_serverfd, level, optname, &this->opt);
    SockBind(this->_serverfd, this->_ipserveraddr, _portno, this->_serversock);
    SockListen(this->_serverfd, backlog_queue);

    return this->_serverfd;
}