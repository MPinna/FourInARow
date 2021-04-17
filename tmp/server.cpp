// Server side C/C++ program to demonstrate Socket programming
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

#define PORT 8080

struct server_user_response
{
	int num_of_user;
	std::vector<std::string> list_users;
};

struct test
{
	int n {0};
	char next[10];
};

struct header
{
	int type {0};
	int size {0};
	char somechar [16];
};

bool 
readNBytes(int fd, void *buf, std::size_t n) {
    std::size_t offset = 0;
    char *cbuf = reinterpret_cast<char*>(buf);
    while (true) {
        ssize_t ret = recv(fd, cbuf + offset, n - offset, MSG_WAITALL);
        if (ret < 0) {
            if (errno != EINTR) {
                perror("readNBytes()");
                return false;
            }
        } else if (ret == 0) {
            // No data available anymore
            if (offset == 0) return false;
            else perror("Unexpected end of stream");
        } else if (offset + ret == n) {
            // All n bytes read
            return true;
        } else {
            offset += ret;
        }
    }
}

/// Reads message from fd
std::vector<char> readMessage(int fd) {
    std::uint64_t size;
    if (readNBytes(fd, &size, sizeof(size))) {
        std::vector<char> buf(size);
        if (readNBytes(fd, buf.data(), size)) {
            return buf;
        } else {
            perror("Unexpected end of stream");
        }
    } else {
        // connection was closed
        return std::vector<char>();
    }
}

void
deserialize(char * buf, struct header *value)
{
	std::cout << "i'm in deserialize\n";
	value->type = buf[0] - '0';
	value->size = buf[1] - '0';
	std::string somechar;
	int j {0};
	std::cout << buf << std::endl;
	for (size_t i = 2; i <= sizeof(buf); i++)
	{	
		value->somechar[j] = buf[i];
		j++;
	}
}

int
deserialize_int(char * buf)
{
	std::cout << "i'm in deserialize\n";
	int type = buf[0] - '0';
	
	return type;
}



int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[4096] = {0};
	char *hello = "./server.o uno due tre";
	// // create obj	
	// server_user_response obj1;
	
	// // Fill object with junk
	// obj1.num_of_user = argc;
	// for (size_t i = 0; i < argc; i++)
	// {
	// 	obj1.list_users.push_back(argv[i]);
	// }

	// std::string set_up_data{""};

	// set_up_data.append(std::to_string(obj1.num_of_user) + "\n");

	// for (std::string tmp : obj1.list_users)
	// 	set_up_data.append(tmp);
	
	// // Fill the array of char
	// char msg[sizeof(set_up_data)] = {""};
	// std::strncat(msg, set_up_data.c_str(), sizeof(set_up_data));

	// std::cout << "sizeof - msg: " << strlen(msg) << std::endl;
	// std::cout << "sizeof - msg: " << sizeof(msg) << std::endl;
	// std::cout << "sizeof - set_up: " << strlen(set_up_data.c_str()) << std::endl;
	
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
				   &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
			 sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
							 (socklen_t *)&addrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

	char buf[4096];
	header header;
	valread = read(new_socket, buf, sizeof(buf));
	printf("%s\n", buf);

	// int type = buf[0] - '0';
	// int size = buf[1] - '0';
	// std::string somechar;
	// for (size_t i = 2; i <= 12; i++)
	// {
	// 	somechar.push_back(buf[i]);
	// }
	deserialize(buf, &header);
	
	// std::cout << "type: " << buf[0] << " size: " << buf[1] << " somechar: " << buf[2] << std::endl;

	std::cout << "Type: " << header.type << " size: " << header.size << " somechar: " << header.somechar << std::endl;

	send(new_socket, msg, strlen(msg), 0);
	printf("struct field message sent\n");
	
	return 0;
}

void
deserialize(char * buf, struct header *value)
{
	int start {0};
	int i {0};
	char seek = '\n';
	while (buf[i] != seek)
	{
		/* code */
	}
	
	value->type = buf[0] - '0';
	value->size = buf[1] - '0';
	std::string somechar;
	int j {0};
	std::cout << buf << std::endl;
	for (size_t i = 2; i <= sizeof(buf); i++)
	{	
		value->somechar[j] = buf[i];
		j++;
	}
}