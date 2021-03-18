#include "../Connection/connection.hpp"

Connection::Connection()
    : sock{-1}, port{"0"}, _addr{NULL}
{
}

Connection::Connection(int sock_val, std::string port_val, std::string _addr_val)
    : sock{sock_val}, port{port_val}, _addr{_addr_val}
{
}

Connection::~Connection()
{
}