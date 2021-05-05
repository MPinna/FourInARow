/**
 * DESCRIPTION
 * The connection class it is used to manage two nodes on a network to communicate with each other. More in detail, it is responsible for:
 *  - Initialize Server socket (Socket, setsockopt, Bind, Listen, Accept)
 *  - Initialize Client socket (Socket, Connect)
 *  - Manage send() / receive() (for both Client and Server)
 */
#ifndef _CONNECTION_H_
#define _CONNECTION_H_
#include "Utils/constant.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <cstring>

// TOCHECK ha senso avere una classe qua ?
class Connection
{
private:
    /* data related to generic connection connection */
    struct sockaddr_in _serv_addr;
    int _server_port;
    int _sockfd;

public:
    // DESCRIPTION: constructor
    Connection();
    Connection(int sock_fd, int port);
    // NOTE: i setter e getter vanno rivisti, alcuni potrebbero non servire
    // DESCRIPTION: setter
    void setSockFD(int sock);
    void setPort(int port);
    
    // DESCRIPTION: getter
    int getSockFD();
    int getPort();
    
    struct addrinfo *getAddrInfo(  
                                    const char *node, 
                                    const char *service,
                                    int family,
                                    int sock_type
                                );

    // DESCRIPTION: Class members
    int Sock(int family, int socktype, int protocol);
    int Bind(int socktype);
    int TcpBind();
    int UdpBind();
    int Listen(); 
    // NOTE: from here
    int Accept(struct sockaddr *addr, socklen_t *addrlen);
    int Connect(struct addrinfo info);
    bool SendData(int socket, const void *buf, size_t len, struct addrinfo *info);
    bool RecvData(int socket, void *buf, size_t len);
    bool ReadNBytes(int socket, char *buf, std::size_t N);
    void Close(int socket);
};
#endif