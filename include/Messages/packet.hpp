/**
 * DESCRIPTION
 * The Packet structure define the generic structure of a packet, made up by:
 *  - header (type, count, payload size)
 *  - payload(which is used to carry out the information)
 * The Packet class member is in charge of:
 *  - set the payload size (given an array of char)
 *  - manage serialization / deserialization for both:
 *      - header
 *      - payload
 */
#ifndef _PACKET_H_
#define _PACKET_H_
#include "header.hpp"
#include "../Utils/constant.hpp"

typedef struct __attribute__((packed))
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
} Packet;
#endif