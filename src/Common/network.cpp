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
    : _sockfd{0}, _server_port{DEFAULT_PORT}
{
}

Connection::Connection(int _sockfd_val, int _port_val)
    : _sockfd{_sockfd_val}, _server_port{_port_val}
{
}

void Connection::setSockFD(int sockfd)
{
    this->_sockfd = sockfd;
}



void Connection::setPort(int port)
{
    this->_server_port = port;
}

int Connection::getSockFD()
{
    return this->_sockfd;
}

int Connection::getPort()
{
    return this->_server_port;
}

struct addrinfo *
Connection::getAddrInfo(const char *node, const char *service, int family, int sock_type)
{
    // TODO
}

int Connection::Sock(int family, int socktype, int protocol)
{
    // socket(int domain, int type, int protocol)
    this->_sockfd = socket(family, socktype, protocol);
    if (this->_sockfd < 0)
        std::cout << "ERROR creating an endpoint for communication" << std::endl;

    return this->_sockfd;
}

int Connection::Bind(int socktype)
{
    this->_sockfd = Sock(AF_INET, socktype, 0);
    if (this->_sockfd == -1)
        return -1;
    // struct sockaddr_in6 sockaddr;
    memset(&this->_serv_addr, 0, sizeof(struct sockaddr_in));
    this->_serv_addr.sin_family = AF_INET;
    this->_serv_addr.sin_addr.s_addr = INADDR_ANY;
    this->_serv_addr.sin_port = htons(this->_server_port);
    if (bind(this->_sockfd, (const struct sockaddr *)&this->_serv_addr, sizeof(this->_serv_addr)) == -1)
    {
        std::cout << "ERROR: bind() failed";
        return -1;
    }
    return this->_sockfd;
}

int Connection::TcpBind()
{
    return Bind(SOCK_STREAM);
}

int Connection::UdpBind()
{
    return Bind(SOCK_DGRAM);
}

int Connection::Listen()
{
    if (listen(this->_sockfd, BACKLOG_QUEUE) == -1)
    {
        std::cout << "listen() failed" << std::endl;
        return -1;
    }
    return this->_sockfd;
}

// Reads N bytes VERIFIED
bool Connection::ReadNBytes(int socket, char *buf, std::size_t N)
{
    std::size_t offset = 0;
    while (true)
    {
        ssize_t ret = recv(socket, buf + offset, N - offset, MSG_WAITALL);
        if (ret < 0)
        {
            if (errno != EINTR)
            {
                // Error occurred
                std::cout << "IOException(strerror(errno)" << std::endl;
                return false;
            }
        }
        else if (ret == 0)
        {
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