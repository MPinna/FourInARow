#include "Connection/network.hpp"
#include "Utils/constant.hpp"
#include "Client/slave.hpp"
#include <netdb.h>

Slave::Slave()
    : _clientfd{0}, _peerfd{0}, _serveraddr{DEFAULT_SERVER_ADDR}, _serverport{DEFAULT_SERVER_PORT}, _serverinfo{NULL}, _peeraddr{DEFAULT_PEER_ADDR}, _peerport{0}, _peerinfo{NULL}
{
}

Slave::Slave(std::string peerport)
    : _clientfd{0}, _peerfd{0}, _serveraddr{DEFAULT_SERVER_ADDR}, _serverport{DEFAULT_SERVER_PORT}, _serverinfo{NULL}, _peeraddr{DEFAULT_PEER_ADDR}, _peerport{peerport}, _peerinfo{NULL}
{
}

Slave::Slave(std::string peerport, std::string peeraddr)
	: _clientfd{0}, _peerfd{0}, _serveraddr{DEFAULT_SERVER_ADDR}, _serverport{DEFAULT_SERVER_PORT}, _serverinfo{NULL}, _peeraddr{peeraddr}, _peerport{peerport}, _peerinfo{NULL}
{
}

Slave::Slave(std::string serverport)
	: _clientfd{0}, _peerfd{0}, _serveraddr{DEFAULT_SERVER_ADDR}, _serverport{serverport}, _serverinfo{NULL}, _peeraddr{DEFAULT_PEER_ADDR}, _peerport{0}, _peerinfo{NULL}
{
}

Slave::Slave(std::string serverport, std::string serveraddr)
	: _clientfd{0}, _peerfd{0}, _serveraddr{serveraddr}, _serverport{serverport}, _serverinfo{NULL}, _peeraddr{DEFAULT_PEER_ADDR}, _peerport{0}, _peerinfo{NULL}
{
}

int
Slave::InitClient()
{
    this->_clientfd = InitSocket(AF_INET, SOCK_STREAM, 0);
    _serverinfo = GetAddrInfo(this->_serveraddr.c_str(),
                                    this->_serverport.c_str(), 
                                    AF_INET, 
                                    SOCK_STREAM);
	SockConnect(this->_clientfd, *this->_serverinfo);
}

int
Slave::InitPeerReceiver()
{
	// TODO

	return 0;
}

int
Slave::InitPeerSender()
{
	// TODO

	return 0;
}