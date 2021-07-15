#include "../include/x509.hpp"
#include <openssl/pem.h>
#include <iostream>

int 
retrieve_cert(std::string file_name, X509** cacert)
{
    FILE *file = fopen(file_name.c_str(), "r");
    if (!file)
    {
        std::cerr << "Error: cannot open file '" << file_name << "' (missing?) Try with another one (or type \"exit\" to close the program\n";
        return 0;
    }
    cacert = PEM_read_X509(file, NULL, NULL, NULL);
    fclose(file);
    if (!cacert)
    {
        std::cerr << "Error: PEM_read_X509 returned NULL\n";
        return -1;
    }
    return 1;
}

int 
retrieve_crl(std::string file_name, X509_CRL** crl)
{
    FILE *file = fopen(file_name.c_str(), "r");
    if (!file)
    {
        std::cerr << "Error: cannot open file '" << file_name << "' (missing?) Try with another one (or type \"exit\" to close the program\n";
        return 0;
    }
    crl = PEM_read_X509_CRL(file, NULL, NULL, NULL);
    fclose(file);
    if (!crl)
    {
        std::cerr << "Error: PEM_read_X509_CRL returned NULL";
        return -1;
    }
    return 1;
}

int compare_subject_name(X509* cert, const char* str_name)
{   
   char* tmp;
   int ret;

   tmp = X509_NAME_oneline(X509_get_subject_name(cert), NULL, 0);
   ret = strcmp(tmp, str_name);
   free(tmp);

   return ret;
}

int
setup_store(X509_STORE* store, std::string ca_cert_file_name, std::string crl_file_name)
{
    X509 *ca_cert;
    X509_CRL *crl;
    int ret;

    ca_cert = retrieve_cert(ca_cert_file_name);
    if (!ca_cert)
    {
        std::cerr << "<== setup_store()";
        return -1;
    }
    ret = X509_STORE_add_cert(store, ca_cert);
    if (ret != 1)
    {
        std::cerr "setup_store(), Error: X509_STORE_add_cert returned " << ret;
        return -1;
    }

    /* Load the CRL */
    crl = retrieve_crl("crl.pem");
    if (!crl)
    {
        std::cerr << "<== setup_store()";
        return -1;
    }
    ret = X509_STORE_add_crl(store, crl);
    if (ret != 1)
    {
        std::cerr << "setup_store(), Error: X509_STORE_add_crl returned " << ret;
        return -1;
    }

    ret = X509_STORE_set_flags(store, X509_V_FLAG_CRL_CHECK);
    if (ret != 1)
    {
        std::cerr << "setup_store(), Error: X509_STORE_set_flags returned \n" << ret;
        return -1;
    }

    return 1;
}

// TODO check if needed or not
int
send_cert()
{

}