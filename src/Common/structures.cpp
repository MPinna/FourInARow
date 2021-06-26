#include "../../include/Utils/structures.hpp"

void
User::setName(char *name)
{
    memcpy(this->_name + '\0', name, USERNAME_LENGHT_16);
}

int
SecurityCheck::update()
{
    // TODO
}