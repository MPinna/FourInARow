#include "../../include/Utils/structures.hpp"

void
SecurityCheck::update()
{
    _next = _received + 1;
    _expected = _next + 1;
}

// void
// ClientInfo::setUsername()
// {
//     size_t size{this->packet.getPayloadSize()};
//     this->username = new unsigned char[size];
//     memcpy(this->username, this->packet._payload, size);
// }