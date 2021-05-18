/**
 * DESCRIPTION
 */
#include "Utils/strinops.hpp"
#include <iostream>

char *StringToLower(char *str)
{
	for (char *c = str; *c != '\0'; c++)
		*c = tolower(*c);
	return str;
}