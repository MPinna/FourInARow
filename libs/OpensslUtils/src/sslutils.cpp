#include "../include/sslutils.hpp"

int
ReadPrvKey(std::string file_name, EVP_PKEY **prvkey)
{
    // load my private key:
    FILE *prvkey_file = fopen(file_name.c_str(), "r");
    assert(prvkey_file != NULL);
    if (!prvkey_file)
    {
        std::cerr << "Error: cannot open file '" << file_name << "' (missing?)\n";
        return 0;
    }
    *prvkey = PEM_read_PrivateKey(prvkey_file, NULL, NULL, NULL);
    assert(prvkey != NULL);
    if (!prvkey)
    {
        std::cerr << "Error: PEM_read_PrivateKey returned NULL\n";
        return -1;
    }
    
    fclose(prvkey_file);

    return 1;
}

int
RetrievePrvKey(EVP_PKEY **prvkey, const char * prvkeyname)
{
    if((ReadPrvKey(prvkeyname, prvkey)) == 1)
    {
        return 1;
    }
    // Else, the default name is not valid, insert your own private key name
    std::string prvkey_file_name;
    std::cout << "Please, type the PEM file containing private key: ";
    while(true)
    {
        int ret{-1};
        getline(std::cin, prvkey_file_name);
        if (!std::cin)
        {
            std::cerr << "Error during input";
            return -1;
        }
        
        if(prvkey_file_name.compare("exit") == 0)
        {
            std::cout << "You choose to exit. Terminated execution...";
            return 0;
        }
        else
        {
            ret = ReadPrvKey(prvkey_file_name, prvkey);
            if(ret <= 0)
            {
                if (ret == 0)
                    continue;
                if (ret == -1)
                    std::cerr << "RetrievePrvKey() Error!";
                    return -1;
            }
            else
                break;
        }
    }
    return 1;
}

int
ReadPubKey(std::string file_name, EVP_PKEY **pubkey)
{
    // load my private key:
    FILE *pubkey_file = fopen(file_name.c_str(), "r");
    assert(pubkey_file != NULL);
    if (!pubkey_file)
    {
        std::cerr << "Error: cannot open file '" << file_name << "' (missing?)\n";
        return 0;
    }
    *pubkey = PEM_read_PUBKEY(pubkey_file, NULL, NULL, NULL);
    assert(pubkey != NULL);
    if (!pubkey)
    {
        std::cerr << "Error: PEM_read_PrivateKey returned NULL\n";
        return 0;
    }
    fclose(pubkey_file);

    return 1;
}

int
RetrievePubKey(EVP_PKEY **pubkey, char * pubkeyname)
{
    if((ReadPubKey(pubkeyname, pubkey)) == 1)
    {
        return 1;
    }
    std::string pubkey_file_name;
    std::cout << "Please, type the PEM file containing public key: ";
    while(true)
    {
        int ret{-1};
        getline(std::cin, pubkey_file_name);
        if (!std::cin)
        {
            std::cerr << "Error during input";
            return -1;
        }
        
        if(pubkey_file_name.compare("exit") == 0)
        {
            std::cout << "You choose to exit. Terminated execution...";
            return 0;
        }
        else
        {
            ret = ReadPubKey(pubkey_file_name, pubkey);
            if(ret <= 0)
            {
                if (ret == 0)
                    continue;
                if (ret == -1)
                    std::cerr << "RetrievePubKey() Error!";
                    return -1;
            }
            else
                break;
        }
    }
    return 1;
}