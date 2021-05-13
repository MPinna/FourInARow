/**
 * DESCRIPTION
 * This file it is used to manage serialization / desarialization over defined messages
 */
#include "../Messages/Client/auth.hpp"
#include "../Messages/Server/auth.hpp"
#include "../Messages/Client/enc.hpp"
#include "../Messages/Server/enc.hpp"
#include "../Messages/packet.hpp"
#include <netinet/in.h>
#include <stdlib.h>
#include <cstring>

/**
 * DESCRIPTION
 * Member class of Packet structure
 */
void Packet::set_csize(char *payload)
{
    this->_header._payload_size = strlen(payload) + 1;
}

void Packet::set_isize(unsigned short int size)
{
    this->_header._payload_size = size;
}

void Packet::set_payl(char *payload)
{
    // NOTE: il terminatore va qua ?
    this->_payload = (unsigned char *)malloc(strlen(payload) + 1);
    memcpy(this->_payload, payload + '\0', strlen(payload));
}

void Packet::serialize(char *to_ser_buf)
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
    memcpy(to_ser_buf + pos, this->_payload, this->_header._payload_size);
}

void Packet::deserializeHeader(char *ser_buf)
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

void Packet::deserializePayload(PacketType)
{
    // NOTE: before implementing ser / des class member, check its correctness

}
