/**
 * DESCRIPTION
 * Functions implementation of the network.hpp header
 */
#include "../../include/Connection/network.hpp"
#include "../../include/Messages/packet.hpp"
#include "../../include/Utils/constant.hpp"

int 
InitSocket(int domain, int socktype, int protocol)
{
	// 0 on success, -1 on failure
	int _sockfd = socket(domain, socktype, protocol);
	if (_sockfd < 0)
		std::cerr << "InitSocket()::socket failed! =>";
	return _sockfd;
}

int
SetSockOpt(int sockfd, int level, int optname, void *optval)
{
	short int ret{-1};
	unsigned int len = sizeof(optval);
	ret = setsockopt(sockfd, level, optname, &optval, len);
	if (ret < 0)
		std::cerr << "SetSockOpt()::setsockopt failed! =>";
	return ret;
}

int 
SockBind(int sockfd, std::string ipaddr, std::string port, int family, struct sockaddr_in sockaddress)
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
		std::cerr << "SockBind()::bind failed! =>";
	return ret;
}

int 
SockListen(int sockfd, int max_queue)
{
	short int ret{-1};
	ret = listen(sockfd, max_queue);
	if (ret < 0)
		std::cerr << "SockListen()::listen failed!" << gai_strerror(errno) << std::endl;
	return ret;
}

int
SockAccept(int sockfd, sockaddr *sockaddress, socklen_t *addrlen)
{
	int _newsockfd{0};
	if ((_newsockfd = accept(sockfd, sockaddress, addrlen)) < 0)
		std::cerr << "SockAccept()::accept failed! =>";

	return _newsockfd;
}

struct addrinfo *
GetAddrInfo(const char *node, const char *port, int family, int socktype)
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
		return NULL;
	}

	return result;
}

int SockConnect(int sockfd, struct addrinfo info)
{
	int status = connect(sockfd, info.ai_addr, info.ai_addrlen);
	if (status < 0)
		std::cerr << "SockConnect()::connect failed! =>";

	return status;
}

int 
SockSelect(int sockfd, fd_set read_fds)
{	
	int ret{-1};
	ret = select(sockfd + 1, &read_fds, NULL, NULL, NULL);
	if (ret == -1)
		std::cerr << "select() failed! =>";
	
	return ret;
}

int 
SockReceive(int rec_sockfd, void *rec_buf, size_t len)
{
	short int ret{0};

	// NOTA: by default socket are set to block
	ret = recv(rec_sockfd, (char *)rec_buf, len, 0);
	if (ret == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			std::cerr << ETIMEDOUT << std::endl;
		else
			std::cerr << "SockReceive()::recv failed! =>";

		return ret;
	}
	if (ret == 0)
	{
		std::cout << "SockReceive()::recv connection closed!";
		return ret;
	}

	return ret; // TOCHECK
}

int 
SockSend(int send_sockfd, void *send_buf, size_t len)
{
	short int ret{0};

	ret = send(send_sockfd, (char *)send_buf, len, 0);
	if (ret == -1)
	{
		std::cerr << "SockSend()::sendto - failed! =>";
		return ret;
	}

	return ret; // TOCHECK
}

int 
SockReceiveFrom(int rec_sockfd, void *rec_buf, size_t len)
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
				std::cerr << "SockReceive()::recvfrom failed! =>";

			return ret;
		}
		if (ret == 0)
		{
			std::cerr << "SockReceive()::recvfrom connection closed! =>";
			return ret;
		}
		read += ret;
	}
	return read; 
}

int 
SockSendTo(int send_sockfd, void *send_buf, size_t len)
{
	size_t sent{0}, ret{0};
	while (sent < len)
	{
		ret = sendto(send_sockfd, (char *)send_buf, len, 0, NULL, 0);
		if (ret == -1)
		{
			std::cerr << "SockSend()::sendto - failed! =>";
			return ret;
		}
		sent += ret;
	}
	return sent; 
}

int 
ReadNBytes(int socket, void *buf, std::size_t N)
{
	std::size_t offset = 0;
	while (true)
	{
		ssize_t ret = recvfrom(socket, (char *)buf + offset, N - offset, MSG_WAITALL, NULL, 0);
		if (ret < 0)
		{
			if (errno != EINTR)
			{
				// Error occurred
				std::cerr << "ReadNBytes::IOException(strerror(errno)) " << ret << " => "; 
				return -1;
			}
		}
		else if (ret == 0)
		{
			// No data available anymore
			if (offset == 0)
			{
				std::cout << "ReadNBytes::No Data! => ";
				return 0;
			}
			else
			{
				std::cerr << "ReadNBytes::ProtocolException (Unexpected end of stream) => ";
				return -1;
			}
		}
		else if (offset + ret == N)
		{
			// All n bytes read
			return N;
		}
		else
		{
			offset += ret;
		}
	}
}

int 
SockClose(int sockfd)
{
	short int ret{-1};
	ret = close(sockfd);
	if(ret < 0)
		std::cerr << "SockClose()::close - failed! =>";
	
	return ret;
}