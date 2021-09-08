/**
 * SECTION
 * The Client class, it is responsible for:
 *  - Launch a new client
 *  - Interact with server
 *  - Challenge another client
 *  - Play a four-in-a-row game
 *  - Init a board
 * TODO
 * manage all error (exit, go on, message error and close connection, etc...)
 */
#include "../../libs/OpensslDS/include/digitalSignature.hpp"
#include "../../libs/OpensslUtils/include/sslutils.hpp"
#include "../../libs/OpensslX509/include/x509.hpp"
#include "../../include/Messages/Client/auth.hpp"
#include "../../include/Messages/Server/auth.hpp"
#include "../../include/Messages/packet.hpp"
#include "../../include/Utils/structures.hpp"
#include "../../include/Utils/utils.hpp"
#include "../../include/Client/slave.hpp"
#include <chrono>
#include <cerrno>

int main(int argc, char **argv)
{
    short int ret{-1};
    
    // Check initial arguments
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
    std::string prvkey_file_name{client->_username.append(".pem")};
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

    /* Init security check structure */
    SecurityCheck check = SecurityCheck();

    /**
     * SECTION_START
     * Client - Server authentication
     */
    ClientHello hello = ClientHello();
    hello.setUsername(argv[1]);
    hello._port_number = client->_port;
    hello._nonce = getRandomInt();
    unsigned char *hello_buf;
    size_t hello_size = hello.HtoN(&hello_buf);
    
    /* Make ESP packet */
    ESP *esp = new ESP();
    esp->initCounter();
    esp->setType(hello.getType());
    esp->setPayload(hello_buf, hello_size);
    delete hello_buf;
    
    check._nonce = hello._nonce;
    check._sent  = esp->getCounter();

    unsigned char *packet_buf;
    size_t packet_size = esp->htonPacket(&packet_buf);
    
    /* Sign hello packet */
    const EVP_MD *cipher = EVP_sha256();
    size_t sig_len;
    unsigned char *sig_buf;
    ret = digestSign(packet_buf, packet_size, &sig_buf, &sig_len, prvkey, cipher);
    if(ret < 0)
    {
        std::cerr << argv[0] << "Sign failed!" << std::endl;
    }
    esp->setTag(sig_buf, sig_len);
    
    delete[] packet_buf;
    delete[] sig_buf;

    /* Print out */
    esp->print();
    hello.print();
    esp->printTag();

    ESPPacketSend(client->GetClientfd(), esp);
    /* VERIFIED Send ESP packet */

    /* Receive Certificate */
    ESPPacketReceive(client->GetClientfd(), esp, NULL);

    /* TODO Check counter */
    // if(esp->getCounter() != check._expected)
    //     std::cerr << "Attention: possible reply attack! Sequence number already seen" << std::endl;

    /* Setup certificate and verify it */
    const unsigned char *serialized_x509;
    serialized_x509 = esp->getPayload();
    X509 *x509 = d2i_X509(NULL, &serialized_x509, esp->getPayloadSize());
    ret = verifyCert(store, x509);
    if(ret < 1)
    {
        std::cerr << "Server certificate is not valid, authentication phase failed!" << std::endl;
    }
    else
        std::cout << "Server certificate is valid, authentication will continue" << std::endl;
    EVP_PKEY *server_pubkey{NULL};

    server_pubkey = X509_get_pubkey(x509);

    check.updateFields();

    /* Receive response (ClientHello) */
    unsigned char *response_buff;
    ret = ESPPacketReceive(client->GetClientfd(), esp, &response_buff);
    if(ret < 1)
    {
        std::cerr << "Server response error: failed to receive DH key and parameters" << std::endl; 
    }

    /* TODO Check counter */
    // if(esp->getCounter() != check._expected)
    //     std::cerr << "Attention: possible reply attack! Sequence number already seen" << std::endl;

    /* Verify validity and check packet contents */
    ret = digestVerify(
        response_buff,
        esp->getPacketSize(),
        esp->getTag(),
        esp->getTaglen(),
        server_pubkey,
        EVP_sha256()
    );
    if(ret < 1)
        std::cerr << "DH Key exchange verification failure: signature mismatch" << std::endl;
    
    /* Get information */
    ServerResponse response = ServerResponse();
    response.NtoH(esp->getPayload());

    /* Retrieve DH params and DH public key of server */
    BIO *dh_params_bio = BIO_new(BIO_s_mem());
    DH *dh_params{NULL};
    BIO_write(dh_params_bio, response.params._params, response.params._params_length);
    PEM_read_bio_DHparams(dh_params_bio, &dh_params, NULL, NULL);
    
    BIO *dh_serverkey_bio = BIO_new(BIO_s_mem());
    BIO_write(dh_serverkey_bio, response.dh_key._dh_key, response.dh_key._dh_lenght);
    EVP_PKEY *serverdh_key{NULL};
    PEM_read_bio_PUBKEY(dh_serverkey_bio, &serverdh_key, NULL, NULL);

    /* Generate client DH Key */
    
    /* TODO Derive the shared secret */


    // std::cout << "esp.getSize(): "<< esp->getSize() << std::endl;
    // std::cout << "esp.getPayloadSize(): "<< esp->getPayloadSize() << std::endl;
    // std::cout << "esp.getHeaderSize(): "<< esp->getHeaderSize() << std::endl;
    
    // std::cout << "Just before" << std::endl;
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