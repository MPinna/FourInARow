/**
 * SECTION
 * The server class is responsible to manage all server functionalities:
 *  - Process Client authentication requests
 *  - Forward Client challenge
 *  - Send information about security DH parameter to the peers
 *  - Maintain the online player list
 *  - Maintain the public key list associated to each registered user
 */
#include "Connection/connection.hpp"

class Server    
{
private:
    /* data */
public:
    Server(/* args */);
    ~Server();
    Connection connection;
};

Server::Server(/* args */)
{
}

Server::~Server()
{
}

int main()
{
    Server server;
    server.connection.connectServer();
    
    return 0;
}