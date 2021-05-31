/**
 * DESCRIPTION
 */
#include "Utils/strinops.hpp"

char *
StringToLower(char *str)
{
	for (char *c = str; *c != '\0'; c++)
		*c = tolower(*c);
	return str;
}

void
writeInto(unsigned char *dest, unsigned char *src)
{
	dest = (unsigned char *)malloc(strlen((char *)src));
	memcpy(dest, src+'\0', strlen((char *)src));
}

void
writeInto(char *dest, char *src)
{
	dest = (char *)malloc(strlen(src));
	memcpy(dest, src+'\0', strlen(src));
}