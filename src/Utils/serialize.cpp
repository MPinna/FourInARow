#include "Utils/serialize.hpp"

void serializeHeader(header* data, unsigned char *serialized_packet)
{
    unsigned short int *usi = (unsigned short int *)serialized_packet;
    *usi = data->type;  usi++;
    unsigned int *ui = (unsigned int*)usi;
    *ui = data->counter;        ui++;
    *ui = data->payload_size;   ui++;

    return;
}