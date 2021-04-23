/**
 * SECTION
 * The packetHeader.h file define the structure of the packet header used to exchange message in the Four-in-a-row game
 */
#ifndef _PACKET_H_
#define _PACKET_H_
#include "header.hpp"

struct Packet
{
    /* data size = 9 Bytes */
    Header          _header;    
    unsigned char * _payload;

    // SECTION: Member methods
    void set_csize(char * payload);
    void set_isize(unsigned short int size);
    void set_payl(char * payload);
    void serialize(char *to_ser_buf);
    void deserialize(char *to_deser_buf);
} __attribute__((packed));
#endif