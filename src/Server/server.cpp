/** DESCRIPTION
 * This file it used to instantiate a server instance capable to:
 *  - Accept a player
 *  - List online players
 *  - Make possible to run challenge between two 
*/
#include "../../include/Server/master.hpp"
#include <iostream>
#include <cstring>

int main(int argc, char *argv[])
{
    Master *server = new Master();
    unsigned char buf[1024];
    bool _check{false};
    // Create socket, configure parameters, bind it, make it ready for listen
    server->InitServer(AF_INET, SOCK_STREAM, 0, AF_INET, SOL_SOCKET, SO_REUSEADDR, 1, BACKLOG_QUEUE);

    for (;;)
    {
        server->_exchangefd = SockAccept(server->_serverfd, server->_serversock);
        _check = SockReceive(server->_exchangefd, buf, strlen("Hello Server from player 1"));
        if(_check)
            std::cout << "RECEIVED: " << buf << std::endl;

        _check = SockSend(server->_exchangefd, "Hello client from server", strlen("Hello client from server"));
        if(_check)
            std::cout << "RESPONSE SENT" << std::endl;
    }

    return 1;
}