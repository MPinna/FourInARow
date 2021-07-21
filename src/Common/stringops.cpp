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

int readFile(unsigned char **buf)
{
	std::string file_name;
	int ret{-1};
	while(true)
	{
		/* Open the file to be sent */
        std::cout << "Please, type the file name to read: " << std::endl;
        getline(std::cin, file_name);
        if (!std::cin)
        {
            std::cerr << "Error during input";
            return -1;
        }
		FILE *file = fopen(file_name.c_str(), "r");
		if(file == NULL)
		{
			std::cerr << "<== File not found: " << file_name;
			return 1;
		}
		
		/* Retrieve the file size */
		fseek(file, 0, SEEK_END);
		size_t msg_size = ftell(file);
		fseek(file, 0, SEEK_SET);

		/* Reading the file to be sent */
		*buf = (unsigned char *)malloc(msg_size + 1);
		ret = fread(*buf, 1, msg_size, file);
		if (ret < msg_size)
		{
			fprintf(stderr, "Error reading the file\n");
			return 1;
		}
		*buf[msg_size] = '\0';
		fclose(file);
		
	}
	std::cout << "Plaintext: " << *buf;
}