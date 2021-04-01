/**
 * SECTION
 * The server class is responsible to manage all server functionalities:
 *  - Process Client authentication requests
 *  - Forward Client challenge
 *  - Send information about security DH parameter to the peers
 *  - Maintain the online player list
 *  - Maintain the public key list associated to each registered user
 */
#include "connection.hpp"

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
    : _name{_name_val}
{
}

Server::~Server()
{
}

int main()
{
    Server server;
    bool ret;
    ret = server.connection.connectServer();
    if (ret != true)
        std::cout << "ERROR" << std::endl;
    else
        std::cout << "OK" << std::endl;

    return 0;
}