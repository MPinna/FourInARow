#include "../../include/Utils/structure.hpp"

void
User::setName(char *name)
{
    memcpy(this->_name + '\0', name, USERNAME_LENGHT_16);
}