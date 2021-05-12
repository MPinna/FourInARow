/**
 * DESCRIPTION
 * The server network file it is used to manage a generic server object, with its specific fields and class members
 */
#ifndef _SERVER_SOCKET_H
#define _SERVER_SOCKET_H
class Master
{
private:
    /* data */
    std::string _ipserveraddr, _portno;
    int _serverfd, _exchangefd, opt;
    struct sockaddr_in _serversock;
    struct addrinfo *info;
public:
    Master(/* args */);
    // TODO all kind of constructor
    ~Master();
    int InitServer(int domain, int socktype, int protocol, int family, int optname, int backlog_queue);
};

#endif