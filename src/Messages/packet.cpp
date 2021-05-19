/**
 * DESCRIPTION
 * This file it is used to manage serialization / desarialization over defined messages
 */
#include "../Messages/packet.hpp"
#include <netinet/in.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>

/**
 * DESCRIPTION
 * Member class of Packet structure
 */
void Packet::setCharSize(char *payload)
{
    this->_header._payload_size = strlen(payload);
}

void Packet::setCharSize(unsigned short int size)
{
    this->_header._payload_size = size;
}

void Packet::SerializeHeader(char *to_ser_buf)
{
    short int pos{0};
    uint16_t type{htons(this->_header._type)}, psize{htons(this->_header._payload_size)};
    uint32_t count{htonl(this->_header._counter)};

    memcpy(to_ser_buf, &type, sizeof(type));
    pos += sizeof(type);
    memcpy(to_ser_buf + pos, &count, sizeof(count));
    pos += sizeof(count);
    memcpy(to_ser_buf + pos, &psize, sizeof(psize));
    pos += sizeof(psize);
}

void Packet::DeserializeHeader(char *ser_buf)
{
    short int pos{0};
    u_int16_t dtype, dpsize;
    u_int32_t dcount;

    memcpy(&dtype, ser_buf, sizeof(dtype));
    this->_header._type = (ntohs(dtype));
    pos += sizeof(dtype);
    memcpy(&dcount, ser_buf + pos, sizeof(dcount));
    this->_header._counter = (ntohl(dcount));
    pos += sizeof(dcount);
    memcpy(&dpsize, ser_buf + pos, sizeof(dpsize));
    this->_header._payload_size = (ntohs(dpsize));
    pos += sizeof(dpsize);
}

// void Packet::SerializePayload(PacketType type)
// {   
//     switch (type)
//     {
//     case CLIENT_HELLO:
//         break;
    
//     case SERVER_HELLO:
//         break;

//     default:
//         break;
//     }
// }

// void Packet::DeserializePayload(PacketType type)
// {
    
// }