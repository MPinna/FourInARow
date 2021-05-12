#include "Connection/network.hpp"
#include "Utils/constant.hpp"
#include "Client/slave.hpp"
#include <iostream>
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

Slave::~Slave()
{
}

int
Slave::InitClient(int domain, int socktype, int protocol, int family)
{
    this->_clientfd = InitSocket(domain, socktype, protocol);
    _serverinfo = GetAddrInfo(
		this->_serveraddr.c_str(),
        this->_serverport.c_str(), 
        family, 
        socktype);
	SockConnect(this->_clientfd, *this->_serverinfo);
	return this->_clientfd;
}

int
Slave::InitPeerReceiver(struct sockaddr_in _peersock, int backlog_queue)
{
	int opt{1};
	this->_peerfd = InitSocket(AF_INET, SOCK_STREAM, 0);
	SetSockOpt(this->_peerfd, SOL_SOCKET, SO_REUSEADDR, &opt);
	SockBind(this->_peerfd, this->_peeraddr, this->_peerport, _peersock);
	// FIXME: remove this print
	std::cout << "READY! Waiting for a client to connect..." << std::endl;
	SockListen(this->_peerfd, backlog_queue);
	int _acceptfd = SockAccept(this->_peerfd, _peersock);
	// TOCHECK could be useful to have getnameinfo here to keep track of received messages ?
	std::cout << "Connected with peer!" << std::endl;
	return _acceptfd;
}

int
Slave::InitPeerSender(struct sockaddr_in _peersock_, int domain, int socktype, int protocol, int family)
{
	this->_peerfd = InitSocket(domain, socktype, protocol);
	this->_peerinfo = GetAddrInfo(
		this->_peeraddr.c_str(), 
		this->_peerport.c_str(), 
		family, 
		socktype);
	SockConnect(this->_peerfd, *this->_peerinfo);

	return 0;
}