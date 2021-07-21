/**
 * SECTION
 * The Client class, it is responsible for:
 * TODO
 *  - Launch a new client
 *  - Interact with server
 *  - Challenge another client
 *  - Play a four-in-a-row game
 *  - Init a board
 */
#include "../../libs/OpensslDS/include/digitalSignature.hpp"
#include "../../libs/OpensslX509/include/x509.hpp"
#include "../../include/Utils/structures.hpp"
#include "../../include/Messages/packet.hpp"
#include "../../include/Client/slave.hpp"
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <chrono>
#include <cerrno>
#include <openssl/err.h> // for error descriptions
using namespace std;

int main()
{
    GameInfo gameinfo;
    gameinfo.username.append("Player2");
    Slave *client = new Slave();
    Packet *packet = new Packet();
    short int ret{-1};
    
    /**
     * SECTION_START
     * Openssl keys, setup store
     */
    X509_STORE* store;
    ret = SetupStore(&store);
    if(ret <= 0)
    {
        if(ret == 0)
            std::cout << " <== Player exit";
        if(ret < 0)
            std::cerr << " <== Player error!";
        
        exit(1);
    }
    // 

    /** 
     * SECTION_START: 
     * Create socket, get server info and connect
     */
    ret = client->InitSlave(AF_INET, SOCK_STREAM, 0, AF_INET);
    if(ret < 0)
    {
        std::cerr << " <== main(initSlave)";
        exit(1);
    }
    else
    {
        ret = PacketReceive(client->GetClientfd(), packet, 0);
        if (ret < 0)
        {
            std::cerr << " <== main()";
            exit(1);
        }
        packet->print();
    }
    // SECTION_END

    /**
     * SECTION_START
     * Authentication phase
     * TODO: send signed message
     * challenge - response
    */


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
    ret = PacketSend(client->GetClientfd(), packet);
    if(ret < 0)
    {
        std::cerr << " <== main()::PacketSend() failed to send close connection signal";
        exit(1);
    }
    ret = PacketReceive(client->GetClientfd(), packet, 0); 
    if(ret < 0)
    {
        std::cerr << " <== main()::PacketReceive() failed to receive close connection signal"; 
        exit(1);
    }
    else
        packet->print();
    SockClose(client->GetClientfd());

    packet->~Packet();
    client->~Slave();
    
    return 1;
}