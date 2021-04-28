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
    struct sockaddr_in _addr, _opp_addr;
    int _sock_fd;
    int _optval;
    int _port;

public:
    // DESCRIPTION: constructor
    Connection();
    Connection(int sock_fd, int _optvalue, int port);
    // NOTE: i setter e getter vanno rivisti, alcuni potrebbero non servire
    // DESCRIPTION: setter
    void setSockFD(int sock);
    void setOptval(int optval);
    void setPort(int port);
    
    // DESCRIPTION: getter
    int getSockFD();
    int getOptval();
    int getPort();
    struct addrinfo *getAddrInfo(  
                                    const char *node, 
                                    const char *service,
                                    int family,
                                    int sock_type
                                );

    // DESCRIPTION: Class members
    int setSock(int family, int socktype, int protocol);
    int setBind(uint16_t port, int socktype);
    int setTcpBind(uint16_t port);
    int setUdpBind(uint16_t port);
    int setListen(uint16_t port, int socktype);
    int setAccept(int socket, struct sockaddr *addr, socklen_t *addrlen);
    int setConnect(struct addrinfo info);
    bool sendData(int socket, const void *buf, size_t len, struct addrinfo *info);
    bool recvData(int socket, void *buf, size_t len);
    bool readNBytes(int socket, char *buf, std::size_t N);
    void setClose(int socket);
};
#endif