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
    unsigned char sbuf[] = "Hello Server!";
    unsigned char rbuf[14];
    struct sockaddr_in _peersock;
    Slave client = Slave();
    
    // Create socket, get server info and connect
    ret = client.InitClient(AF_INET, SOCK_STREAM, 0, AF_INET);
    if(ret < 0)
    {
        std::cerr << "main::client->InitClient() failed!\nReturn code: " << ret << std::endl;
        exit(1);
    }

    ret = SockSend(client.GetClientfd(), sbuf, sizeof(sbuf));
    if(ret < 0)
    {
        std::cerr << "main::client->InitClient() failed!\nReturn code: " << ret << std::endl;
        exit(1);
    }
    else
        std::cout << "HELLO SENT!" << std::endl;

    ret = SockReceive(client.GetClientfd(), rbuf, sizeof(rbuf));
    if(ret < 0)
    {
        std::cerr << "main::client->InitClient() failed!\nReturn code: " << ret << std::endl;
        exit(1);
    }
    else
        std::cout << "RECEIVED: " << rbuf << std::endl;

    // SECTION_START
    // Manage peer-to-peer connection
    std::string assign;
    std::cout << "Please, type s (for sender) r (for receiver): ";
    getline(std::cin, assign);
    if (!std::cin)
    {
        std::cerr << "Error during input\n";
        exit(1);
    }
    if(assign == "r")
    {   
        std::thread t1(&Slave::InitPeerReceiver, &client, AF_INET, SOCK_STREAM, 0, AF_INET, SOL_SOCKET, SO_REUSEADDR, 1, BACKLOG_QUEUE, _peersock);
        t1.join();
    }
    else if (assign == "s")
    {
        std::thread t2(&Slave::InitPeerSender, &client, AF_INET, SOCK_STREAM, 0, AF_INET, _peersock);
        t2.join();
    }
    else
        std::cout << "Invalid value!" << std::endl;

    // NOTE: all this block will be replaced by an automatic generation code which will be assigned when a player challenge another one
    // SECTION_END
    
    return 1;
}