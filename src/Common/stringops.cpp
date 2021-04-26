#include "Utils/strinops.hpp"
#include <iostream>

char *string_to_lower(char *str)
{
	for (char *c = str; *c != '\0'; c++)
		*c = tolower(*c);
	return str;
}