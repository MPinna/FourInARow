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

int main()
{
    short int ret{-1};
    bool _check{false};
    unsigned char sbuf[] = "Hello Server!";
    unsigned char rbuf[14];
    struct sockaddr_in _peersock;
    Slave *client = new Slave();
    
    // Create socket, get server info and connect
    ret = client->InitClient(AF_INET, SOCK_STREAM, 0, AF_INET);
    if(ret < 0)
    {
        std::cerr << "client->InitClient() failed!\nReturn code: " << ret << std::endl;
        exit(1);
    }

    _check = SockSend(client->GetClientfd(), sbuf, sizeof(sbuf));
    if(_check)
        std::cout << "HELLO SENT!" << std::endl;

    _check = SockReceive(client->GetClientfd(), rbuf, sizeof(rbuf));
    if(_check)
        std::cout << "RECEIVED: " << rbuf << std::endl;

    // Manage peer-to-peer connection
    // TOCHECK
    std::string assign;
    std::cout << "Please, type s (for sender) r (for receiver): ";
    getline(std::cin, assign);
    if (!std::cin)
    {
        std::cerr << "Error during input\n";
        exit(1);
    }
    if(assign == "r")
        std::thread th1(&Slave::InitPeerReceiver, &client, AF_INET, SOCK_STREAM, 0, AF_INET, SOL_SOCKET, SO_REUSEADDR, 1, BACKLOG_QUEUE, _peersock);
    if(assign == "s")
        std::thread th1(&Slave::InitPeerSender, &client, AF_INET, SOCK_STREAM, 0, AF_INET, _peersock);
    
    return 1;
}