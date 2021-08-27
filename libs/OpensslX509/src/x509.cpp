#include "../../../include/Utils/constant.hpp"
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
        std::cerr << "Error: cannot open file '" << file_name << "' (missing?)\nTry with another one (or type \"exit\" to close the program): ";
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

int
RetrieveCACert(X509** ca_cert)
{
    if((SetupCert(DEFAULT_CA_CERT, ca_cert)) == 1)
    {
        return 1;
    }
    std::string cacert_file_name;
    std::cout << "Please, type the PEM file containing certificate: ";
    while(true)
    {
        int ret{-1};
        getline(std::cin, cacert_file_name);
        if (!std::cin)
        {
            std::cerr << "Error during input";
            return -1;
        }
        
        if(cacert_file_name.compare("exit") == 0)
        {
            std::cout << "You choose to exit. Terminated execution...";
            return 0;
        }
        else
        {
            ret = SetupCert(cacert_file_name, ca_cert);
            if(ret <= 0)
            {
                if (ret == 0)
                    continue;
                if (ret == -1)
                    std::cerr << "RetrieveCert() Error!";
                    return -1;
            }
            else
                break;
        }
    }
    return 1;
}

int RetrieveCert(X509** ca_cert)
{
    if((SetupCert(DEFAULT_CERT, ca_cert)) == 1)
    {
        return 1;
    }
    std::string cacert_file_name;
    std::cout << "Please, type the PEM file containing certificate: ";
    while(true)
    {
        int ret{-1};
        getline(std::cin, cacert_file_name);
        if (!std::cin)
        {
            std::cerr << "Error during input";
            return -1;
        }
        
        if(cacert_file_name.compare("exit") == 0)
        {
            std::cout << "You choose to exit. Terminated execution...";
            return 0;
        }
        else
        {
            ret = SetupCert(cacert_file_name, ca_cert);
            if(ret <= 0)
            {
                if (ret == 0)
                    continue;
                if (ret == -1)
                    std::cerr << "RetrieveCert() Error!";
                    return -1;
            }
            else
                break;
        }
    }
    return 1;
}

int 
SetupCrl(std::string file_name, X509_CRL** crl)
{
    FILE *file = fopen(file_name.c_str(), "r");
    if (!file)
    {
        std::cerr << "Error: cannot open file '" << file_name << "' (missing?) \nTry with another one (or type \"exit\" to close the program): ";
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

int
RetrieveCrl(X509_CRL** crl)
{
    if((SetupCrl(DEFAULT_CRL, crl)) == 1)
    {
        return 1;
    }
    std::string crl_file_name;
    std::cout << "Please, type the PEM file containing a certificate revocation list (CRL): ";
    while(true)
    {
        int ret{-1};
        getline(std::cin, crl_file_name);
        if (!std::cin)
        {
            std::cerr << "Error during input";
            return -1;
        }
        if(crl_file_name.compare("exit") == 0)
        {
            std::cout << "You choose to exit. Terminated execution...";
            return 0;
        }
        else
        {
            ret = SetupCrl(crl_file_name, crl);
            if(ret <= 0)
            {
                if (ret == 0)
                    continue;
                if (ret == -1)
                    std::cerr << "RetrieveCrl() Error!";
                    return -1;
            }
            else
                break;
        }
    }
    return 1;
}

int
SetupStore(X509_STORE** store)
{
    int ret;
    X509 *ca_cert;
    ret = RetrieveCACert(&ca_cert);
    if(ret <= 0)
    {
        if(ret < 0)
        {
            std::cerr << "<== SetupStore() Error!";
            return -1;
        }
        if(ret == 0)
        {
            std::cerr << "<== SetupStore() exit()";
            return 0;
        }
    }

    /* Load the CRL */
    X509_CRL *crl;
    ret = RetrieveCrl(&crl);
    if(ret <= 0)
    {
        if(ret < 0)
        {
            std::cerr << "<== SetupStore() Error!";
            return -1;
        }
        if(ret == 0)
        {
            std::cerr << "<== SetupStore() exit()";
            return 0;
        }
    }

    // build a store with the CA's certificate and the CRL:
    *store = X509_STORE_new();
    if (!*store)
    {
        std::cerr << "SetupStore() Error: X509_STORE_new returned NULL\n"
             << ERR_error_string(ERR_get_error(), NULL) << "\n";
        return -1;
    }
    ret = X509_STORE_add_cert(*store, ca_cert);
    if (ret != 1)
    {
        std::cerr << "SetupStore() Error: X509_STORE_add_cert returned " << ret << "\n"
             << ERR_error_string(ERR_get_error(), NULL) << "\n";
        return -1;
    }
    ret = X509_STORE_add_crl(*store, crl);
    if (ret != 1)
    {
        std::cerr << "Error: X509_STORE_add_crl returned " << ret << "\n"
             << ERR_error_string(ERR_get_error(), NULL) << "\n";
        return -1;
    }
    ret = X509_STORE_set_flags(*store, X509_V_FLAG_CRL_CHECK);
    if (ret != 1)
    {
        std::cerr << "Error: X509_STORE_set_flags returned " << ret << "\n"
             << ERR_error_string(ERR_get_error(), NULL) << "\n";
        return -1;
    }

    return 1;
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
verifyCert(X509_STORE *store, X509* cert)
{
    int ret{-1};
    X509_STORE_CTX *certvfy_ctx = X509_STORE_CTX_new();
    if (!certvfy_ctx)
    {
        std::cerr << "Error: X509_STORE_CTX_new returned NULL\n"
             << ERR_error_string(ERR_get_error(), NULL) << "\n";
        return -1;
    }
    ret = X509_STORE_CTX_init(certvfy_ctx, store, cert, NULL);
    if (ret != 1)
    {
        std::cerr << "verifyCert() Error: X509_STORE_CTX_init returned " << ret << "\n"
             << ERR_error_string(ERR_get_error(), NULL) << "\n";
        return -1;
    }
    ret = X509_verify_cert(certvfy_ctx);
    if (ret != 1)
    {
        std::cerr << "verifyCert() Error: X509_verify_cert returned " << ret << "\n"
             << ERR_error_string(ERR_get_error(), NULL) << "\n";
        return -1;
    }
    X509_STORE_CTX_free(certvfy_ctx);
    return 1;
}

int
serializeCert(X509* cert, unsigned char **ser_cert)
{
    return 1;
}