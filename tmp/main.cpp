#include <string>
#include <iostream>
#include <vector>
#include <string.h> 
#include <cstring>
#include <openssl/x509_vfy.h>

struct server_user_response
{
    int num_of_user;
    std::vector<std::string> list_users;
};


int main(int argc, char* argv[]){
    server_user_response obj1;

    for (size_t i = 0; i < argc; i++)
    {
        obj1.list_users.push_back(argv[i]);
    }

    // for (size_t i = 0; i < 3; i++)
    // {
    //     std::cout << obj1.list_users.at(i) << std::endl;
    // }
    
    // std::cout << sizeof(std::string) << std::endl;

    // char msg [1024] = {0};
	// std::string tmp {""};
	// for (size_t i = 0; i < argc; i++)
	// {
	// 	/* code */
    //     tmp.append(obj1.list_users.at(i));
	// }

    // std::cout << "TMP string: " << tmp << std::endl;

    // std::strcpy (msg, tmp.c_str());
    
    // // memcpy(msg, tmp.c_str(), sizeof(tmp));
    
    // char msg [1024] = {0};
	
	// for(std::string tmp : obj1.list_users)
	// 	std::strncat(msg, tmp.c_str(), sizeof(tmp.c_str()));
	
    // std::cout << "Arr of char: " << msg << std::endl;

    // SECTION size 
    // std::cout << "Size of std::string: " << sizeof(std::string) << std::endl;
    // std::cout << "Size of short unsigned int: " << sizeof(short unsigned int) << std::endl;
    // std::cout << "Size of int8_t and uint8_t: " << sizeof(int8_t) << ", " << sizeof(uint8_t) << std::endl;
    // std::cout << "Size of unsigned int: " << sizeof(unsigned int) << std::endl;
    // std::cout << "Size of X509*: " << sizeof(X509*) << std::endl;
    // std::cout << "Size of bool: " << sizeof(bool) << std::endl;

    // uint8_t n = 65;
    // std::cout << n << std::endl;

    // SECTION string size
    std::string sized {NULL};
    std::cout << "Sized: " << sized.length() << std::endl;
    std::cout << "Sized: " << sized << std::endl;
    sized = sized + "Additional\0 data";
    std::cout << "Sized: " << sized.length() << std::endl;
    std::cout << "Sized: " << sized << std::endl;
    
    return 0;
}