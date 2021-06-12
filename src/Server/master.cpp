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
#include "../../include/Server/master.hpp"

Master::Master()
    : _ipserveraddr{DEFAULT_SERVER_ADDR}, _portno{DEFAULT_SERVER_PORT}, _masterfd{-1}, _receivefd{-1}, _fdmax{-1}, _read_fds{0}, _master_set{0}, _addrlen{NULL}
{
}

Master::Master(std::string port)
    : _ipserveraddr{DEFAULT_SERVER_ADDR}, _portno{port}, _masterfd{-1}, _receivefd{-1}
{
}

Master::Master(std::string ipaddr, std::string port)
    : _ipserveraddr{ipaddr}, _portno{port}, _masterfd{-1}, _receivefd{-1}
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
    unsigned char buf[256];
    size_t buf_size{sizeof(buf)};
    int nbytes{-1};
    for (;;)
    {
        this->_read_fds = this->_master_set; // copy it
        if (select(this->_fdmax + 1, &this->_read_fds, NULL, NULL, NULL) == -1)
        {
            std::cerr << "select() failed" << std::endl;
            return -1;
        }

        // run through the exisiting connections looking for data to read
        for (size_t i = 0; i <= this->_fdmax; i++)
        {
            if (FD_ISSET(i, &this->_read_fds))
            {
                if (i == this->_masterfd)
                {
                    this->_addrlen = sizeof(this->_peeraddr);
                    if ((this->_receivefd = accept(this->_masterfd, (sockaddr *)&this->_peeraddr, &this->_addrlen)) == -1)
                        std::cerr << "accept() failed!" << std::endl;
                    else
                    {
                        FD_SET(this->_receivefd, &this->_master_set);
                        if (this->_receivefd > this->_fdmax)
                            this->_fdmax = this->_receivefd;
                    }
                    std::cout << "\nselectserver: new connection from " << inet_ntoa(this->_peeraddr.sin_addr) << " on socket " << this->_receivefd << std::endl;
                }
                else
                {
                    memset(&buf, 0, buf_size); //TOCHECK clear the buffer
                    if ((nbytes = recv(i, buf, buf_size, 0)) <= 0)
                    {
                        if (nbytes == 0)
                            std::cout << "selectserver: socket " << i << " hung up\n" << std::endl;
                        else
                            std::cerr << "recv() failed!" << std::endl;
                        close(i); // bye!
                        FD_CLR(i, &this->_master_set);
                    }
                    else
                    {
                        if(FD_ISSET(i, &this->_master_set))
                            if (send(i, "Hi client!", strlen("Hi client!"), 0) == -1)
                                std::cerr << "send() failed!" << std::endl;
                            else
                                std::cout << "\nReceived: " << buf << " from sock " << i <<std::endl;
                                std::cout << "response sent!" << std::endl;
                    }
                }
            }
        }
    }

    return 1;
}