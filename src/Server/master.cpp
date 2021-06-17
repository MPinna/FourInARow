/**
 * SECTION
 * The server class is responsible to manage all server functionalities:
 *  - Process Client authentication requests
 *  - Forward Client challenge
 *  - Send information about security DH parameter to the peers
 *  - Maintain the online player list
 *  - Maintain the public key list associated to each registered user
 */
#include "../../include/Connection/network.hpp"
#include "../../include/Messages/packet.hpp"
#include "../../include/Server/master.hpp"

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
        std::cerr << "Master::InitMaster failed(1)!" << std::endl;
        return -1;
    }
    ret = SetSockOpt(this->_masterfd, level, optname, &optval);
    if(ret < 0)
    {
        std::cerr << "Master::InitMaster failed(2)!" << std::endl;
        return -1;
    }
    ret = SockBind(this->_masterfd, this->_ipserveraddr, _portno, family, this->_masteraddr);
    if(ret < 0)
    {
        std::cerr << "Master::InitMaster failed(3)!" << std::endl;
        return -1;
    }
    ret = SockListen(this->_masterfd, backlog_queue);
    if(ret < 0)
    {
        std::cerr << "Master::InitMaster failed(4)!" << std::endl;
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
    // TODO hai bisogno di capire come fare a passare un buff calcolato ogni volta
    std::string wlc_msg{"Connected with => " + this->_ipserveraddr + ":" + this->_portno};
    int nbytes{-1}, _ret{-1};
    unsigned char buf[256];
    size_t buf_size{sizeof(buf)};

    for (;;)
    {
        this->_read_fds = this->_master_set; // copy it
        if (select(this->_fdmax + 1, &this->_read_fds, NULL, NULL, NULL) == -1)
        {
            std::cerr << "Master::Run() failed(1)" << std::endl;
            return -1;
        }

        // run through the exisiting connections looking for data to read
        for (size_t i = 0; i <= this->_fdmax; i++)
        {
            if (FD_ISSET(i, &this->_read_fds))
            {
                if (i == this->_masterfd)
                {   // New client connected to the server
                    this->_addrlen = sizeof(this->_peeraddr); 
                    if ((this->_receivefd = SockAccept(this->_masterfd, (sockaddr *)&this->_peeraddr, &this->_addrlen)) == -1)
                        std::cerr << "Master::Run() failed(2)" << std::endl;
                    else
                    {
                        FD_SET(this->_receivefd, &this->_master_set);
                        if (this->_receivefd > this->_fdmax)
                            this->_fdmax = this->_receivefd;
                    }
                    std::cout << "\nNew connection from " << inet_ntoa(this->_peeraddr.sin_addr) << " on socket: " << this->_receivefd << std::endl;
                }
                else // We are going to manage communication
                {   // SECTION Receive packet's header
                    unsigned char *header = new unsigned char[sizeof(struct Header) + 1];
                    Packet *packet = new Packet();
                    memset(header, 0, sizeof(header));
                    header[9] = '\0';
                    nbytes = ReadNBytes(i, header, sizeof(struct Header));
                    if (nbytes <= 0)
                    {
                        if (nbytes == 0)
                            std::cout << " Master::Run() -> Peer: " << i << " disconnected" << std::endl;
                        else
                            std::cout << "Master::Run() failed(3)" << std::endl;
                        
                        _ret = SockClose(i); // bye!
                        if(_ret < 0)
                            std::cout << "Master::Run() failed(4)" << std::endl;
                        FD_CLR(i, &this->_master_set);
                        
                        continue;
                    }
                    else
                        packet->deserializeHeader(header);

                    std::cout << 
                        "\npacket->getType() "<< packet->getType() <<
                        "\npacket->getCounter() "<< packet->getCounter() <<
                        "\npacket->getPayloadSize() "<< packet->getPayloadSize() <<
                    std::endl;

                    // SECTION Ready to receive Payload
                    unsigned char *payload = new unsigned char[packet->getPayloadSize() + 1];
                    memset(payload, 0, sizeof(payload));
                    payload[packet->getPayloadSize() + 1] = '\0';
                    nbytes = ReadNBytes(i, payload, packet->getPayloadSize());
                    if (nbytes <= 0)
                    {
                        if (nbytes == 0)
                            std::cout << " Master::Run() -> Peer: " << i << " disconnected" << std::endl;
                        else
                            std::cout << "Master::Run() failed(5)" << std::endl;
                        
                        _ret = SockClose(i); // bye!
                        if(_ret < 0)
                            std::cout << "Master::Run() failed(6)" << std::endl;
                        FD_CLR(i, &this->_master_set);
                    }
                    else // TOCHECK here open the switch case
                    {
                        std::cout << "Payload: " << payload << std::endl;
                        if (FD_ISSET(i, &this->_master_set))
                            _ret = SockSend(i, (char *)wlc_msg.c_str() + '\0', wlc_msg.length() + 1);
                        if (_ret == -1)
                            std::cerr << "Master::Run() failed(7)" << std::endl;
                    }
                    delete[] header;
                    delete[] payload;
                }
            }
        }
    }

    return 1;
}