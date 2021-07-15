/** DESCRIPTION
 * This file it used to instantiate a server instance capable to:
 *  - Accept a player
 *  - List online players
 *  - Make possible to run challenge between two 
*/
#include "../../libs/OpensslDS/include/digitalSignature.hpp"
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
     * TODO 
     * method to initialize all openssl variable, keys, etc...
    */


    /**
     * TODO
     * 
    */
    std::string wlc_msg{"Connected with: " + server->getIPAddr() + ":" + server->getPort()};
    unsigned char rcv_msg[] = "Message Received";
    unsigned char cls_msg[] = "Close signal received";
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
                        "\n==> New connection from " << 
                        inet_ntoa(server->_peeraddr.sin_addr) << 
                        " on socket: " << server->_receivefd << 
                    std::endl;
                    clients.at(server->_receivefd)._status = 1;
                    clients.at(server->_receivefd).packet.reallocPayload((unsigned char *)wlc_msg.c_str());
                    ret = PacketSend(server->_receivefd, &clients.at(server->_receivefd).packet);
                    if(ret == -1)
                        std::cerr << " <== server()::response(): wlc_msg not sent";
                    // Start authentication phase
                    continue;
                }
                // SECTION We are going to manage communication
                else 
                {   
                    nbytes = PacketReceive(i, &clients.at(i).packet, 0);
                    if(nbytes > 0)
                    {   
                        clients.at(i).packet.print();
                        clients.at(i).packet.reallocPayload(rcv_msg);
                        ret = PacketSend(i, &clients.at(i).packet);
                        if(ret < 0)
                            std::cerr << " <== server()::response(): response msg not sent";
                            continue;
                    }
                    else if(nbytes == 0)
                    {   
		                std::cout << "Peer: " << i << " disconnected" << std::endl;
                        clients.at(i).packet.reallocPayload(cls_msg);
                        ret = PacketSend(i, &clients.at(i).packet);
                        if(ret < 0)
                            std::cerr << " <== server()::response(): close msg not sent";
                        ret = SockClose(i);
                        if(ret < 0)
		                    std::cerr << " <== server()::receive(sockClose)";
                        FD_CLR(i, &server->_master_set);
                        clients.at(i).username.clear();
                        clients.at(i)._status = false;
                        clients.at(i).packet.reset();
                        continue;
                    }
                    else
                    {
                        std::cerr << " <== server()::receive(): unexpected error on receive";
                        ret = SockClose(i);
                        FD_CLR(i, &server->_master_set);
                        clients.at(i).username.clear();
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