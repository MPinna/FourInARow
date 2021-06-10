/**
 * DESCRIPTION
 * Functions implementation of the network.hpp header
 */
#include "../../include/Connection/network.hpp"
#include "../../include/Utils/constant.hpp"

int 
InitSocket(int domain, int socktype, int protocol)
{
	int _sockfd = socket(domain, socktype, protocol);
	if (_sockfd < 0)
		std::cerr << "InitSocket() - socket - failed!" << std::endl;
	return _sockfd;
}

int
SetSockOpt(int sockfd, int level, int optname, void *optval)
{
	short int ret{-1};
	unsigned int len = sizeof(optval);
	ret = setsockopt(sockfd, level, optname, &optval, len);
	if (ret < 0)
		perror("SetSockOpt() - setsockopt - failed!");
	return ret;
}

int SockBind(
	int sockfd,
	std::string ipaddr,
	std::string port,
	int family,
	struct sockaddr_in sockaddress)
{
	short int ret{-1};
	uint32_t _port = atoi(port.c_str());
	uint32_t _addr = atoi(ipaddr.c_str());
	memset(&sockaddress, 0, sizeof(struct sockaddr_in));
	sockaddress.sin_family = family;
	sockaddress.sin_addr.s_addr = htonl(_addr);
	sockaddress.sin_port = htons(_port);
	ret = bind(sockfd, (const struct sockaddr *)&sockaddress, sizeof(sockaddr));
	if (ret < 0)
		std::cerr << "SockBind() - bind - failed!" << std::endl;
	return ret;
}

int 
SockListen(int sockfd, int max_queue)
{
	short int ret{-1};
	ret = listen(sockfd, max_queue);
	if (ret < 0)
		std::cerr << "SockListen() - listen - failed!" << gai_strerror(errno) << std::endl;
	return ret;
}

int
SockAccept(int sockfd, struct sockaddr_in sockaddress)
{
	int addrlen = sizeof(sockaddress), _newsockfd{0};
	if ((_newsockfd = accept(sockfd, (struct sockaddr *)&sockaddress, (socklen_t *)&addrlen)) < 0)
		std::cerr << "SockAccept() - accept - failed!" << std::endl;
	return _newsockfd;
}

struct addrinfo *GetAddrInfo(
	const char *node,
	const char *port,
	int family,
	int socktype)
{
	struct addrinfo hints;
	struct addrinfo *result;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = family;
	hints.ai_socktype = socktype;
	hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICSERV;
	hints.ai_protocol = 0;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	int ret = getaddrinfo(node, port, &hints, &result);
	if (ret != 0)
	{
		std::cerr << gai_strerror(ret) << std::endl;
		exit(1);
	}

	return result;
}

int SockConnect(int sockfd, struct addrinfo info)
{
	int status = connect(sockfd, info.ai_addr, info.ai_addrlen);
	if (status < 0)
		std::cerr << "SockConnect() - connect - failed!" << std::endl;
	return status;
}

void SockSelect()
{
	// TODO
}

bool SockReceive(int rec_sockfd, void *rec_buf, size_t len)
{
	size_t read{0};
	ssize_t ret{0};
	while (read < len)
	{
		// NOTA: by default socket are set to block
		ret = recvfrom(rec_sockfd, (char *)rec_buf, len, 0, NULL, 0);
		if (ret == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				std::cerr << ETIMEDOUT << std::endl;
			else
				std::cerr << "SockReceive() - recvfrom - failed!" << std::endl;

			return false;
		}
		if (ret == 0)
		{
			std::cerr << "SockReceive() - recvfrom - connection closed!" << std::endl;
			return false;
		}
		read += ret;
	}
	return true;
}

bool SockSend(int send_sockfd, unsigned char *send_buf, size_t len)
{
	size_t sent{0}, ret{0};
	while (sent < len)
	{
		ret = sendto(send_sockfd, (char *)send_buf, len, 0, NULL, 0);
		if (ret == -1)
		{
			std::cerr << "SockSend() - send - failed!" << std::endl;
			return false;
		}
		sent += ret;
	}
	return true;
}

int 
SockClose(int sockfd)
{
	short int ret{-1};
	ret = close(sockfd);
	if(ret < 0)
		std::cerr << "SockClose() - close - failed!" << std::endl;
	return ret;
}

bool ReadNBytes(int socket, char *buf, std::size_t N)
{
	std::size_t offset = 0;
	while (true)
	{
		ssize_t ret = recvfrom(socket, buf + offset, N - offset, MSG_WAITALL, NULL, 0);
		if (ret < 0)
		{
			if (errno != EINTR)
			{
				// Error occurred
				std::cerr << "IOException(strerror(errno)" << std::endl;
				return false;
			}
		}
		else if (ret == 0)
		{
			// No data available anymore
			if (offset == 0)
			{
				std::cerr << "No Data!" << std::endl;
				return false;
			}
			else
			{
				std::cerr << "ProtocolException (Unexpected end of stream)" << std::endl;
				return false;
			}
		}
		else if (offset + ret == N)
		{
			// All n bytes read
			return true;
		}
		else
		{
			offset += ret;
		}
	}
}