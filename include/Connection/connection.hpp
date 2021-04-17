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

class Connection
{
private:
    /* data */
    struct sockaddr_in _addr_in;
    int _optvalue;
    int _sock_fd;
    int _port;
public:
    int opp_port;
    struct sockaddr_in opp_addr_in;
    
    /* Default constructor */
    Connection();
    // TODO fai i test e poi vedi se puoi prendere la roba dal `this`
    // Connection(int _sock_fd, int _port, int opp_port, int _optvalue, std::string _addr, std::string _opp_addr);

    /* Class members */
    bool connectClient(std::string _addr, int _port);
    bool sendData(std::string data);
    bool connectServer();
    std::string receiveData(int size);
};
#endif