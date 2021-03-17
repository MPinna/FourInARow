#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>

class Connection
{
private:
    /* data */
    int port, opt_value, sock, ret;
    struct sockaddr_in addr;
public:
    Connection(/* args */);
    ~Connection();
};

Connection::Connection(/* args */)
{
}

Connection::~Connection()
{
}
