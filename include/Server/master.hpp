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
    struct sockaddr_in _masteraddr, _peeraddr;
    std::string _ipserveraddr, _portno;
    int _masterfd, _receivefd, _fdmax;
    fd_set _read_fds, _master_set;
    socklen_t _addrlen;
public:
    /* public data */
    
    // DESCRIPTION: default constructor
    Master();
    ~Master();
    
    // DESCRIPTION: Getter / Setter
    std::string getIPAddr(){return this->_ipserveraddr;}
    std::string getPort(){return this->_portno;}
    int getMasterfd(){return this->_masterfd;}
    int getReceivefd(){return this->_receivefd;}
    struct sockaddr_in getMasterAddr(){return this->_masteraddr;}
    void setIPAddr(std::string ipaddr){this->_ipserveraddr = ipaddr;}
    void setPortno(std::string port){this->_portno = port;}
    void setReceivefd(int sockfd){this->_receivefd = sockfd;}
    void updateReadfds(fd_set master){this->_read_fds = master;}

    // DESCRIPTION Class member
    int InitMaster(int domain, int socktype, int protocol, int family, int level, int optname, int optval, int backlog_queue);
    int Run();
};
#endif