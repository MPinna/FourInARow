// Client side C/C++ program to demonstrate Socket programming 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <iostream>
#include <stdio.h> 
#include <cstring>
#include <string>
#include <vector>

#define PORT 8080 

struct header
{
	int type {0};
	int size {0};
	char somechar [16];
};

struct temp
{
	int a;
	char b;
	char c[32];
};

struct temp2
{	
	int a_size;
	char *a;
	int b_size;
	char *b;
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
serialize_char(unsigned char *buffer, char value)
{
	buffer[0] = value;
	return buffer + 1;
}

unsigned char *
deserialize_char(unsigned char *buffer, char *value)
{
	value[0] = buffer[0];
	return buffer + 1;
}

unsigned char *
serialize_uchar(unsigned char *buffer, unsigned char value)
{
	buffer[0] = value;
	return buffer + 1;
}

unsigned char *
deserialize_uchar(unsigned char *buffer, unsigned char *value)
{
	value[0] = buffer[0];
	return buffer + 1;
}

unsigned char *
serialize_arr_of_char(unsigned char *buffer, char *value, int size)
{	
	for (size_t i = 0; i < size; i++)
	{
		/* code */
		buffer[i] = value[i];
	}
	
	return buffer + size;
}

unsigned char *
deserialize_arr_of_char(unsigned char *buffer, char *value, int size)
{
	for (size_t i = 0; i < size; i++)
	{
		/* code */
		value[i] = buffer[i];
	}
	
	return buffer + size;
}

unsigned char *
serialize_arr_of_uchar(unsigned char *buffer, unsigned char *value, int size)
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

unsigned char *serialize_temp2(unsigned char *buffer, struct temp2 *value)
{
	buffer = serialize_int(buffer, value->a_size);
	int a_size = value->a_size;
	buffer = serialize_arr_of_char(buffer, value->a, a_size);
	buffer = serialize_int(buffer, value->b_size);
	int b_size = value->b_size;
	buffer = serialize_arr_of_char(buffer, value->b, b_size);

	return buffer;
}

unsigned char *deserialize_temp2(unsigned char *buffer, struct temp2 *value)
{
	buffer = deserialize_int(buffer, &value->a_size);
	int a_size = value->a_size;
	value->a = (char *)malloc(a_size);
	buffer = deserialize_arr_of_char(buffer, value->a, a_size);
	buffer = deserialize_int(buffer, &value->b_size);
	int b_size = value->b_size;
	value->b = (char *)malloc(b_size);
	buffer = deserialize_arr_of_char(buffer, value->b, b_size);

	return buffer;
}

int main(int argc, char const *argv[]) 
{ 
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	char hello [32]= "Hello from client"; 
	char buffer[4096] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	// Packet creation

	// Packet Serialization

	// send data
	send(sock , hello, sizeof(hello) , 0 );
	printf("Message sent!\n");
	
	valread = read(sock, buffer, 1024);
	printf("%s\n",buffer );
	
	return 0;
}