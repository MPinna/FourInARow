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
		std::cerr << "InitSocket()::socket() failed";
	return _sockfd;
}

int
SetSockOpt(int sockfd, int level, int optname, void *optval)
{
	short int ret{-1};
	unsigned int len = sizeof(optval);
	ret = setsockopt(sockfd, level, optname, &optval, len);
	if (ret < 0)
		std::cerr << "SetSockOpt()::setsockopt() failed";
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
		std::cerr << "SockBind()::bind() failed";
	return ret;
}

int 
SockListen(int sockfd, int max_queue)
{
	short int ret{-1};
	ret = listen(sockfd, max_queue);
	if (ret < 0)
		std::cerr << "SockListen()::listen failed (code: " << gai_strerror(errno) << ")";
	return ret;
}

int
SockAccept(int sockfd, sockaddr *sockaddress, socklen_t *addrlen)
{
	int _newsockfd{0};
	if ((_newsockfd = accept(sockfd, sockaddress, addrlen)) < 0)
		std::cerr << "SockAccept()::accept() failed";

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
		std::cerr << "GetAddrInfo()::getaddrinfo() failed (code: " << gai_strerror(ret) << ")";
		return NULL;
	}

	return result;
}

int SockConnect(int sockfd, struct addrinfo info)
{
	int status = connect(sockfd, info.ai_addr, info.ai_addrlen);
	if (status < 0)
		std::cerr << "SockConnect()::connect() failed";
	return status;
}

int 
SockSelect(int sockfd, fd_set read_fds)
{	
	int ret{-1};
	ret = select(sockfd + 1, &read_fds, NULL, NULL, NULL);
	if (ret == -1)
		std::cerr << "SockSelect::select() failed";
	
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
			std::cerr << "SockReceive()::recv() failed";

		return ret;
	}
	if (ret == 0)
	{
		std::cout << "SockReceive()::recv() connection closed!";
		return ret;
	}

	return ret;
}

int 
SockSend(int send_sockfd, void *send_buf, size_t len)
{
	short int ret{0};

	ret = send(send_sockfd, (char *)send_buf, len, 0);
	if (ret == -1)
	{
		std::cerr << "SockSend()::sendto() failed";
		return ret;
	}

	return ret; 
}

int 
SockReceiveFrom(int rec_sockfd, void *rec_buf, size_t len)
{
	size_t read{0};
	size_t ret{0};
	while (read < len)
	{
		// NOTA: by default socket are set to block
		ret = recvfrom(rec_sockfd, (char *)rec_buf, len, 0, NULL, 0);
		if (ret == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				std::cerr << ETIMEDOUT << std::endl;
			else
				std::cerr << "SockReceiveFrom() failed";

			return ret;
		}
		if (ret == 0)
		{
			std::cerr << "SockReceiveFrom() connection closed";
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
			std::cerr << "SockSendTo() failed";
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
				std::cerr << "ReadNBytes()::recvfrom(): IOException(strerror(errno: " << ret << "))"; 
				return ret;
			}
		}
		else if (ret == 0)
		{
			// No data available anymore
			if (offset == 0)
			{
				std::cout << "ReadNBytes()::recvfrom(): No Data!";
				return 0;
			}
			else
			{
				std::cerr << "ReadNBytes()::recvfrom(): ProtocolException (Unexpected end of stream)";
				return -1;
			}
		}
		else if (offset + ret == N)
			return N; // All n bytes read
		else
			offset += ret;
	}
}

int 
PacketSend(int send_sockfd, Packet *packet)
{
	packet->incCounter();
	int _ret_code{-1};
	size_t _buf_size = sizeof(struct Header) + packet->getPayloadSize();
	unsigned char *_buf = new unsigned char[_buf_size];
	packet->htonPacket(_buf);
	_ret_code = SockSendTo(send_sockfd, _buf, _buf_size);
	if (_ret_code < 0)
	{
		std::cerr << " <== PacketSend()";
		return -1;
	}
	delete[] _buf;
	return 1;
}

int 
ESPPacketSend(int send_sockfd, ESP *packet)
{
	int _ret_code{-1};
	unsigned char *_buf;
	size_t _buf_size = packet->HtoN(&_buf);
	_ret_code = SockSendTo(send_sockfd, _buf, _buf_size);
	if (_ret_code < 0)
	{
		std::cerr << " <== ESPPacketSend()";
		return -1;
	}
	packet->incCounter();
	delete[] _buf;
	return 1;
}

