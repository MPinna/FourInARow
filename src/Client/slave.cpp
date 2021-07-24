#include "../../include/Client/slave.hpp"
#include <sys/time.h>

Slave::Slave(std::string username)
    : _serverinfo{NULL}, _peerinfo{NULL}, _serveraddr{DEFAULT_SERVER_ADDR}, _serverport{DEFAULT_SERVER_PORT},  _peeraddr{DEFAULT_PEER_ADDR}, _peerport{DFLT_OPP_LISTENER_PORT}, _peersock{0}, _clientfd{-1}, _peerfd{-1}, _username{username}
{
}

Slave::~Slave()
{
}

int
Slave::InitSlave(int domain, int socktype, int protocol, int family)
{
	short int ret{-1};
    this->_clientfd = InitSocket(domain, socktype, protocol);
	if(this->_clientfd < 0)
	{
		std::cerr << " <== Slave::InitSlave(initSocket)"; 
		return _clientfd;
	}
    
	this->_serverinfo = GetAddrInfo(
		this->_serveraddr.c_str(),
        this->_serverport.c_str(), 
        family, 
        socktype);
	if(this->_serverinfo == NULL)
	{
		std::cerr << " <== Slave::InitSlave(getAddrInfo)";
		return -1;
	}
	
	ret = SockConnect(this->_clientfd, *this->_serverinfo);
	if(ret < 0)
	{
		std::cerr << " <== Slave::InitSlave(sockConnect)";
		return ret;
	}
	
    socklen_t len = sizeof(_sock);
    if (getsockname(this->GetClientfd(), (struct sockaddr *)&_sock, &len) == -1)
        std::cerr << "Slave() getsockname error";
    else
        this->_port = ntohs(_sock.sin_port);

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
    int backlog_queue
)
{
	int ret{-1};
	// TODO implement the choice between default send/receive port or customized one (if and only if you have time)

	this->_peerfd = InitSocket(domain, socktype, protocol);
	if(this->_peerfd < 0)
	{	
		std::cerr << " <== Slave::InitPeerReceiver(initSocket)";
		return 0;
	}
	ret = SetSockOpt(this->_peerfd, level, optname, &optval);
	if(ret < 0)
	{
		std::cerr << " <== Slave::InitPeerReceiver(SetSockOpt)";
		return 0;
	}
	ret = SockBind(this->_peerfd, this->_peeraddr, this->_peerport, family, this->_peersock);
	if(ret < 0)
	{
		std::cerr << " <== Slave::InitPeerReceiver(SockBind)";
		return 0;
	}
	// TOCHECK: remove this print?
	std::cout << "READY!\nWaiting for a client to connect..." << std::endl;
	ret = SockListen(this->_peerfd, backlog_queue);
	if(ret < 0)
	{
		std::cerr << " <== Slave::InitPeerReceiver(sockListen)";
		return 0;
	}
	socklen_t _addrlen = sizeof(this->_peersock);
	int _acceptfd = SockAccept(this->_peerfd, (sockaddr *)&this->_peersock, &_addrlen);
	if(_acceptfd < 0)
	{
		std::cerr << " <== Slave::InitPeerReceiver(SockAccept)";
		return 0;
	}
	// TOCHECK could be useful to have getnameinfo here to keep track of received messages ?
	std::cout << "Connected with peer!" << std::endl;
	//lets keep track of the session time
	struct timeval start1, end1;
	gettimeofday(&start1, NULL);
	//also keep track of the amount of data sent as well
	size_t bytesRead{0}, bytesWritten{0};
	int ret_code{-1};
	char msg[1500];
	while (1)
	{
		//receive a message from the client (listen)
		std::cout << "Awaiting sender response..." << std::endl;
		memset(&msg, 0, sizeof(msg)); //clear the buffer
		ret_code = SockReceive(_acceptfd, (char *)&msg, sizeof(msg));
		bytesRead += ret_code;
		if (ret_code == 0)
		{
			std::cerr << "Sender has quit inadvertently!" << std::endl;
			break;
		}
		else if (!strcmp(msg, "exit"))
		{
			std::cout << "Sender has quit the session" << std::endl;
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
	std::cout << "\n********Session********" << std::endl;
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
    int family
)
{
	int ret{-1};
	// TODO implement the choice between default send/receive port or customized one (if and only if you have time)
	this->_peerfd = InitSocket(domain, socktype, protocol);
	if(this->_peerfd < 0) 
	{
		std::cerr << " <== Slave::InitPeerSender(initSocket)";
		return ret;
	}
	this->_peerinfo = GetAddrInfo(this->_peeraddr.c_str(), 
		this->_peerport.c_str(), family, socktype);
	if(this->_peerinfo == NULL)
	{
		std::cerr << " <== Slave::InitPeerSender(getAddrInfo)";
		return ret;
	}
	ret = SockConnect(this->_peerfd, *this->_peerinfo);
	if(ret < 0)
	{
		std::cerr << " <== Slave::InitPeerSender(SockConnect)";
		return ret;
	}
	std::cout << "Connected to the peer!" << std::endl;
    size_t bytesRead{0}, bytesWritten{0};
	int ret_code{-1};
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
        bytesWritten += SockSend(this->_peerfd, (char *)&msg, strlen(msg));
        std::cout << "Awaiting receiver response..." << std::endl;
        memset(&msg, 0, sizeof(msg)); //clear the buffer
        ret_code = SockReceive(this->_peerfd, (char *)&msg, sizeof(msg));
		bytesRead += ret_code;
		if (ret_code == 0)
		{
			std::cerr << " <- Receiver has quit inadvertently!";
			break;
		}
        else if(!strcmp(msg, "exit"))
        {
            std::cout << " <- Server has quit the session";
            break;
        }
        std::cout << "Receiver: " << msg << std::endl;
    }
    gettimeofday(&end1, NULL);
    close(this->_peerfd);
    std::cout << "\n******** Session ********" << std::endl;
    std::cout << "Bytes written: " << bytesWritten << " Bytes read: " << bytesRead << std::endl;
    std::cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec)
         << " secs" << std::endl;
    std::cout << "Connection closed" << std::endl;

	return 1;
}