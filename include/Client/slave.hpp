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
    struct addrinfo *_serverinfo;
    struct addrinfo *_peerinfo;
    std::string _serveraddr, _serverport;
    std::string _peeraddr, _peerport;
    int _clientfd;
    int _peerfd;
public:
    // DESCRIPTION: Constructor / Deconstructor
    Slave();
    Slave(std::string peerport);
    Slave(std::string peerport, std::string peeraddr);
    Slave(std::string serverport);
    Slave(std::string serverport, std::string serveraddr);
    ~Slave();
    // DESCRIPTION: class members
    int InitClient();
    int InitPeerReceiver();
    int InitPeerSender();
};

// Client::Client(/* args */)
// {
// }

// Client::~Client()
// {
// }
#endif