#include "Messages/p2pChallRes.hpp"
#include "Messages/p2pMessages.hpp"
#include "Messages/packetHeader.hpp"
#include "Messages/clientServerChallRes.hpp"
#include "Messages/clientServerMessages.hpp"

void packetHeader(header* data, unsigned char *packet)
{
    unsigned short int *usi = (unsigned short int *)packet;
    *usi = data->type;  usi++;
    unsigned int *ui = (unsigned int*)usi;
    *ui = data->counter;        ui++;
    *ui = data->payload_size;   ui++;

    return;
}