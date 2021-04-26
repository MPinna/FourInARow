/**
 * SECTION
 * The Client class, it is responsible for:
 *  - Launch a new client
 *  - Interact with server
 *  - Challenge another client
 *  - Play a four-in-a-row game
 *  - Init a board
 */
#include <iostream>

class Client
{
private:
    /* data */
    std::string username;

public:
    Client(/* args */);
    Client(std::string username);
    ~Client();
};

Client::Client()
{
}

Client::Client(std::string username_val)
    : username{username_val}
{
}

Client::~Client()
{
}

int main()
{
    Client user {"Player1"};
    

    return 0;
}