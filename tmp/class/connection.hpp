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
#include <iostream>
#include <cstring>
#include <vector>
#include <string>

#define DEFAULT_SERVER_ADDR "localhost"
#define DEFAULT_SERVER_PORT 8888

class Connection
{
private:
    /* data */
    int _sock_fd;
    int rec_sock_fd;
    int _port;
    int opp_port;
    int _optvalue;
    // std::string _addr; TOCHECK do you need these?
    // std::string opp_addr;
    struct sockaddr_in _addr_in;
    struct sockaddr_in opp_addr_in;
public:
    /* Constructor */
    Connection();
    // TODO fai i test e poi vedi se puoi prendere la roba dal `this`
    Connection(int _sock_fd, int rec_sock_fd, int _port, int opp_port, int _optvalue);

    bool connectServer();
    bool connectClient(std::string _addr, int _port);
    bool sendData(std::string data);
    bool readNBytes(int fd, void *buf, std::size_t n);
    std::string receiveData(int size);
};
#endif