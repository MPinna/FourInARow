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

    /**
     * method to initialize all openssl variable, keys, etc...
    */
    X509* certificate;
    ret = RetrieveCert(&certificate);
    if(ret <= 0)
    {
        if(ret < 0)
            std::cerr << "<== Server Error!" << std::endl;
        if(ret == 0)
            std::cerr << "<== Server exit()" << std::endl;
        exit(1);
    }

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
                    nbytes = ESPPacketReceive(server->_receivefd, &clients.at(server->_receivefd)._packet, &msg); 
                    
                    /* Get message info: content and signature */
                    ClientHello hello = ClientHello();
                    hello.NtoH(clients.at(server->_receivefd)._packet._payload);
                    
                    // Print received info
                    clients.at(server->_receivefd)._packet.print();
                    hello.print();
                    clients.at(server->_receivefd)._packet.printTag();

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
                    const EVP_MD *cipher = EVP_sha512();
                    ret = digestVerify(
                        msg,
                        clients.at(server->_receivefd)._packet.getPacketSize(),
                        clients.at(server->_receivefd)._packet.getTag(),
                        clients.at(server->_receivefd)._packet.getTaglen(),clients.at(server->_receivefd)._pubkey,
                        cipher
                    );
                    if (ret < 0)
                    {
                        std::cerr << argv[0] << "Verify failed!" << std::endl;
                        continue;
                    }
                    
                    /* Send Certificate */

                    /* Send response */

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
                    nbytes = PacketReceive(i, &clients.at(i)._packet, 0);
                    if(nbytes > 0)
                    {   
                        clients.at(i)._packet.print();
                        clients.at(i)._packet.reallocPayload(rcv_msg, rcv_msg_size);
                        ret = PacketSend(i, &clients.at(i)._packet);
                        if(ret < 0)
                            std::cerr << " <== server()::response(): response msg not sent";
                            continue;
                    }
                    else if(nbytes == 0)
                    {   
		                std::cout << "Peer: " << i << " disconnected" << std::endl;
                        clients.at(i)._packet.reallocPayload(cls_msg, cls_msg_size);
                        ret = PacketSend(i, &clients.at(i)._packet);
                        if(ret < 0)
                            std::cerr << " <== server()::response(): close msg not sent";
                        ret = SockClose(i);
                        if(ret < 0)
		                    std::cerr << " <== server()::receive(sockClose)";
                        FD_CLR(i, &server->_master_set);
                        clients.at(i)._username.clear();
                        clients.at(i)._status = false;
                        clients.at(i)._packet.reset();
                        continue;
                    }
                    else
                    {
                        std::cerr << " <== server()::receive(): unexpected error on receive";
                        ret = SockClose(i);
                        FD_CLR(i, &server->_master_set);
                        clients.at(i)._username.clear();
                        clients.at(i)._status = false;
                        clients.at(i)._packet.reset();
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