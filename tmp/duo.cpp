#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <bitset>


struct duo
{
	/* data */
	int size_a {0};
	std::basic_string<unsigned char> a;
	int size_b {0};
	std::basic_string<unsigned char> b;
};

unsigned char *
serialize_int(unsigned char *buffer, int value)
{
	/* Serialize little-endian (least significant byte -> first byte position) int value into buffer; assumes 32-bit int and 8-bit char. */
	buffer[0] = value;
	buffer[1] = value >> 8;
	buffer[2] = value >> 16;
	buffer[3] = value >> 24;

	return buffer + 4;
}

unsigned char *
deserialize_int(unsigned char *buffer, int *value)
{
	value[0] = buffer[0];
	value[1] = buffer[1];
	value[2] = buffer[2];
	value[3] = buffer[3];
	
	return buffer + 4;
}

unsigned char *
serialize_arr_of_uchar(unsigned char *buffer, std::basic_string<unsigned char> value, int size)
{	
	for (size_t i = 0; i < size; i++)
	{
		/* code */
		buffer[i] = value[i];
	}
	
	return buffer + size;
}

unsigned char *
deserialize_arr_of_uchar(unsigned char *buffer, unsigned char *value, int size)
{
	for (size_t i = 0; i < size; i++)
	{
		/* code */
		value[i] = buffer[i];
	}
	
	return buffer + size;
}

int main()
{
    std::basic_string<unsigned char> test;
    strncpy(test, "Ciao Mondo", strlen("Ciao Mondo"));
    int size {sizeof(test)};
    unsigned char *buf, *ptr, *ptr2;
    buf = (unsigned char *)malloc(sizeof(test));
    serialize_arr_of_uchar(buf, test, size);
    //basic_string( const CharT* s, size_type count);

    return 0;
}