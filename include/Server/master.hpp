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
public:
    /* public data */
    struct sockaddr_in _masteraddr, _peeraddr;
    int _masterfd, _receivefd, _fdmax;
    fd_set _read_fds, _master_set;
    socklen_t _addrlen;
    
    // DESCRIPTION: default constructor
    Master();
    ~Master();
    
    // Getter 
    std::string getIPAddr(){return this->_ipserveraddr;}
    std::string getPort(){return this->_portno;}
    // int         getMasterfd(){return this->_masterfd;}
    // int         getReceivefd(){return this->_receivefd;}
    // int         getfdMax(){return this->_fdmax;}
    // fd_set      getReadfds(){return this->_read_fds;}
    // fd_set      getMasterSet(){return this->_master_set;}
    
    // Setter
    void setIPAddr(std::string ipaddr){this->_ipserveraddr = ipaddr;}
    void setPortno(std::string port){this->_portno = port;}
    // void setMasterfd(int sockfd){this->_masterfd = sockfd;}
    // void setReceivefd(int sockfd){this->_receivefd = sockfd;}
    // void setfdMax(int sockfd){this->_fdmax = sockfd;}
    // void setReadfds(fd_set value){this->_read_fds = value;}
    // void setMasterSet(fd_set value){this->_read_fds = value;}

    // DESCRIPTION Class member
    int Init(int domain, int socktype, int protocol, int family, int level, int optname, int optval, int backlog_queue);
    
    int Run();
};
#endif