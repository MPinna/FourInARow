#ifndef _DESERIALIZER_H_
#define _DESERIALIZER_H_
#include "Messages/p2pChallRes.hpp"
#include "Messages/p2pMessages.hpp"
#include "Messages/packetHeader.hpp"
#include "Messages/clientServerChallRes.hpp"
#include "Messages/clientServerMessages.hpp"

void deserializeHeader(unsigned char *seriliazed_data, header* data);

#endif