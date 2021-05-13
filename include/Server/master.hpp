/**
 * DESCRIPTION
 * The server network file it is used to manage a generic server object, with its specific fields and class members
 */
#ifndef _SERVER_SOCKET_H
#define _SERVER_SOCKET_H

#include "Connection/network.hpp"
#include "Utils/constant.hpp"
class Master
{
private:
    /* data */
    std::string _ipserveraddr, _portno;
    struct addrinfo *info;
public:
    struct sockaddr_in _serversock;
    int _serverfd, _exchangefd;
    // DESCRIPTION: default constructor
    Master();
    Master(std::string port);
    Master(std::string ipaddr, std::string port);
    ~Master();
    
    // DESCRIPTION: Getter / Setter
    std::string getIPAddr(){return this->_ipserveraddr;};
    std::string getPort(){return this->_portno;};
    // int getServerfd(){return this->_serverfd;};
    // int getExchangefd(){return this->_exchangefd;};
    // void setServerfd(int value){this->_serverfd = value;};
    // void setExchangefd(int value){this->_exchangefd = value;};
    // DESCRIPTION Class member
    int InitServer(int domain, int socktype, int protocol, int family, int level, int optname, int optval, int backlog_queue);
};
#endif