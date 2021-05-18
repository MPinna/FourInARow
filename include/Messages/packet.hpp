/**
 * DESCRIPTION
 * The packetHeader.h file define the structure of the packet header used to exchange message in the Four-in-a-row game
 */
#ifndef _PACKET_H_
#define _PACKET_H_
#include "header.hpp"
#include "../Utils/constant.hpp"

struct Packet
{
    /* data size = 9 Bytes */
    Header          _header;
    unsigned char * _payload;

    // DESCRIPTION: Member methods
    void setCharSize(char * payload);
    void setCharSize(unsigned short int size);
    void SerializeHeader(char *to_ser_buf);
    void DeserializeHeader(char * ser_buf);
    void SerializePayload(PacketType type);
    void DeserializePayload(PacketType type);
}__attribute__((packed));
#endif