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
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>

#define DEFAULT_SERVER_ADDR "localhost"
#define DEFAULT_SERVER_PORT 8888

class Connection
{
private:
    /* data */
    int _sock_fd;
    int _port;
    int opp_port;
    int _optvalue;
    std::string _addr;
    std::string opp_addr;
    struct sockaddr_in _addr_in;
    struct sockaddr_in opp_addr_in;
public:
    /* Constructor */
    Connection();
    // TODO fai i test e poi vedi se puoi prendere la roba dal `this`
    Connection(int _sock_fd, int _port, int opp_port, int _optvalue, std::string _addr, std::string opp_addr);
    
    /* Destructor */
    ~Connection();

    bool Connection::sendData(std::string data);
    std::string Connection::receiveData(int size);
    bool Connection::connectClient(std::string _addr, int _port);
    bool Connection::connectServer();
};
#endif