int
PacketReceive(int sockfd, Packet *packet, int type)
{
	int _ret_code{-1};
	unsigned char *header = new unsigned char[sizeof(struct Header) + 1];
    memset(header, 0, sizeof(struct Header) + 1);
    header[sizeof(struct Header) + 1] = '\0';
    int nbytes = ReadNBytes(sockfd, header, sizeof(struct Header));
	// Receive header
    if (nbytes <= 0)
    {
        if (nbytes == 0)
            std::cerr << " <== PacketReceive(): Empty Header!";
        else
            std::cerr << " <== PacketReceive(header): Unexpected!";

        _ret_code = SockClose(sockfd); // bye!
        if (_ret_code < 0)
            std::cout << " <== PacketReceive()";

        delete[] header;
		return -1;
    }
    else
		size_t pos = packet->ntohHeader(header);
	
	// Receive payload
	assert(packet->getPayloadSize() >= 0);
	if(packet->getPayloadSize() == 0 && packet->getType() == CLOSE_SIGNAL)
	{
		// Conncetion closed
		delete[] header;
		return 0;
	}
	else if(packet->getPayloadSize() > 0)
	{
		unsigned char *payload = new unsigned char[packet->getPayloadSize()+1];
		payload[packet->getPayloadSize()+1] = '\0';
		nbytes = ReadNBytes(sockfd, payload, packet->getPayloadSize());
		if (nbytes <= 0)
		{
			if (nbytes == 0)
				std::cerr << " <== PacketReceive(): Empty payload";
			else
				std::cout << " <== PacketReceive(payload): Unexpected" << std::endl;
			delete[] payload;
			delete[] header;
			return -1;
		}
		else
			packet->reallocPayload(payload, packet->getPayloadSize());

		delete[] header;
	}
    
	return 1;
}

int
ESPPacketReceive(int sockfd, ESP *packet, unsigned char **msg)
{
	int _ret_code{-1}, _size_code{0};
	unsigned char *header = new unsigned char[sizeof(struct Header)];
	// unsigned char *header = new unsigned char[sizeof(struct Header)+1];
	// header[sizeof(struct Header) + 1] = '\0';
    int nbytes = ReadNBytes(sockfd, header, sizeof(struct Header));
	// Receive header
    if (nbytes <= 0)
    {
        if (nbytes == 0)
            std::cerr << " <== ESPPacketReceive(): Empty Header";
        else
            std::cerr << " <== ESPPacketReceive(header): Unexpected";

        _ret_code = SockClose(sockfd); // bye!
        if (_ret_code < 0)
            std::cerr << " <== ESPPacketReceive()";

        delete[] header;
		return -1;
    }
    else
		packet->ntohHeader(header);
	
	// Receive payload
	assert(packet->getPayloadSize() >= 0);
	if(packet->getPayloadSize() == 0)
	{	
		delete[] header;
		return 0;
	}
	else if(packet->getPayloadSize() > 0)
	{
		unsigned char *payload = new unsigned char[packet->getPayloadSize()];
		nbytes = ReadNBytes(sockfd, payload, packet->getPayloadSize());
		// payload[packet->getPayloadSize()] = '\0';
		if (nbytes <= 0)
		{
			if (nbytes == 0)
				std::cerr << " <== ESPPacketReceive(): Empty payload";
			else
				std::cout << " <== ESPPacketReceive(payload): Unexpected";
			delete[] payload;
			delete[] header;
			return -1;
		}
		else
			packet->reallocPayload(payload, packet->getPayloadSize()); // FIXME
		
		unsigned char *taglen = new unsigned char[sizeof(uint16_t)];
		nbytes = ReadNBytes(sockfd, taglen, sizeof(uint16_t));
		if (nbytes <= 0)
		{
			if (nbytes == 0)
				std::cerr << " <== ESPPacketReceive(): Empty tag length";
			else
				std::cout << " <== ESPPacketReceive(taglength) Unexpected";
			
			delete[] header;
			delete[] payload;
			delete[] taglen;
			return -1;
		}
		else
		{
			// Deserialize tag len
			packet->ntohTaglen(taglen);
			
			// Receive tag
			size_t tag_length{packet->getTaglen()};
			unsigned char *tag_buf = new unsigned char[tag_length];
			nbytes = ReadNBytes(sockfd, tag_buf, tag_length);
			if (nbytes <= 0)
			{
				if (nbytes == 0)
					std::cerr << " <== ESPPacketReceive(): Empty tag";
				else
					std::cout << " <== ESPPacketReceive(tag) Unexpected";
				
				delete[] header;
				delete[] payload;
				delete[] taglen;
				delete[] tag_buf;
				return -1;
			}
			else
				packet->setTag(tag_buf, tag_length);
			
			// TOCHECK tieni il buffer originale
			*msg = new unsigned char[packet->getPacketSize()];
			memcpy(*msg, header, packet->getHeaderSize());
			memcpy(*msg + packet->getHeaderSize(), payload, packet->getPayloadSize());

			delete[] header;
			delete[] payload;
			delete[] taglen;
			delete[] tag_buf;

			// TOCHECK incrementi il counter del pacchetto
			packet->incCounter();
		}
	}
	return 1;
}

int
SockClose(int sockfd)
{
	short int ret{-1};
	ret = close(sockfd);
	if(ret < 0)
		std::cerr << "SockClose()::close() failed";
	
	return ret;
}