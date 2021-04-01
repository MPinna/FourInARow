// string::copy
#include <iostream>
#include <string>
#include <cstring>

struct temp {
	int a; // 4 Bytes
	char b; // 1 Byte
};

unsigned char * serialize_int(unsigned char *buffer, int value)
{
  /* Write big-endian int value into buffer; assumes 32-bit int and 8-bit char */
  buffer[0] = value >> 24;
  buffer[1] = value >> 16;
  buffer[2] = value >> 8;
  buffer[3] = value;
  return buffer + 4;
}

unsigned char * serialize_char(unsigned char *buffer, char value)
{
  buffer[0] = value;
  return buffer + 1;
}

unsigned char * serialize_temp(unsigned char *buffer, struct temp *value)
{
  buffer = serialize_int(buffer, value->a);
  buffer = serialize_char(buffer, value->b);
  return buffer;
}

// TODO deserialize 

int main()
{
	// char buffer[20];
	// std::string str ("Test string...");
	// char bufToString[20] = "Buf to string...";
	// // std::size_t length = str.copy(buffer,sizeof(buffer),0);
	// // buffer[length]='\0';
	// // std::cout << "buffer contains: " << buffer << '\n';

	// std::string _str {NULL};
	// _str = bufToString;

	// std::cout << "_str contains: " << _str << std::endl;
	// std::cout << "_str size: " << _str.length() << std::endl;

	// tmp struct
	temp t1;
	t1.a = 5;

	// Serialization
	unsigned char buff[sizeof(struct temp)];
	unsigned char * data;
	data = serialize_temp(buff, &t1);
	// NOTE: when you serialize you get block of 4 bytes
	for (size_t i = 0; i < sizeof(data); i++)
	{
		/* code */
		std::cout << "Data of [" << i << "]" << data[i] << std::endl;
	}
	
	// tmp struct 2
	temp t2;



	//unsigned char * to_back = unserialize_int(*to_int, value);
	//std::cout << back << std::endl;

	return 0;
}
