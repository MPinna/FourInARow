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
    : _ipserveraddr{DEFAULT_SERVER_ADDR}, _portno{DEFAULT_SERVER_PORT}, _masterfd{-1}, _receivefd{-1}
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
        std::cerr << "Master::InitMaster failed(1)!" << std::endl;
    ret = SetSockOpt(this->_masterfd, level, optname, &optval);
    if(ret < 0)
        std::cerr << "Master::InitMaster failed(2)!" << std::endl;
    ret = SockBind(this->_masterfd, this->_ipserveraddr, _portno, family, this->_serversock);
    if(ret < 0)
        std::cerr << "Master::InitMaster failed(3)!" << std::endl;
    ret = SockListen(this->_masterfd, backlog_queue);
    if(ret < 0)
        std::cerr << "Master::InitMaster failed(4)!" << std::endl;

    return this->_masterfd;
}