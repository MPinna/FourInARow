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
    
    if(argc!=2)
    {
        std::cerr << "Error inserting parameters. \nUsage " << argv[0] << " (username)" << std::endl;
        exit(1);
    }

    // Retrieve private key
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
    
    /** SECTION_START
     * Start authentication
     * - Setup client hello message 
     * - Serialize and sign
     * - Send client-hello
     * - Receive Challenge response
     * - DH key exchange
     */            
    // Set up client hello
    // ClientHello hello = ClientHello();
    // hello.setUsername(argv[1]);
    // hello._port_number = client->_port;
    // hello._nonce = getRandomInt();
    // // Serialize
    // unsigned char *hello_buf;
    // size_t hsize = hello.HtoN(&hello_buf);
    // //  Make packet
    // ESP packet = ESP();
    // packet.initCounter();
    // packet.setType(hello.getType());
    // packet.setPayload(hello_buf, hsize);
    // unsigned char *packet_buf;
    // size_t psize = packet.hostToNet(&packet_buf);
    // // Sign and serialize     
    // const EVP_MD *cipher = EVP_sha512();
    // size_t sig_len;
    // unsigned char *sig_buf;
    // ret = digestSign(packet_buf, psize, &sig_buf, &sig_len, prvkey, cipher);
    // if(ret < 0)
    // {
    //     std::cerr << argv[1] << "auth failed!" << std::endl;
    // }
    // packet.setTag(sig_buf, sig_len);
    // delete[] hello_buf;
    // delete[] packet_buf;
    // delete[] sig_buf;
     
    // ESP packet is ready
    Packet packet = Packet();
    packet.initCounter();
    packet.setType(1);
    std::string hello{"Hi from: " + client->_username};
    packet.setPayload((unsigned char *)hello.c_str(), hello.length());
    ret = PacketSend(client->GetClientfd(), &packet);
    if (ret < 0)
    {
        std::cerr << " <== player: hello error";
        exit(1);
    }

    ret = PacketReceive(client->GetClientfd(), &packet, 0);
    if (ret < 0)
    {
        std::cerr << " <== player error!";
        exit(1);
    }
    packet.print();
    // SECTION_END



    /**
     * SECTION_START
     */
    // Prepare challenge message
    
    // Send challenge message

    // Receive certificate

    // Setup store
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

    // Verify Certificate

    // Get public key from certificate

    // Receive response

    // Send DH-Key
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
    packet.reallocPayload((unsigned char *)"");
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