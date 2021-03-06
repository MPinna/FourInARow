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
#include "../../include/Messages/Server/auth.hpp"
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

    /* Retrive private key */
    EVP_PKEY* prvkey;
    std::string prvkey_file_name{DEFAULT_SER_KEY};
    ret = RetrievePrvKey(&prvkey, prvkey_file_name.c_str());
    if (ret <= 0)
    {
        if (ret == 0)
            std::cout << " <== Server exit";
        if (ret < 0)
            std::cerr << " <== Server error!";

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
                            std::cout << " <== (RetrievePubKey) Server failed";
                        if (ret < 0)
                            std::cerr << " <== (RetrievePubKey) Internal Server error!";
                        
                        continue;
                    }

                    /* Check signature TODO automatize */
                    const EVP_MD *cipher = EVP_sha256();
                    ret = digestVerify(
                        msg,
                        clients.at(server->_receivefd).packet.getPacketSize(),
                        clients.at(server->_receivefd).packet.getTag(),
                        clients.at(server->_receivefd).packet.getTaglen(),clients.at(server->_receivefd)._pubkey,
                        EVP_sha256()
                    );
                    if (ret < 0)
                    {
                        std::cerr << argv[0] << "Verify failed!" << std::endl;
                        continue;
                    }
                    
                    /* Write certificate inside payload */
                    clients.at(server->_receivefd).packet.reallocPayload(x509_buff, x509_len);
                    
                    /* Send Certificate */
                    clients.at(server->_receivefd).packet.incCounter();
                    ret = ESPPacketSend(server->_receivefd, &clients.at(server->_receivefd).packet);

                    // SECTION Diffie-Hellman key-exchange

                    // PHASE-1: 
                    /* Load Diffie-Hellman parameters in dh_params */
                    EVP_PKEY* dh_params;
                    if (NULL == (dh_params = EVP_PKEY_new()))
                        std::cerr << "Error setting up DH parameters" << std::endl;

                    DH *temp_params = DH_get_2048_224();
                    if(1 != EVP_PKEY_set1_DH(dh_params, temp_params))
                        std::cerr << "EVP_PKEY_set1_DH failed (fail to set the dh key)" << std::endl;

                    // PHASE-2:
                    /* Create context for the key generation */
                    EVP_PKEY_CTX* dh_ctx;
                    if(!(dh_ctx = EVP_PKEY_CTX_new(dh_params, NULL)))
                        std::cerr << "DH Context failed (error during context creation)" << std::endl;

                    /* Generation of private/public key pair */
                    EVP_PKEY* my_dhkey = NULL;
                    if(1 != EVP_PKEY_keygen_init(dh_ctx))
                        std::cerr << "EVP_PKEY_keygen_init() failed" << std::endl;
                    if(1 != EVP_PKEY_keygen(dh_ctx, &my_dhkey))
                        std::cerr << "EVP_PKEY_keygen() failed generating DH Keys" << std::endl;


                    // TODO: da eliminare
                    /* Write pubkey and dh parameters inside a buffer */
                    // BIO *server_bio = BIO_new(BIO_s_mem());
                    // PEM_write_bio_DHparams(server_bio, temp_params);
                    // const int dh_size = BIO_pending(server_bio);
                    // unsigned char *dh_char = new unsigned char[dh_size];
                    // BIO_read(server_bio, dh_char, dh_size);
                    
                    // long int size{EVP_PKEY_size(my_dhkey)};
                    // unsigned char *buf = new unsigned char[size];
                    // memcpy(buf, dh_params, size);
                    
                    ServerResponse response = ServerResponse();
                    response.params.setParams(temp_params);
                    response.dh_key.setKey(my_dhkey);

                    unsigned char *response_buf;
                    clients.at(server->_receivefd).packet.incCounter();
                    size_t response_size = response.HtoN(&response_buf);
                    clients.at(server->_receivefd).packet.setType(response.getType());
                    clients.at(server->_receivefd).packet.reallocPayload(response_buf, response_size);

                    clients.at(server->_receivefd).packet.sign(prvkey);

                    /* Send pubkey and dh parameters */
                    ret = ESPPacketSend(server->_receivefd, &clients.at(server->_receivefd).packet);

                    std::cout << "DH Server parameters\n";
                    BIO_dump_fp(stdout, (const char *)response.params._params, response.params._params_length);

                    std::cout << "DH Server Key\n";
                    BIO_dump_fp(stdout, (const char *)response.dh_key._dh_key, response.dh_key._dh_lenght);
                    
                    /* SECTION */

                    FILE *p1w = fopen("dhkey.pem", "w");
                    if (!p1w)
                    {
                        std::cerr << "Error: cannot open file '"
                                  << "dhkey"
                                  << "' (missing?)\n";
                        exit(1);
                    }
                    PEM_write_PUBKEY(p1w, my_dhkey);
                    fclose(p1w);

                    EVP_PKEY_CTX *derive_ctx;
                    unsigned char *skey;
                    size_t skeylen;
                    std::string peer_pubkey_file_name;
                    std::cout << "Please, type the PEM file that contains the peer's DH public key: ";
                    getline(std::cin, peer_pubkey_file_name);
                    if (!std::cin)
                    {
                        std::cerr << "Error during input\n";
                        exit(1);
                    }

                    /* Load peer public key from a file */
                    FILE *p2r = fopen(peer_pubkey_file_name.c_str(), "r");
                    if (!p2r)
                    {
                        std::cerr << "Error: cannot open file '" << peer_pubkey_file_name << "' (missing?)\n";
                        exit(1);
                    }
                    EVP_PKEY *peer_pubkey = PEM_read_PUBKEY(p2r, NULL, NULL, NULL);
                    fclose(p2r);
                    if (!peer_pubkey)
                    {
                        std::cerr << "Error: PEM_read_PUBKEY returned NULL\n";
                        exit(1);
                    }

                    derive_ctx = EVP_PKEY_CTX_new(my_dhkey, NULL);
                    if (!derive_ctx)
                        std::cerr << "EVP_PKEY_CTX_new() failed!" << std::endl;

                    if (EVP_PKEY_derive_init(derive_ctx) <= 0)
                        std::cerr << "EVP_PKEY_derive_init() failed!" << std::endl;

                    /* Setting the peer with its pubkey */
                    if (EVP_PKEY_derive_set_peer(derive_ctx, peer_pubkey) <= 0)
                        std::cerr << "EVP_PKEY_derive_set_peer() failed!" << std::endl;

                    /* Determine buffer length, by performing a derivation but writing the result nowhere */
                    EVP_PKEY_derive(derive_ctx, NULL, &skeylen);

                    /* allocate buffer for the shared secret */
                    skey = (unsigned char *)(malloc(int(skeylen)));
                    if (!skey)
                        std::cerr << "malloc() failed!" << std::endl;

                    /*Perform again the derivation and store it in skey buffer*/
                    if (EVP_PKEY_derive(derive_ctx, skey, &skeylen) <= 0)
                        std::cerr << "EVP_PKEY_derive() failed!" << std::endl;

                    printf("Here it is the shared secret: \n");
                    BIO_dump_fp(stdout, (const char *)skey, skeylen);

                    /* SECTION */

                    /* Receive Client DH Key */
                    
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