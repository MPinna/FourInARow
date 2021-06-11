#include "../../include/Client/slave.hpp"
#include <sys/time.h>

Slave::Slave()
    : _clientfd{0}, _peerfd{0}, _serveraddr{DEFAULT_SERVER_ADDR}, _serverport{DEFAULT_SERVER_PORT}, _serverinfo{NULL}, _peeraddr{DEFAULT_PEER_ADDR}, _peerport{DEFAULT_PEER_PORT}, _peerinfo{NULL}
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
	short int ret{-1};
    this->_clientfd = InitSocket(domain, socktype, protocol);
	if(this->_clientfd < 0)
	{
		std::cerr << "Slave::InitClient(1)" << std::endl;
		return _clientfd;
	}
    this->_serverinfo = GetAddrInfo(
		this->_serveraddr.c_str(),
        this->_serverport.c_str(), 
        family, 
        socktype);
	if(this->_serverinfo == NULL)
	{
		std::cerr << "Slave::InitClient(2)" << std::endl;
		return -1;
	}
	ret = SockConnect(this->_clientfd, *this->_serverinfo);
	if(ret < 0)
	{
		std::cerr << "Slave::InitClient(3)" << std::endl;
		return ret;
	}
	
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
	short int ret{-1};

	this->_peerfd = InitSocket(domain, socktype, protocol);
	if(this->_peerfd < 0)
	{	
		std::cerr << "Slave::InitPeerReceiver(1)" << std::endl;
		return 0;
	}
	ret = SetSockOpt(this->_peerfd, level, optname, &optval);
	if(ret < 0)
	{
		std::cerr << "Slave::InitPeerReceiver(2)" << std::endl;
		return 0;
	}
	ret = SockBind(this->_peerfd, this->_peeraddr, this->_peerport, family, _peersock);
	if(ret < 0)
	{
		std::cerr << "Slave::InitPeerReceiver(3)" << std::endl;
		return 0;
	}
	// TOCHECK: remove this print?
	std::cout << "READY! Waiting for a client to connect..." << std::endl;
	ret = SockListen(this->_peerfd, backlog_queue);
	if(ret < 0)
	{
		std::cerr << "Slave::InitPeerReceiver(4)" << std::endl;
		return 0;
	}
	int _acceptfd = SockAccept(this->_peerfd, _peersock);
	if(_acceptfd < 0)
	{
		std::cerr << "Slave::InitPeerReceiver(5)" << std::endl;
		return 0;
	}
	// TOCHECK could be useful to have getnameinfo here to keep track of received messages ?
	std::cout << "Connected with peer!" << std::endl;
	//lets keep track of the session time
	struct timeval start1, end1;
	gettimeofday(&start1, NULL);
	//also keep track of the amount of data sent as well
	int bytesRead, bytesWritten = 0;
	char msg[1500];
	while (1)
	{
		//receive a message from the client (listen)
		std::cout << "Awaiting sender response..." << std::endl;
		memset(&msg, 0, sizeof(msg)); //clear the buffer
		bytesRead += SockReceive(_acceptfd, (char *)&msg, sizeof(msg));
		if (!strcmp(msg, "exit"))
		{
			std::cout << "Client has quit the session" << std::endl;
			break;
		}
		std::cout << "Sender: " << msg << std::endl;
		std::cout << ">";
		std::string data;
		getline(std::cin, data);
		memset(&msg, 0, sizeof(msg)); //clear the buffer
		strcpy(msg, data.c_str());
		if (data == "exit")
		{
			//send to the client that server has closed the connection
			SockSend(_acceptfd, (char *)&msg, strlen(msg));
			break;
		}
		//send the message to client
		bytesWritten += SockSend(_acceptfd, (unsigned char *)&msg, strlen(msg));
	}
	//we need to close the socket descriptors after we're all done
	gettimeofday(&end1, NULL);
	close(this->_peerfd);
	close(_acceptfd);
	std::cout << "********Session********" << std::endl;
	std::cout << "Bytes written: " << bytesWritten << " Bytes read: " << bytesRead << std::endl;
	std::cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec)
		 << " secs" << std::endl;
	std::cout << "Connection closed..." << std::endl;
	
	return 1;
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
	short int ret{-1};

	this->_peerfd = InitSocket(domain, socktype, protocol);
	if(this->_peerfd < 0) 
	{
		std::cerr << "Slave::InitPeerSender(1)" << std::endl;
		return ret;
	}
	this->_peerinfo = GetAddrInfo(this->_peeraddr.c_str(), 
		this->_peerport.c_str(), family, socktype);
	if(this->_peerinfo == NULL)
	{
		std::cerr << "Slave::InitPeerSender(2)" << std::endl;
		return ret;
	}
	ret = SockConnect(this->_peerfd, *this->_peerinfo);
	if(ret < 0)
	{
		std::cerr << "Slave::InitPeerSender(3)" << std::endl;
		return ret;
	}
	std::cout << "Connected to the peer!" << std::endl;
    int bytesRead, bytesWritten = 0;
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);
	char msg[1500];
    while (1)
    {
        std::cout << ">";
        std::string data;
        getline(std::cin, data);
        memset(&msg, 0, sizeof(msg)); //clear the buffer
        strcpy(msg, data.c_str());
        if (data == "exit")
        {
            send(this->_peerfd, (unsigned char *)&msg, strlen(msg), 0);
            break;
        }
        bytesWritten += send(this->_peerfd, (char *)&msg, strlen(msg), 0);
        std::cout << "Awaiting receiver response..." << std::endl;
        memset(&msg, 0, sizeof(msg)); //clear the buffer
        bytesRead += recv(this->_peerfd, (char *)&msg, sizeof(msg), 0);
        if (!strcmp(msg, "exit"))
        {
            std::cout << "Server has quit the session" << std::endl;
            break;
        }
        std::cout << "Receiver: " << msg << std::endl;
    }
    gettimeofday(&end1, NULL);
    close(this->_peerfd);
    std::cout << "******** Session ********" << std::endl;
    std::cout << "Bytes written: " << bytesWritten << " Bytes read: " << bytesRead << std::endl;
    std::cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec)
         << " secs" << std::endl;
    std::cout << "Connection closed" << std::endl;

	return 1;
}