/**
 * DESCRIPTION
 * The connection class it is used to manage two nodes on a network to communicate with each other. More in detail, it is responsible for:
 *  - Initialize Server socket (Socket, setsockopt, Bind, Listen, Accept)
 *  - Initialize Client socket (Socket, Connect)
 *  - Manage send() / receive() (for both Client and Server)
 */
#ifndef _CONNECTION_H_
#define _CONNECTION_H_
#include "../Utils/constant.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <sys/un.h>
#include <netdb.h>
#include <string>

int InitSocket(int domain, int socktype, int protocol);

int SetSockOpt(int sockfd, int level, int optname, void *optval);

int SockBind(
    int sockfd,
    std::string ipaddr,
    std::string port,
    int family,
    struct sockaddr_in sockaddress);

int SockListen(int sockfd, int max_queue);

int SockAccept(int sockfd, struct sockaddr_in sockaddress);

struct addrinfo *GetAddrInfo(
    const char *node,
    const char *port,
    int family,
    int socktype);

int SockConnect(int sockfd, struct addrinfo info);

void SockSelect(); // TODO

bool SockReceive(int rec_sockfd, void *rec_buf, size_t len);

bool SockSend(int send_sockfd, unsigned char *send_buf, size_t len);

int SockClose(int sockfd);
#endif