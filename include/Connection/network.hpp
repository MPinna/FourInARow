/**
 * SECTION
 * The connection class it is used to manage two nodes on a network to communicate with each other. More in detail, it is responsible for:
 *  - Initialize Server socket (Socket, setsockopt, Bind, Listen, Accept)
 *  - Initialize Client socket (Socket, Connect)
 *  - Manage send() / receive() (for both Client and Server)
 */
#ifndef _CONNECTION_H_
#define _CONNECTION_H_
#include "Utils/constant.hpp"
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <string>
// TOCHECK ha senso avere una classe qua ? 
class Connection
{
private:
    /* data related to connection */
    struct sockaddr_in _addr;
    int _optvalue;
    int _sock_fd;
    int _port;
public:
    // SECTION: constructor
    // TODO: define default and specialized
    Connection();
    // Connection(int _sock_fd, int _port, int opp_port, int _optvalue, std::string _addr, std::string _opp_addr);

    // SECTION: setter

    // SECTION: getter

    /* Class members */
    // ATTENTION 
    // -- da rivedere -- START
    bool connectClient(std::string _addr, int _port);
    bool sendData(std::string data);
    bool connectServer();
    std::string receiveData(int size);
    // -- da rivedere -- END
    bool readNBytes(int socket, char *buf, std::size_t N);
};
#endif