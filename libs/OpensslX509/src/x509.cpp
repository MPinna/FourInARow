#include "../include/x509.hpp"
#include <openssl/pem.h>
#include <iostream>
#include <cstring>

int 
SetupCert(std::string file_name, X509** cacert)
{
    FILE *file = fopen(file_name.c_str(), "r");
    if (!file)
    {
        std::cerr << "Error: cannot open file '" << file_name << "' (missing?) Try with another one (or type \"exit\" to close the program\n";
        return 0;
    }
    *cacert = PEM_read_X509(file, NULL, NULL, NULL);
    fclose(file);
    if (!cacert)
    {
        std::cerr << "Error: PEM_read_X509 returned NULL\n";
        return -1;
    }
    return 1;
}

X509*
RetrieveCert()
{
    X509* cacert;
    std::string cacert_file_name;
    while(true)
    {
        int ret{-1};
        std::cout << "Please, type the PEM file containing a trusted CA's certificate: ";
        getline(std::cin, cacert_file_name);
        if (!std::cin)
        {
            std::cerr << "Error during input";
            return NULL;
        }
        else if(cacert_file_name.compare("exit"))
        {
            std::cout << "You choose to exit. Terminated execution...";
            return NULL;
        }
        else
        {
            ret = SetupCert(cacert_file_name, &cacert);
            if(ret <= 0)
            {
                if (ret == 0)
                    continue;
                if (ret == -1)
                    std::cerr << "RetrieveCert() Error!";
                    return NULL;
            }
            else
                break;
        }
    }
    return cacert;
}

int 
SetupCrl(std::string file_name, X509_CRL** crl)
{
    FILE *file = fopen(file_name.c_str(), "r");
    if (!file)
    {
        std::cerr << "Error: cannot open file '" << file_name << "' (missing?) Try with another one (or type \"exit\" to close the program\n";
        return 0;
    }
    *crl = PEM_read_X509_CRL(file, NULL, NULL, NULL);
    fclose(file);
    if (!crl)
    {
        std::cerr << "Error: PEM_read_X509_CRL returned NULL";
        return -1;
    }
    return 1;
}

X509_CRL* 
RetrieveCrl(std::string crl_file_name)
{
    X509_CRL* crl;
    std::string crl_file_name;
    while(true)
    {
        int ret{-1};
        std::cout << "Please, type the PEM file containing a certificate revocation list (CRL): ";
        getline(std::cin, crl_file_name);
        if (!std::cin)
        {
            std::cerr << "Error during input";
            return NULL;
        }
        else if(crl_file_name.compare("exit"))
        {
            std::cout << "You choose to exit. Terminated execution...";
            return NULL;
        }
        else
        {
            ret = SetupCrl(crl_file_name, &crl);
            if(ret <= 0)
            {
                if (ret == 0)
                    continue;
                if (ret == -1)
                    std::cerr << "RetrieveCrl() Error!";
                    return NULL;
            }
            else
                break;
        }
    }
    return crl;
}


int compareSubjectName(X509* cert, const char* str_name)
{   
   char* tmp;
   int ret;

   tmp = X509_NAME_oneline(X509_get_subject_name(cert), NULL, 0);
   ret = strcmp(tmp, str_name);
   free(tmp);

   return ret;
}

int
SetupStore(X509_STORE** store, std::string ca_cert_file_name, std::string crl_file_name)
{
    X509_CRL *crl;
    int ret;

    X509 *ca_cert = RetrieveCert(ca_cert_file_name);
    if(!ca_cert)
    {
        std::cerr << " <== SetupStore() failed retrieving CA Cert";
        return -1;
    }
    ret = X509_STORE_add_cert(*store, ca_cert);
    if (ret != 1)
    {
        std::cerr << "SetupStore(), Error: X509_STORE_add_cert returned: " << ret;
        return -1;
    }

    /* Load the CRL */
    ret = SetupCrl(crl_file_name, &crl);
    if (!crl)
    {
        std::cerr << "<== SetupStore() failed retrieving CRL";
        return -1;
    }
    ret = X509_STORE_add_crl(*store, crl);
    if (ret != 1)
    {
        std::cerr << "SetupStore(), Error: X509_STORE_add_crl returned " << ret;
        return -1;
    }

    ret = X509_STORE_set_flags(*store, X509_V_FLAG_CRL_CHECK);
    if (ret != 1)
    {
        std::cerr << "SetupStore(), Error: X509_STORE_set_flags returned \n" << ret;
        return -1;
    }

    return 1;
}

// TODO check if needed or not
int
SendCert()
{

}