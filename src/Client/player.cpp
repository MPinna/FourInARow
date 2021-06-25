/**
 * SECTION
 * The Client class, it is responsible for:
 *  - Launch a new client
 *  - Interact with server
 *  - Challenge another client
 *  - Play a four-in-a-row game
 *  - Init a board
 */
#include "../../include/Messages/packet.hpp"
#include "../../include/Client/slave.hpp"
#include <chrono>
int rval{0};
int main()
{
    Slave *client = new Slave();
    Packet *packet = new Packet();
    short int _ret_code{-1};
    

    /** 
     * SECTION_START: Create socket, get server info and connect
     */
    _ret_code = client->InitSlave(AF_INET, SOCK_STREAM, 0, AF_INET);
    if(_ret_code < 0)
    {
        std::cerr << " <== main(initSlave)";
        exit(1);
    }
    else
    {
        _ret_code = PacketReceive(client->GetClientfd(), packet, 0);
        if(_ret_code < 0)
        {
            std::cerr << "ERROR"; // TODO check how to manage error chain
            exit(1);
        }
        else
            packet->print();
    }
    /**
     * SECTION_END
     */

    /**
     * SECTION_START
     * send packet / wait for response
     */
    packet->reallocPayload((unsigned char *)"Just a message");
    _ret_code = PacketSend(client->GetClientfd(), packet);
    if(_ret_code < 0)
    {
        std::cerr << "ERROR"; // TODO check how to manage error chain
        exit(1);
    }
    _ret_code = PacketReceive(client->GetClientfd(), packet, 0);
    if(_ret_code < 0)
    {
        std::cerr << "ERROR"; // TODO check how to manage error chain
        exit(1);
    }
    else
        packet->print();
    // SECTION_END    

    /**
     * SECTION_START
     * Manage peer-to-peer connection
     */
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
        std::thread t1(&Slave::InitPeerReceiver, client, AF_INET, SOCK_STREAM, 0, AF_INET, SOL_SOCKET, SO_REUSEADDR, 1, BACKLOG_QUEUE);
        t1.join();
        
    }   
    else if (assign == "s")
    {
        std::thread t2(&Slave::InitPeerSender, client, AF_INET, SOCK_STREAM, 0, AF_INET);
        t2.join();
    }
    else
        std::cout << "Invalid value!" << std::endl;
    // NOTE: all this block will be replaced by an automatic generation code which will be assigned when a player challenge another one
    /** 
     * SECTION_END
     */

    srand(time(0));
    int tmp = rand() % 10;
    sleep(tmp);
    std::cout << "RAND: " << tmp << std::endl;

    /**
     * SECTION_START
     * Close connection
     */
    packet->reallocPayload((unsigned char *)"");
    _ret_code = PacketSend(client->GetClientfd(), packet);
    if(_ret_code < 0)
    {
        std::cerr << "/!\\ ERROR";
        exit(1);
    }

    _ret_code = PacketReceive(client->GetClientfd(), packet, 0);
    if(_ret_code < 0)
    {
        std::cerr << "/!\\ ERROR"; 
        exit(1);
    }
    else
        packet->print();
    SockClose(client->GetClientfd());

    packet->~Packet();
    client->~Slave();
    
    return 1;
}