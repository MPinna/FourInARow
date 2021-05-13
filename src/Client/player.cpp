/**
 * SECTION
 * The Client class, it is responsible for:
 *  - Launch a new client
 *  - Interact with server
 *  - Challenge another client
 *  - Play a four-in-a-row game
 *  - Init a board
 */
#include "../../include/Client/slave.hpp"
#include <iostream>
#include <cstring>
#include <thread>

int main()
{
    bool _check{false};
    unsigned char buf[1024];
    struct sockaddr_in _peersock;
    Slave *client = new Slave();
    
    // Create socket, get server info and connect
    client->InitClient(AF_INET, SOCK_STREAM, 0, AF_INET);
    
    /* Exchange data */
    _check = SockSend(client->getClientfd(), "Hello Server from player 1", strlen("Hello Server from player 1"));
    if(_check)
        std::cout << "Hello Sent" << std::endl;

    _check = SockReceive(client->getClientfd(), buf, strlen("Hello client from server"));
    if(_check)
        std::cout << "Received: " << buf << std::endl;

    // SECTION: peer2peer 
    // std::thread peerReceiver ([=]{client->InitPeerReceiver(AF_INET, SOCK_STREAM, 0, AF_INET, SOL_SOCKET, SO_REUSEADDR, 1, BACKLOG_QUEUE, _peersock);});
    // FIXME
    std::thread th1(&Slave::InitPeerReceiver, &client, AF_INET, SOCK_STREAM, 0, AF_INET, SOL_SOCKET, SO_REUSEADDR, 1, BACKLOG_QUEUE, _peersock);


    return 1;
}