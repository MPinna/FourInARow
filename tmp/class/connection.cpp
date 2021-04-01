#include "connection.hpp"

/**
 * SECTION
 * The source file has structured as follows:
 *  - constructor
 *  - destructor
 *  - class members
 */

/* CONSTRUCTOR */
Connection::Connection()
    : _sock_fd{-1}, _port{0}, opp_port{0}, _optvalue{0}
{
}

Connection::Connection(int _sock_fd_val, int rec_sock_fd_val, int _port_val, int opp_port_val, int _opt_value_val)
    : _sock_fd{_sock_fd_val}, rec_sock_fd{rec_sock_fd_val}, _port{_port_val}, opp_port{opp_port_val}, _optvalue{_opt_value_val}
{
}

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

/// Reads n bytes from fd.
bool 
Connection::readNBytes(int fd, void *buf, std::size_t n) {
    std::size_t offset = 0;
    char *cbuf = reinterpret_cast<char*>(buf);
    while (true) {
        ssize_t ret = recv(fd, cbuf + offset, n - offset, MSG_WAITALL);
        if (ret < 0) {
            if (errno != EINTR) {
                perror("readNBytes()");
                return false;
            }
        } else if (ret == 0) {
            // No data available anymore
            if (offset == 0) return false;
            else perror("Unexpected end of stream");
        } else if (offset + ret == n) {
            // All n bytes read
            return true;
        } else {
            offset += ret;
        }
    }
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
    // int ret{0};
    // char buffer[size];
    // std::string received_data;

    // //Receive a reply from the server
    // if (recv(this->_sock_fd, buffer, sizeof(buffer), 0) < 0)
    // {
    //     puts("read() failed");
    //     return NULL;
    // }

    // received_data = buffer;

    // return received_data;
}

bool
Connection::connectClient(std::string addr, int port)
{
    return true;
}

bool
Connection::connectServer()
{
    int ret{0};
    char buffer[256];
    socklen_t clilen = sizeof(this->opp_addr_in);
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
        perror("binding() ERROR");
        return false;
    }

    if (listen(this->_sock_fd, 5) < 0)
    {
        perror("listen()");
        return false;
    }

    this->rec_sock_fd = accept(this->_sock_fd, (struct sockaddr *)&this->opp_addr_in, &clilen);
    if (this->rec_sock_fd < 0)
        perror("ERROR on accept");

    while (true)
    {
        /**
         * TOIMPLEMENT
         *  - read header
         *  - read
         */
    }
    
    
    // TOCHECK can you manage send and receive by functions ?
    bzero(buffer, 256);
    ret = read(this->rec_sock_fd, buffer, 255);
    if (ret < 0)
    {
        perror("ERROR reading from socket");
        return false;
    }

    std::cout << "Here is the message: " << buffer << std::endl;

    ret = write(this->rec_sock_fd, "I got your message", 18);
    if (ret < 0)
    {
        perror("ERROR writing to socket");
        return false;
    }

    close(this->rec_sock_fd);
    close(this->_sock_fd);

    return true;
}
