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
#include <string>

int InitSocket(int domain, int socktype, int protocol);

void SetSockOpt(int sockfd, int level, int optname, void *optval);

void SockBind(
    int sockfd,
    std::string ipaddr,
    std::string port,
    struct sockaddr_in sockaddress);

void SockListen(int sockfd, int max_queue);

int SockAccept(int sockfd, struct sockaddr_in sockaddress);

struct addrinfo *GetAddrInfo(
    const char *node,
    const char *port,
    int family,
    int socktype);

void SockConnect(int sockfd, struct addrinfo info);

bool SockReceive(int rec_sockfd, void *rec_buf, size_t len);

bool SockSend(int send_sockfd, const char *send_buf, size_t len);

void SockClose(int sockfd);
#endif