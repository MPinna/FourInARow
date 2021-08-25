/**
 * DESCRIPTION
 */
#include "../../include/Utils/utils.hpp"

char *
StringToLower(char *str)
{
	for (char *c = str; *c != '\0'; c++)
		*c = tolower(*c);
	return str;
}

int 
readFile(unsigned char **buf)
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
            std::cerr << " <==Error during input";
            return -1;
        }
		FILE *file = fopen(file_name.c_str(), "r");
		if(file == NULL)
		{
			std::cerr << " <== File not found: " << file_name;
			return 0;
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
			std::cerr << " <== Error reading the file";
			return -1;
		}
		*buf[msg_size] = '\0';
		fclose(file);
	}
	std::cout << "File Content: " << *buf;
	return 1;
}

unsigned int
getRandomInt()
{
	unsigned int seed;
    FILE *urnd;
    urnd = fopen("/dev/urandom", "rb");
    if(!urnd)
    {
        std::cerr << "Header::Header::fopen() failed!" << std::endl;
        return -1;
    }
    if((fread(&seed, 1, sizeof(seed), urnd)) != sizeof(seed))
    {
        std::cerr << "Header::Header::fread() failed!" << std::endl;
        return -1;
    }
    fclose(urnd);
    srand(seed);
    return rand();
}