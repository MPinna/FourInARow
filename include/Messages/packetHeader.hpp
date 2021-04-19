/**
 * SECTION
 * The packetHeader.h file define the structure of the packet header used to exchange message in the Four-in-a-row game
 */
#ifndef _PACKETHEADER_H_
#define _PACKETHEADER_H_
#include <string>
#include <vector>

struct Header
{
    /* data size = 9 Bytes */
    short unsigned int  type{0}; // TODO check allpacket type
    unsigned int        counter{0};
    unsigned int        payload_size{0};

    void serialize(Header *packet, char *ser_buf)
    {
        short unsigned int* sui = (short unsigned int*)ser_buf;
        *sui = packet->type;    sui++;
        unsigned int* ui = (unsigned int*)sui;
        *ui = packet->counter;  ui++;
        *ui = packet->payload_size; ui++;
    }
    void deserilize(char *ser_buf, Header *packet)
    {
        short unsigned int *usi = (short unsigned int*)ser_buf;
        packet->type = *usi;  usi++;
        unsigned int* ui = (unsigned int*)usi;
        packet->counter = *ui; ui++;
        packet->payload_size = *ui; ui++;
    }
};
#endif