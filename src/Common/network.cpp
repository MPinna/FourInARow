#include "Connection/network.hpp"
#include "Utils/constant.hpp"

/**
 * DESCRIPTION
 * The source file has structured as follows:
 *  - constructor (default, specialized)
 *  - setter / getter
 *  - class members
 */

// Constructor
Connection::Connection()
    : _sock_fd{0}, _port{DEFAULT_PORT}, _optval{0}
{
}

Connection::Connection(int _sock_fd_val, int _opt_value_val, int _port_val)
    : _sock_fd{_sock_fd_val}, _optval{_opt_value_val}, _port{_port_val}
{
}

void
Connection::setSockFD(int sockfd)
{
    this->_sock_fd = sockfd;
}

void
Connection::setOptval(int optval)
{
    this->_optval = optval;
}

void
Connection::setPort(int port)
{
    this->_port = port;
}

int
Connection::getSockFD()
{
    return this->_sock_fd;
}

int
Connection::getOptval()
{
    return this->_optval;
}

int
Connection::getPort()
{
    return this->_port;
}

struct addrinfo *
Connection::getAddrInfo(const char *node, const char *service, int family, int sock_type)
{
    // TODO    
}

int
Connection::setSock(int family, int socktype, int protocol)
{
    // socket(int domain, int type, int protocol)
    this->_sock_fd =  socket(family, socktype, protocol);
    if (this->_sock_fd < 0) 
            std::cout << "ERROR creating an endpoint for communication" << std::endl;

    return this->_sock_fd;
}

int
Connection::setBind(uint16_t port, int socktype)
{
    this->_sock_fd = setSock(AF_INET, socktype, 0);
	if (this->_sock_fd == -1)
		return -1;
	// struct sockaddr_in6 sockaddr;
	memset(&this->_addr, 0, sizeof(struct sockaddr_in));
	this->_addr.sin_family = AF_INET;
    this->_addr.sin_addr.s_addr = INADDR_ANY;
    this->_addr.sin_port = htons(port);
	if (bind(this->_sock_fd, (const struct sockaddr*)&this->_addr, sizeof(this->_addr)) == -1) {
		std::cout << "bind() failed.";
		return -1;
	}
	return this->_sock_fd;
}

int
Connection::setTcpBind(uint16_t port)
{
    return setBind(port, SOCK_STREAM);
}

int
Connection::setUdpBind(uint16_t port)
{
    return setBind(port, SOCK_DGRAM);
}

// Reads N bytes VERIFIED
bool
Connection::readNBytes(int socket, char *buf, std::size_t N)
{
    std::size_t offset = 0;
    while (true) {
        ssize_t ret = recv(socket, buf + offset, N - offset, MSG_WAITALL);
        if (ret < 0) {
            if (errno != EINTR) {
                // Error occurred
                std::cout << "IOException(strerror(errno)" << std::endl;
                return false;
            }
        } else if (ret == 0) {
            // No data available anymore
            if (offset == 0)
            {
                std::cout << "No Data!" << std::endl;
                return false;
            }
            else
            {
                std::cout << "ProtocolException (\"Unexpected end of stream)" << std::endl;
                return false;
            }
        } else if (offset + ret == N) {
            // All n bytes read
            return true;
        } else {
            offset += ret;
        }
    }
}