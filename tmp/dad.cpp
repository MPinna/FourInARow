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


struct dad
{
	/* data */
	int size_a {0};
	std::basic_string<unsigned char> a;
	int size_b {0};
	std::basic_string<unsigned char> b;
};

struct temp2
{	
	int a_size;
	unsigned char *a = 0;
	int b_size;
	unsigned char *b = 0;
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
	buffer = serialize_arr_of_uchar(buffer, value->a, value->a_size);
	buffer = serialize_int(buffer, value->b_size);
	buffer = serialize_arr_of_uchar(buffer, value->b, value->b_size);

	return buffer;
}

unsigned char *deserialize_temp2(unsigned char *buffer, struct temp2 *value)
{
	buffer = deserialize_int(buffer, &value->a_size);
	value->a = (unsigned char *)malloc(value->a_size);
	buffer = deserialize_arr_of_uchar(buffer, value->a, value->a_size);
	
	buffer = deserialize_int(buffer, &value->b_size);
	value->b = (unsigned char *)malloc(value->b_size);
	buffer = deserialize_arr_of_uchar(buffer, value->b, value->b_size);

	return buffer;
}

int main()
{
	unsigned char adata[] = "Ultimo test";
	unsigned char bdata[] = "Ultimo testb";
	temp2 sdata;
	sdata.a = (unsigned char *)malloc(strlen((char *)adata)+1);
	strncpy((char *)sdata.a, (char *)adata, strlen((char *)adata));
	sdata.a_size = strlen((char *)adata);
	sdata.b = (unsigned char *)malloc(strlen((char *)bdata)+1);
	strncpy((char *)sdata.b, (char *)bdata, strlen((char *)bdata));
	sdata.b_size = strlen((char *)adata);

	// TODO
	unsigned char buf[sizeof(sdata)], *ptr, *ptr2;
	ptr = serialize_temp2(buf, &sdata);
	std::cout << buf;
	

	// std::string data {"Ciaone proprio v2"};
	// unsigned char *buf, *ptr, *ptr2;
	// buf = (unsigned char *)malloc(data.length());
	// ptr = serialize_arr_of_string(buf, data, data.length());
	// // std::cout << buf << std::endl;

	// size_t size = strlen((char *)buf);
	// dvect rdata;
	// rdata.buf = buf;
	// rdata = deserialize_arr_of_string(rdata,size);
	// for (size_t i = 0; i < rdata.data.size(); i++)
	// {
	// 	std::cout << rdata.data[i];
	// }
	
	
	// std::string tmp ( reinterpret_cast<char const*>(buf), size);	

	return 0;
}
	// VERIFIED array_of_uchar
	// unsigned char data[] = "Ciaone proprio";
	// unsigned char *test = 0, *buf, *ptr, *ptr2;
	// test = (unsigned char *)malloc(sizeof(data)+1);
	// strncpy((char *)test, (char *)data, sizeof(data)+1);
	// buf= (unsigned char *)malloc(sizeof(data)+1);
	// ptr = serialize_arr_of_uchar(buf, test, sizeof(data)+1);
	// std::cout << "Data: " << data << std::endl;

	// unsigned char *rtest;
	// int size = strlen((char *)buf)+1;
	// rtest = (unsigned char *)malloc(size);
	// ptr2 = deserialize_arr_of_uchar(buf, rtest, size);
	// std::cout << "rTest: " << rtest << std::endl;
	

	// // VERIFIED uchar
	// unsigned char test = 'a';
	// unsigned char *buf, *ptr;
	// buf = (unsigned char *)malloc(sizeof(unsigned char));
	// ptr = serialize_uchar(buf, test);
	// std::cout << "test: " << test << std::endl;

	// unsigned char rtest;
	// ptr = deserialize_uchar(buf, &rtest);
	// std::cout << "rtest: " << rtest << std::endl;


	// // VERIFIED Full ser / des temp2
	// char data[] = "Certificate Signature and GCM Tag plus some additional data";
	// char data2[] = "Certificate2 Signature2 and2 GCM2 Tag2 plus2 some2 additional2 data2";
	// //std::cout << "" << << std::endl;
	// std::cout << "adatalen: " << strlen(data)+1 << std::endl;
	// std::cout << "bdatalen: " << strlen(data2)+1 << std::endl;
	// temp s_temp;
	// s_temp.a_size = strlen(data)+1;
	// s_temp.a = (char *)malloc(strlen(data) + 1);
	// strncpy(s_temp.a, data, strlen(data)+1);
	// s_temp.b_size = strlen(data2)+1;
	// s_temp.b = (char *)malloc(strlen(data2) + 1);
	// strncpy(s_temp.b, data2, strlen(data2)+1);
	// unsigned char buf[sizeof(s_temp)], *ptr;
	// ptr = serialize_temp(buf, &s_temp);

	// temp d_temp;
	// unsigned char *ptr2;
	// deserialize_temp(buf, &d_temp);

	// std::cout << "asize: " << d_temp.a_size << ", a: " << d_temp.a 
	// << "\nbsize: " << d_temp.b_size << ", b: " << d_temp.b << std::endl;