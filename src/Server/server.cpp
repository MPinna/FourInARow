/** DESCRIPTION
 * This file it used to instantiate a server instance capable to:
 *  - Accept a player
 *  - List online players
 *  - Make possible to run challenge between two 
*/
#include "../../libs/OpensslDS/include/digitalSignature.hpp"
#include "../../libs/OpensslUtils/include/sslutils.hpp"
#include "../../libs/OpensslX509/include/x509.hpp"
#include "../../include/Messages/Client/auth.hpp"
#include "../../include/Utils/structures.hpp"
#include "../../include/Server/master.hpp"
#include <vector>

int main(int argc, char *argv[])
{
    Master *server = new Master();
    int ret{-1};

    // Create socket, configure parameters, bind it, make it ready for listen
    ret = server->Init(AF_INET, SOCK_STREAM, 0, AF_INET, SOL_SOCKET, SO_REUSEADDR, 1, BACKLOG_QUEUE);
    if(ret < 0)
    {
        std::cerr << "main::server->Init() failed!" << std::endl;
        exit(1);
    }

    /* Retrieve certificate and serialize it */
    X509* x509;
    ret = RetrieveCert(&x509);
    if(ret <= 0)
    {
        if(ret < 0)
            std::cerr << "<== Server Error!" << std::endl;
        if(ret == 0)
            std::cerr << "<== Server exit()" << std::endl;
        exit(1);
    }

    unsigned char *x509_buff;
    x509_buff = NULL;
    size_t x509_len = i2d_X509(x509, &x509_buff);  // converting to unsigned char*
    
    // Server life cycle
    std::string wlc_msg{"Connected with: " + server->getIPAddr() + ":" + server->getPort()};
    unsigned char rcv_msg[] = "Message Received";
    unsigned char cls_msg[] = "Close signal received";
    size_t rcv_msg_size = strlen((char *)rcv_msg);
    size_t cls_msg_size = strlen((char *)cls_msg);
    std::vector<ClientInfo> clients(server->_fdmax+1);
    for (;;)
    {   
        int nbytes{-1};
        server->_read_fds = server->_master_set; // copy it
        if(select(server->_fdmax + 1, &server->_read_fds, NULL, NULL, NULL) == -1)
        {
            std::cerr << " <== server()::receive(select)";
            return -1;
        }

        // run through the exisiting connections looking for data to read
        for (size_t i = 0; i <= server->_fdmax; i++)
        {
            if(FD_ISSET(i, &server->_read_fds))
            {
                // SECTION New client connected to the server: manage auth phase
                if (i == server->_masterfd)
                {
                    server->_addrlen = sizeof(server->_peeraddr);
                    server->_receivefd = SockAccept(server->_masterfd, (sockaddr *)&server->_peeraddr, &server->_addrlen);
                    if(server->_receivefd == -1)
                        std::cerr << " <== server()::receive(sockAccept)";
                    else
                    {
                        FD_SET(server->_receivefd, &server->_master_set);
                        if (server->_receivefd > server->_fdmax)
                            server->_fdmax = server->_receivefd;
                        clients.resize(server->_fdmax+1);
                    }
                    std::cout << 
                        "==> New connection from " << 
                        inet_ntoa(server->_peeraddr.sin_addr) << 
                        " on socket: " << server->_receivefd << 
                    std::endl;
                     
                    /* SECTION_START: authentication phase */
                    
                    /* Receive Challenge */
                    unsigned char *msg;
                    ret = ESPPacketReceive(server->_receivefd, &clients.at(server->_receivefd).packet, &msg);
                    if(ret < 1)
                    {
                        std::cerr << "ClientHello() receive error from socket: " << server->_receivefd << std::endl;
                        continue;
                    }
                    
                    /* Get message info: content and signature */
                    ClientHello hello = ClientHello();
                    hello.NtoH(clients.at(server->_receivefd).packet._payload);
                    
                    /* Store the received nonce from the client */
                    clients.at(server->_receivefd).check._nonce = hello._nonce;
                    clients.at(server->_receivefd).check._received = clients.at(server->_receivefd).packet.getCounter();
                    clients.at(server->_receivefd).check.updateFields();
                    
                    // Print received info
                    clients.at(server->_receivefd).packet.print();
                    hello.print();
                    clients.at(server->_receivefd).packet.printTag();

                    /* Retrieve pubkey of client 'i' */
                    std::string pubkey_filename{"key/"};
                    pubkey_filename.append(hello._username);
                    pubkey_filename.append(".pem");
                    
                    ret = RetrievePubKey(&clients.at(server->_receivefd)._pubkey, pubkey_filename.c_str());
                    if (ret <= 0)
                    {
                        if (ret == 0)
                            std::cout << " <== Server failed";
                        if (ret < 0)
                            std::cerr << " <== Internal Server error!";
                        
                        continue;
                    }

                    /* Check signature */
                    const EVP_MD *cipher = EVP_sha256();
                    ret = digestVerify(
                        msg,
                        clients.at(server->_receivefd).packet.getPacketSize(),
                        clients.at(server->_receivefd).packet.getTag(),
                        clients.at(server->_receivefd).packet.getTaglen(),clients.at(server->_receivefd)._pubkey,
                        cipher
                    );
                    if (ret < 0)
                    {
                        std::cerr << argv[0] << "Verify failed!" << std::endl;
                        continue;
                    }
                    
                    /* Write certificate inside payload */
                    clients.at(server->_receivefd).packet.reallocPayload(x509_buff, x509_len);
                    
                    /* Send Certificate */
                    ret = ESPPacketSend(server->_receivefd, &clients.at(server->_receivefd).packet);

                    /* Load Diffie-Hellman parameters in dh_params */
                    EVP_PKEY* dh_params;
                    if (NULL == (dh_params = EVP_PKEY_new()))
                        std::cerr << "Error setting up DH parameters" << std::endl;

                    DH *temp_params = DH_get_2048_224();
                    if(1 != EVP_PKEY_set1_DH(dh_params, temp_params))
                        std::cerr << "EVP_PKEY_set1_DH failed (fail to set the dh key)" << std::endl;

                    /* Create context for the key generation */
                    EVP_PKEY_CTX* dh_ctx;
                    if(!(dh_ctx = EVP_PKEY_CTX_new(dh_params, NULL)))
                        std::cerr << "DH Context failed (error during context creation)" << std::endl;

                    /* Generation of private/public key pair */
                    std::cout << "... Generating ephemeral DH KeyPair ..." << std::endl;
                    EVP_PKEY* my_dhkey = NULL;
                    if(1 != EVP_PKEY_keygen_init(dh_ctx))
                        std::cerr << "EVP_PKEY_keygen_init() failed" << std::endl;
                    if(1 != EVP_PKEY_keygen(dh_ctx, &my_dhkey))
                        std::cerr << "EVP_PKEY_keygen() failed generating DH Keys" << std::endl;

                    std::cout << "EVP_PKEY_size "<< EVP_PKEY_size(my_dhkey) << std::endl;
                    long int size{EVP_PKEY_size(my_dhkey)};
                    unsigned char *buf = new unsigned char[size];
                    memcpy(buf, dh_params, size);
                    
                    /* Write pubkey and dh parameters inside a buffer */
                    BIO *server_bio = BIO_new(BIO_s_mem());
                    PEM_write_bio_DHparams(server_bio, temp_params);
                    const int dh_size = BIO_pending(server_bio);
                    std::cout << "dh_size: "<< dh_size << std::endl;
                    unsigned char *dh_char = new unsigned char[dh_size];
                    BIO_read(server_bio, dh_char, dh_size);
                    std::cout << "wrote bio data into char " << std::endl;

                    /* CLIENT SIDE */
                    BIO *client_bio = BIO_new(BIO_s_mem());
                    BIO *client_bio2 = BIO_new(BIO_s_mem());
                    DH *new_params = NULL;
                    BIO_write(client_bio, dh_char, dh_size);
                    BIO_write(client_bio2, dh_char, dh_size);
                    const int test = BIO_get_mem_data(client_bio, dh_char);
                    PEM_read_bio_DHparams(client_bio, &new_params, NULL, NULL);
                    std::cout << "test: "<< test << std::endl;
                    std::cout << "PEM_read_bio_DHparams" << std::endl;
                    
                    unsigned char *dh_char2 = new unsigned char[dh_size];
                    BIO_read(client_bio2, dh_char2, dh_size);
                    std::cout << "wrote bio data into char " << std::endl;

                    for(int i = 0; i < dh_size; i++)
                        std::cout << dh_char[i] << ".";

                    std::cout << "\n\n BIO2:\n";

                    for(int i = 0; i < dh_size; i++)
                        std::cout << dh_char2[i] << ".";

                    // NOTE

                    /* Send the public key inside my_prvkey to the peer */

                    /* Retrieve the public key of the peer and store it in peer_pubkey */

                    /* Initializing shared secret derivation context */

                    /* Retrieving shared secret's length */

                    /* Deriving shared secret */

                    /* Setup Diffie-Hellman parameters, get priv, pub key pairs. Send the public parameters along with my pubkey to the client */

                    /* Receive DH Param */

                    /* TOCHECK Va alla fine dell'autenticazione, N.B. devi cambiare i parametri */
                    // clients.at(server->_receivefd)._status = 1;
                    // clients.at(server->_receivefd).packet.setType(SERVER_HELLO);
                    // clients.at(server->_receivefd).packet.reallocPayload((unsigned char *)wlc_msg.c_str(), wlc_msg.length());
                    // ret = PacketSend(server->_receivefd, &clients.at(server->_receivefd).packet);
                    // if(ret == -1)
                    //      std::cerr << " <== server()::response(): wlc_msg not sent";

                    continue;
                }
                // SECTION We are going to manage communication
                else
                {   
                    nbytes = PacketReceive(i, &clients.at(i).packet, 0);
                    if(nbytes > 0)
                    {   
                        clients.at(i).packet.print();
                        clients.at(i).packet.reallocPayload(rcv_msg, rcv_msg_size);
                        ret = PacketSend(i, &clients.at(i).packet);
                        if(ret < 0)
                            std::cerr << " <== server()::response(): response msg not sent";
                            continue;
                    }
                    else if(nbytes == 0)
                    {   
		                std::cout << "Peer: " << i << " disconnected" << std::endl;
                        clients.at(i).packet.reallocPayload(cls_msg, cls_msg_size);
                        ret = PacketSend(i, &clients.at(i).packet);
                        if(ret < 0)
                            std::cerr << " <== server()::response(): close msg not sent";
                        ret = SockClose(i);
                        if(ret < 0)
		                    std::cerr << " <== server()::receive(sockClose)";
                        FD_CLR(i, &server->_master_set);
                        clients.at(i)._username.clear();
                        clients.at(i)._status = false;
                        clients.at(i).packet.reset();
                        continue;
                    }
                    else
                    {
                        std::cerr << " <== server()::receive(): unexpected error on receive";
                        ret = SockClose(i);
                        FD_CLR(i, &server->_master_set);
                        clients.at(i)._username.clear();
                        clients.at(i)._status = false;
                        clients.at(i).packet.reset();
                        std::cerr << " <== Conncetion forcefully closed";
                        continue;
                    }
                }
            }
        }
    }
 
    SockClose(server->_receivefd);
    SockClose(server->_masterfd);
    
    return 1;
}