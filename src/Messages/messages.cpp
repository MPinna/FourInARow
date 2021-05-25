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
PeerHello::setUsername(unsigned char *name)
{
    if(strlen((char *)name) > 16)
        std::cout << "/*\\setUsername() lenght greater than: " << _16_BYTES << std::endl;
        exit(1);
    memcpy(this->_username, name, sizeof(this->_username));
}