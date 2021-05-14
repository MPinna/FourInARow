/**
 * DESCRIPTION
 * The client network file it is used to manage a generic client object, with its specific fields and class members
 */
#ifndef _CLIENT_SOCKET_H_
#define _CLIENT_SOCKET_H_

#include "../Connection/network.hpp"
#include "../Utils/constant.hpp"
#include <sys/socket.h>
#include <string>

class Slave
{
private:
    /* data */
    std::string _serveraddr, _serverport;
    std::string _peeraddr, _peerport;
    struct addrinfo *_serverinfo;
    struct addrinfo *_peerinfo;
    int _clientfd, _peerfd;
public:
    // DESCRIPTION: Constructor / Deconstructor
    Slave();
    Slave(std::string peerport);
    Slave(std::string peerport, std::string peeraddr);
    ~Slave();
    // DESCRIPTION: Getter / Setter
    std::string getServerAddr(){return this->_serveraddr;};
    std::string getServerPort(){return this->_serverport;};
    std::string getPeerAddr(){return this->_peeraddr;};
    std::string getPeerPort(){return this->_peerport;};
    int getClientfd(){return this->_clientfd;};
    int getPeerfd(){return this->_peerfd;};
    void setServerAddr(std::string addr){this->_serveraddr = addr;};
    void setServerPort(std::string port){this->_serverport = port;};
    void setPeerAddr(std::string addr){this->_peeraddr = addr;};
    void setPeerPort(std::string port){this->_peerport = port;};
    
    // DESCRIPTION: class members
    int InitClient(int domain, int socktype, int protocol, int family);
    
    void InitPeerReceiver(
        int domain, 
        int socktype, 
        int protocol, 
        int family,
	    int level,
	    int optname,
        int optval,
        int backlog_queue, 
        struct sockaddr_in _peersock
    );
    
    void InitPeerSender(
        int domain,
        int socktype, 
        int protocol, 
        int family,
        struct sockaddr_in _peersock
    );
};
#endif