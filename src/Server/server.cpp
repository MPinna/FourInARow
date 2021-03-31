/**
 * SECTION
 * The server class is responsible to manage all server functionalities:
 *  - Process Client authentication requests
 *  - Forward Client challenge
 *  - Send information about security DH parameter to the peers
 *  - Maintain the online player list
 *  - Maintain the public key list associated to each registered user
 */
#include "include/Connection/connection.hpp"

class Server    
{
private:
    /* data */
    std::string _name;
public:
    Server();
    Server(std::string _name);
    ~Server();
    Connection connection;
};

Server::Server()
{
}

Server::Server(std::string _name_val)
    : _name {_name_val}
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