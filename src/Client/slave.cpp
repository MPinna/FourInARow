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

Slave::~Slave()
{
}

int
Slave::InitClient(int domain, int socktype, int protocol, int family)
{
    this->_clientfd = InitSocket(domain, socktype, protocol);
    this->_serverinfo = GetAddrInfo(
		this->_serveraddr.c_str(),
        this->_serverport.c_str(), 
        family, 
        socktype);
	SockConnect(this->_clientfd, *this->_serverinfo);
	return this->_clientfd;
}

int
Slave::InitPeerReceiver(
	int domain, 
    int socktype, 
    int protocol, 
    int family,
	int level,
	int optname,
    int optval,
    int backlog_queue, 
    struct sockaddr_in _peersock
)
{
	this->_peerfd = InitSocket(domain, socktype, protocol);
	SetSockOpt(this->_peerfd, level, optname, &optval);
	SockBind(this->_peerfd, this->_peeraddr, this->_peerport, family, _peersock);
	// TOCHECK: remove this print?
	std::cout << "READY! Waiting for a client to connect..." << std::endl;
	SockListen(this->_peerfd, backlog_queue);
	int _acceptfd = SockAccept(this->_peerfd, _peersock);
	// TOCHECK could be useful to have getnameinfo here to keep track of received messages ?
	std::cout << "Connected with peer!" << std::endl;
	return _acceptfd;
}

int
Slave::InitPeerSender(
	int domain,
    int socktype, 
    int protocol, 
    int family,
    struct sockaddr_in _peersock
)
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