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
#include "../../libs/OpensslUtils/include/sslutils.hpp"
#include "../../libs/OpensslX509/include/x509.hpp"
#include "../../include/Messages/Client/auth.hpp"
#include "../../include/Messages/packet.hpp"
#include "../../include/Utils/structures.hpp"
#include "../../include/Utils/utils.hpp"
#include "../../include/Client/slave.hpp"
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h> // for error descriptions
#include <chrono>
#include <cerrno>
using namespace std;

int main(int argc, char **argv)
{
    short int ret{-1};
    std::string test;
    if(argc!=2)
    {
        std::cerr << "Error inserting parameters. \nUsage " << argv[0] << " (username)" << std::endl;
        exit(1);
    }

    /** 
     * SECTION_START: 
     * Create socket, get server info and connect
     */
    Slave *client = new Slave(argv[1]);
    ret = client->InitSlave(AF_INET, SOCK_STREAM, 0, AF_INET);
    if(ret < 0)
    {
        std::cerr << " <== main(initSlave)";
        exit(1);
    }

    /* Retrieve private key */
    EVP_PKEY* prvkey;
    std::string prvkey_file_name{argv[1]};
    prvkey_file_name.append(".pem");
    ret = RetrievePrvKey(&prvkey, prvkey_file_name.c_str());
    if (ret <= 0)
    {
        if (ret == 0)
            std::cout << " <== Player exit";
        if (ret < 0)
            std::cerr << " <== Player error!";

        exit(1);
    }

    /* Setup store */
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

    /**
     * SECTION_START
     */
    
    /* FIXME Make hello message */
    ClientHello hello = ClientHello();
    // hello.setUsername(argv[1]);
    hello._port_number = client->_port;
    hello._nonce = getRandomInt();
    unsigned char *hello_buf;
    size_t hello_size = hello.HtoN(&hello_buf);
    
    /* Make ESP packet */
    ESP *esp = new ESP();
    esp->initCounter();
    esp->setType(hello.getType());
    esp->setPayload(hello_buf, hello_size);
    unsigned char *packet_buf;
    size_t packet_size = esp->htonPacket(packet_buf);

    /* Sign ESP packet */
    const EVP_MD *cipher = EVP_sha512();
    size_t sig_len;
    unsigned char *sig_buf;
    ret = digestSign(packet_buf, packet_size, &sig_buf, &sig_len, prvkey, cipher);
    if(ret < 0)
    {
        std::cerr << argv[0] << "Sign failed!" << std::endl;
    }
    esp->setTag(sig_buf, sig_len);
    // delete[] hello_buf;
    // delete[] sig_buf;
    // delete[] packet_buf;

    /* Send ESP packet */
    esp->print();
    hello.print();
    esp->printTag();
    
    // std::cout << "esp.getSize(): "<< esp->getSize() << std::endl;
    // std::cout << "esp.getPayloadSize(): "<< esp->getPayloadSize() << std::endl;
    // std::cout << "esp.getHeaderSize(): "<< esp->getHeaderSize() << std::endl;
    
    // std::cout << "Just before" << std::endl;
    ESPPacketSend(client->GetClientfd(), esp); // TOCHECK
    // std::cout << "Just after" << std::endl;
    
    
    // sockaddr_in my_addr;
    // char myIP[16];
    // unsigned int myPort;
    // bzero(&my_addr, sizeof(my_addr));
    // socklen_t len = sizeof(my_addr);
    // getsockname(client->GetClientfd(), (struct sockaddr *) &my_addr, &len);
    // inet_ntop(AF_INET, &my_addr.sin_addr, myIP, sizeof(myIP));
    // myPort = ntohs(my_addr.sin_port);

    // printf("Local ip address: %s\n", myIP);
    // printf("Local port : %u\n", myPort);

    /**
     * SECTION_START
     * Manage peer-to-peer connection
     */
    Packet packet = Packet();
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

    /**
     * SECTION_START
     * Close connection
     */
    packet.reallocPayload((unsigned char *)"", 0);
    packet.setType(CLOSE_SIGNAL);
    ret = PacketSend(client->GetClientfd(), &packet);
    if(ret < 0)
    {
        std::cerr << " <== main()::PacketSend() failed to send close connection signal";
        exit(1);
    }
    ret = PacketReceive(client->GetClientfd(), &packet, 0); 
    if(ret < 0)
    {
        std::cerr << " <== main()::PacketReceive() failed to receive close connection signal"; 
        exit(1);
    }
    else
        packet.print();
    SockClose(client->GetClientfd());

    packet.~Packet();
    client->~Slave();
    
    return 1;
}