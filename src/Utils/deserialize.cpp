#include "Utils/deserialize.hpp"

void deserializeHeader(unsigned char *seriliazed_data, header* data)
{

    short unsigned int *usi = (short unsigned int*)seriliazed_data;
    data->type = *usi;  usi++;
    unsigned int *ui = (unsigned int *)usi;
    data->counter = *ui;        ui++;
    data->payload_size = *ui;   ui++;

    return;
}