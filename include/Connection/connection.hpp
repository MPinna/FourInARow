/**
 * SECTION
 * The connection class it is used to manage two nodes on a network to communicate with each other. More in detail, it is responsible for:
 *  - Initialize Server socket (Socket, setsockopt, Bind, Listen, Accept)
 *  - Initialize Client socket (Socket, Connect)
 *  - Manage send() / receive() (for both Client and Server)
 */
#ifndef _CONNECTION_H_
#define _CONNECTION_H_
#include <arpa/inet.h>
#include <string>

class Connection
{
private:
    /* data */
    int sock;
    std::string port;
    std::string _addr;
    struct sockaddr_in opponent_addr;

public:
    /* constructor */
    Connection();
    Connection(int sock, std::string port, std::string _addr);
    /* Destructor */
    ~Connection();

    bool Connection::sendData();
    std::string Connection::receiveData();
    bool Connection::connectClient();
    bool Connection::connectServer();
};
#endif