/**
 * DESCRIPTION
 * The client network file it is used to manage a generic client object, with its specific fields and class members
 */
#ifndef _CLIENT_SOCKET_H_
#define _CLIENT_SOCKET_H_
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
    Slave(std::string serverport);
    Slave(std::string serverport, std::string serveraddr);
    ~Slave();
    // DESCRIPTION: class members
    int InitClient(int domain, int socktype, int protocol, int family);
    int InitPeerReceiver(struct sockaddr_in _peersock, int backlog_queue);
    int InitPeerSender(
        struct sockaddr_in _peersock,
        int domain,
        int socktype, 
        int protocol, 
        int family);
};

#endif