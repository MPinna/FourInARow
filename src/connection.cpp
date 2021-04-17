#include "Connection/connection.hpp"

/**
 * SECTION
 * The source file has structured as follows:
 *  - constructor
 *  - destructor
 *  - class members
 */

/* DEFAULT CONSTRUCTOR */
Connection::Connection()
    : _sock_fd{0}, _port{DEFAULT_PORT}, _optvalue{0}, _addr_in{NULL}
{
}
/* SPECIALIZED SERVER CONSTRUCTOR */
// Connection::Connection(int _sock_fd_val, int _port_val, int opp_port_val, int _opt_value_val, std::string _addr_val, std::string opp_addr_val)
//     : _sock_fd{_sock_fd_val}, _port{_port_val}, opp_port{opp_port_val}, _optvalue{_opt_value_val}, opp_addr{opp_addr_val}
// {
// }
/* SPECIALIZED PEER / CLIENT CONSTRUCTOR */

/**
 * CLASS MEMBER 
 */

/**
 * @param data: std::string to be sent
 * @return 
 *  true: on success
 *  false: on failure
 */
bool
Connection::sendData(std::string data)
{
    // Send some data
    if (send(this->_sock_fd, data.c_str(), std::string(data.c_str()).length(), 0) < 0)
    {
        perror("send() failed");
        return false;
    }

    return true;
}

/**
 * @param size: size of the buffer
 * @return
 *  std::string: on success
 *  NULL: on failure
 */
std::string
Connection::receiveData(int size)
{
    int ret{0};
    char buffer[size];
    std::string received_data;

    //Receive a reply from the server
    if (recv(this->_sock_fd, buffer, sizeof(buffer), 0) < 0)
    {
        puts("read() failed");
        return NULL;
    }

    received_data = buffer;

    return received_data;
}

bool
Connection::connectClient(std::string addr, int port)
{
}

bool
Connection::connectServer()
{
    // Creating socket file descriptor
    this->_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_sock_fd < 0)
    {
        perror("socket failed");
        return false;
    }

    // Setup options for the socket referred to by the file descriptor sock
    bzero((char *)&this->_addr_in, sizeof(this->_addr_in));
    if (setsockopt(this->_sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &this->_optvalue, sizeof(this->_optvalue)))
    {
        perror("setsockopt()");
        return false;
    }
    this->_addr_in.sin_family = AF_INET;
    this->_addr_in.sin_addr.s_addr = INADDR_ANY;
    this->_addr_in.sin_port = htons(DEFAULT_SERVER_PORT);

    if (bind(this->_sock_fd, (struct sockaddr *)&this->_addr_in, sizeof(this->_addr_in)) < 0)
    {
        perror("ERROR on binding");
        return false;
    }

    if(listen(this->_sock_fd ,5) < 0)
    {
        perror("listen()");
        return false;
    }

    return true;
}
