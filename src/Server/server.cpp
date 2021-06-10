/** DESCRIPTION
 * This file it used to instantiate a server instance capable to:
 *  - Accept a player
 *  - List online players
 *  - Make possible to run challenge between two 
*/
#include "../../include/Server/master.hpp"

int main(int argc, char *argv[])
{
    Master *server = new Master();
    unsigned char sbuf[] = "Hello Client!";
    unsigned char rbuf[14];
    bool _check{false};
    
    // Create socket, configure parameters, bind it, make it ready for listen
    server->InitServer(AF_INET, SOCK_STREAM, 0, AF_INET, SOL_SOCKET, SO_REUSEADDR, 1, BACKLOG_QUEUE);

    // Manage Clients incoming connections
    for (;;)
    {
        server->setReceivefd(SockAccept(server->getServerfd(), server->_serversock));
        _check = SockReceive(server->getReceivefd(), rbuf, sizeof(rbuf));
        if(_check)
            std::cout << "RECEIVED: " << rbuf << std::endl;

        _check = SockSend(server->getReceivefd(), sbuf, sizeof(sbuf));
        if(_check)
            std::cout << "RESPONSE SENT" << std::endl;
    }

    return 1;
}