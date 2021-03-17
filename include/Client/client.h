#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "../../libs/Openssl_asymmetric/include/crypto.h"
#include "../Connection/connection.h"
#include "../Connection/connection.h"
#include "../../src/board.h"

class User
{
private:
    /* data */
    Board board;
    Connection user_socket;
    Crypto secure_communication_protocol;
    std::string username;
public:
    User();
    ~User();
};

User::User(/* args */)
{
}

User::~User()
{
}

#endif