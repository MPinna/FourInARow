/**
 * SECTION
 * The server class is responsible to manage all server functionalities:
 *  - Process Client authentication requests
 *  - Forward Client challenge
 *  - Send information about security DH parameter to the peers
 *  - Maintain the online player list
 *  - Maintain the public key list associated to each registered user
 */
#include "../../include/Connection/network.hpp"
#include "../../include/Server/master.hpp"

Master::Master()
    : _ipserveraddr{DEFAULT_SERVER_ADDR}, _portno{DEFAULT_SERVER_PORT}, _serverfd{0}, _receivefd{0}
{
}

Master::Master(std::string port)
    : _ipserveraddr{DEFAULT_SERVER_ADDR}, _portno{port}, _serverfd{0}, _receivefd{0}
{
}

Master::Master(std::string ipaddr, std::string port)
    : _ipserveraddr{ipaddr}, _portno{port}, _serverfd{0}, _receivefd{0}
{
}

Master::~Master()
{
}

int
Master::InitServer(int domain, int socktype, int protocol, int family, int level, int optname, int optval, int backlog_queue)
{   
    this->_serverfd = InitSocket(domain, socktype, protocol);
    SetSockOpt(this->_serverfd, level, optname, &optval);
    SockBind(this->_serverfd, this->_ipserveraddr, _portno, family, this->_serversock);
    SockListen(this->_serverfd, backlog_queue);

    return this->_serverfd;
}