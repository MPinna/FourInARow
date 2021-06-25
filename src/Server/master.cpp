/**
 * SECTION
 * The server class is responsible to manage all server functionalities:
 *  - Process Client authentication requests
 *  - Forward Client challenge
 *  - Send information about security DH parameter to the peers
 *  - Maintain the online player list
 *  - Maintain the public key list associated to each registered user
 */
#include "../../libs/OpensslDS/include/digitalSignature.hpp"
#include "../../include/Connection/network.hpp"
#include "../../include/Messages/packet.hpp"
#include "../../include/Server/master.hpp"
#include <vector>

struct ClientInfo
{
    unsigned short int  _status;
    EVP_PKEY            *client_pubkey;
    Packet              packet;
    
    ClientInfo()
        : _status{0}, client_pubkey{NULL} {}
};

Master::Master()
    : _masteraddr{}, _peeraddr{}, _ipserveraddr{DEFAULT_SERVER_ADDR}, _portno{DEFAULT_SERVER_PORT}, _masterfd{-1}, _receivefd{-1}, _fdmax{-1}, _read_fds{0}, _master_set{0}, _addrlen{sizeof(_peeraddr)}
{
}

Master::~Master()
{
}

int
Master::InitMaster(int domain, int socktype, int protocol, int family, int level, int optname, int optval, int backlog_queue)
{   
    short int ret{-1};
    this->_masterfd = InitSocket(domain, socktype, protocol);
    if(this->_masterfd < 0)
    {
        std::cerr << " <== Master::InitMaster(initSocket)";
        return -1;
    }
    ret = SetSockOpt(this->_masterfd, level, optname, &optval);
    if(ret < 0)
    {
        std::cerr << " <== Master::InitMaster(setSockOpt)";
        return -1;
    }
    ret = SockBind(this->_masterfd, this->_ipserveraddr, _portno, family, this->_masteraddr);
    if(ret < 0)
    {
        std::cerr << " <== Master::InitMaster(SockBind)";
        return -1;
    }
    ret = SockListen(this->_masterfd, backlog_queue);
    if(ret < 0)
    {
        std::cerr << " <== Master::InitMaster(SockListen)";
        return -1;
    }

    FD_ZERO(&this->_master_set);
    FD_ZERO(&this->_read_fds);

    // add the listenerfd to the master set
    FD_SET(this->_masterfd, &this->_master_set);

    this->_fdmax = this->_masterfd;

    return this->_masterfd;
}

int
Master::Run()
{
    Packet *server_pck = new Packet();
    std::string wlc_msg{"Connected with => " + this->_ipserveraddr + ":" + this->_portno};
    unsigned char rcv_msg[] = "Message Received!";
    unsigned char cls_msg[] = "Close signal received!";
    int nbytes{-1}, _ret_code{-1};
    std::vector<ClientInfo> clients(this->_fdmax);
    for (;;)
    {
        this->_read_fds = this->_master_set; // copy it
        if (select(this->_fdmax + 1, &this->_read_fds, NULL, NULL, NULL) == -1)
        {
            std::cerr << " <== Master::Run(select)";
            return -1;
        }

        // run through the exisiting connections looking for data to read
        for (size_t i = 0; i <= this->_fdmax; i++)
        {
            if (FD_ISSET(i, &this->_read_fds))
            {
                // SECTION New client connected to the server
                if (i == this->_masterfd) 
                {   
                    this->_addrlen = sizeof(this->_peeraddr); 
                    this->_receivefd = SockAccept(this->_masterfd, (sockaddr *)&this->_peeraddr, &this->_addrlen);
                    if(this->_receivefd == -1)
                        std::cerr << " <== Master::Run(sockAccept)";
                    else
                    {
                        FD_SET(this->_receivefd, &this->_master_set);
                        if (this->_receivefd > this->_fdmax)
                            this->_fdmax = this->_receivefd;
                        clients.resize(this->_receivefd+1);
                    }
                    std::cout << 
                        "\n==> New connection from " << inet_ntoa(this->_peeraddr.sin_addr) << 
                        " on socket: " << this->_receivefd << 
                    std::endl;
                    clients.at(this->_receivefd)._status = 1;
                    clients.at(this->_receivefd).packet.reallocPayload((unsigned char *)wlc_msg.c_str());
                    // packet->reallocPayload((unsigned char *)wlc_msg.c_str());
                    _ret_code = PacketSend(this->_receivefd, &clients.at(this->_receivefd).packet);
                    if(_ret_code == -1)
                        std::cerr << " <== Master::Run(): wlc_msg not sent";
                    continue;
                }
                // SECTION We are going to manage communication
                else 
                {   
                    // nbytes = PacketReceive(i, packet, 0);
                    nbytes = PacketReceive(i, &clients.at(i).packet, 0);
                    if(nbytes > 0)
                    {   
                        clients.at(i).packet.print();
                        clients.at(i).packet.reallocPayload(rcv_msg);
                        _ret_code = PacketSend(i, &clients.at(i).packet);
                        if(_ret_code < 0)
                            std::cerr << " <== Master::Run(): response msg not sent";
                            continue;
                    }
                    else if(nbytes == 0)
                    {   
                        clients.at(i).packet.reallocPayload(cls_msg);
                        _ret_code = PacketSend(i, &clients.at(i).packet);
                        if(_ret_code < 0)
                            std::cerr << " <== Master::Run(): close msg not sent";
                        _ret_code = SockClose(i);
                        if(_ret_code < 0)
		                    std::cerr << " <== Master::Run(sockClose)";
                        FD_CLR(i, &this->_master_set);
                        clients.erase(clients.begin() + i);
                        continue;
                    }
                    else
                    {
                        std::cerr << " <== Master::Run(): unexpected error on receive";
                        _ret_code = SockClose(i);
                        FD_CLR(i, &this->_master_set);
                        clients.erase(clients.begin() + i);
                        return -1;
                    }
                }
            }
        }
    }

    return 1;
}