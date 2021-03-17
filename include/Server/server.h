#include "connection.h"
#include "crypto.h"

class Server
{
private:
    /* data */
    Connection socket;
    Crypto secure_communication_protocol;
public:
    Server(/* args */);
    ~Server();
};

Server::Server(/* args */)
{
}

Server::~Server()
{
}