/**
 * DESCRIPTION
 * The server network file it is used to manage a generic server object, with its specific fields and class members
 */
#ifndef _SERVER_SOCKET_H
#define _SERVER_SOCKET_H

#include "../Connection/network.hpp"
#include "../Utils/constant.hpp"
class Master
{
private:
    /* data */
    std::string _ipserveraddr, _portno;
    int _masterfd, _receivefd;
    struct addrinfo *info;
public:
    struct sockaddr_in _serversock;
    /* public data */
    
    // DESCRIPTION: default constructor
    Master();
    Master(std::string port);
    Master(std::string ipaddr, std::string port);
    ~Master();
    
    // DESCRIPTION: Getter / Setter
    std::string getIPAddr(){return this->_ipserveraddr;}
    std::string getPort(){return this->_portno;}
    int getServerfd(){return this->_masterfd;}
    int getReceivefd(){return this->_receivefd;}
    void setIPAddr(std::string ipaddr){this->_ipserveraddr = ipaddr;}
    void setPortno(std::string port){this->_portno = port;}
    void setReceivefd(int sockfd){this->_receivefd = sockfd;}

    // DESCRIPTION Class member
    int InitMaster(int domain, int socktype, int protocol, int family, int level, int optname, int optval, int backlog_queue);
};
#endif