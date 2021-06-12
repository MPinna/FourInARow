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
    short int _check{-1};

    // Create socket, configure parameters, bind it, make it ready for listen
    _check = server->InitMaster(AF_INET, SOCK_STREAM, 0, AF_INET, SOL_SOCKET, SO_REUSEADDR, 1, BACKLOG_QUEUE);
    if(_check < 0)
    {
        std::cerr << "main::server->InitMaster failed!" << std::endl;
        exit(1);
    }

    // Manage Clients incoming connections
    _check = server->Run(); 
    if(_check < 0)
    {
        std::cerr << "main::Run() failed!" << std::endl;
        exit(1);
    }
    SockClose(server->getReceivefd());
    SockClose(server->getMasterfd());
    return 1;
}