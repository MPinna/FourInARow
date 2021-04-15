/**
 * SECTION
 * The packetHeader.h file define the structure of the packet header used to exchange message in the Four-in-a-row game
 */
#ifndef _PACKETHEADER_H_
#define _PACKETHEADER_H_
#include <string>
#include <vector>

struct header
{
    /* data size = 9 Bytes */
    short unsigned int  type{0}; // TODO check allpacket type
    unsigned int        counter{0};
    unsigned int        payload_size{0};
};
#endif