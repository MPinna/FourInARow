/**
 * DESCRIPTION
 */
#include "Utils/strinops.hpp"
#include <iostream>
#include <cstring>

char *
StringToLower(char *str)
{
	for (char *c = str; *c != '\0'; c++)
		*c = tolower(*c);
	return str;
}

unsigned char *
writeInto(unsigned char *dest, unsigned char *src)
{
	dest = (unsigned char *)malloc(strlen((char *)src));
	memcpy(dest, src+'\0', strlen((char *)src));
}