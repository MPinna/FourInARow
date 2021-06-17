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
    unsigned char sbuf[] = "Hello Server!";
    struct sockaddr_in _peersock;
    Slave *client = new Slave();
    unsigned char rbuf[256];
    short int ret{-1};
    
    Packet *test = new Packet();
    test->setType(3);
    test->initCounter();
    test->incCounter();
    test->setPayload((unsigned char *)"this is a test", strlen("this is a test"));
    size_t tbuf_size = sizeof(struct Header) + test->getPayloadSize();
    unsigned char *tbuf = new unsigned char[tbuf_size];
    test->serialize(tbuf);
    
    std::cout << 
        "\ntest->getType(): " << test->getType() <<
        "\ntest->getCounter(): " << test->getCounter() <<
        "\ntest->getPayloadSize(): " << test->getPayloadSize() <<
    std::endl;

    // Create socket, get server info and connect
    ret = client->InitSlave(AF_INET, SOCK_STREAM, 0, AF_INET);
    if(ret < 0)
    {
        std::cerr << "main::client->InitSlave() failed!\nReturn code: " << ret << std::endl;

        exit(1);
    }

    ret = SockSendTo(client->GetClientfd(), tbuf, tbuf_size);
    if(ret < 0)
    {
        std::cerr << "main::client->SockSend() failed!\nReturn code: " << ret << std::endl;
        exit(1);
    }

    memset(rbuf, 0, sizeof(rbuf));
    ret = SockReceive(client->GetClientfd(), rbuf, sizeof(rbuf));
    if(ret < 0)
    {
        std::cerr << "main::client->SockReceive() failed!\nReturn code: " << ret << std::endl;
        exit(1);
    }
    else
    {
        std::cout << rbuf << std::endl;
    }

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
        std::thread t1(&Slave::InitPeerReceiver, client, AF_INET, SOCK_STREAM, 0, AF_INET, SOL_SOCKET, SO_REUSEADDR, 1, BACKLOG_QUEUE);
        t1.join();
        rval = rand() % 10 + 1;
        std::cout << "rval "<< rval << std::endl; 
    }
    else if (assign == "s")
    {
        std::thread t2(&Slave::InitPeerSender, client, AF_INET, SOCK_STREAM, 0, AF_INET);
        t2.join();
        rval = rand() % 5 + 1;
        std::cout << "rval "<< rval << std::endl; 
    }
    else
        std::cout << "Invalid value!" << std::endl;
    // NOTE: all this block will be replaced by an automatic generation code which will be assigned when a player challenge another one
    // SECTION_END
    
    test->setPayload((unsigned char *)"Bye!", strlen("Bye!"));
    size_t ubuf_size = sizeof(struct Header) + test->getPayloadSize();
    unsigned char *ubuf = new unsigned char[ubuf_size];
    test->serialize(ubuf);
    sleep(rval);
    ret = SockSend(client->GetClientfd(), ubuf, ubuf_size);
    if(ret < 0)
    {
        std::cerr << "main::client->InitClient() failed!\nReturn code: " << ret << std::endl;
        exit(1);
    }
   
    SockClose(client->GetClientfd());
    delete[] ubuf;
    delete[] tbuf;
    
    return 1;
}