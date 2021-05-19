/** DESCRIPTION:
 * This file it is used to implement serialization / deserialization class member for all used messages (both for client side and server side)
 */
#include "../../include/Messages/Client/auth.hpp"
#include "../../include/Messages/Server/auth.hpp"
#include "../../include/Messages/Client/enc.hpp"
#include "../../include/Messages/Server/enc.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>

void
ClientHello::serialize(char *to_ser_buf)
{
    short int pos{0};
    uint16_t portno{htons(this->_port_number)};
    uint32_t nounce{htonl(this->_nounce)};

    
}