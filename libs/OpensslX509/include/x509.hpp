#ifndef _OPENSSL_X509_H_
#define _OPENSSL_X509_H_
#include <openssl/x509.h>
#include <string>

int retrieve_cert(std::string file_name, X509** cacert);

int retrieve_crl(std::string file_name, X509** crl);

int compare_subject_name(X509* cert, std::string str_name);

int setup_store(X509_STORE* store, std::string ca_cert_file_name, std::string crl_file_name);

// TODO: check if needed or not
int send_cert();

#endif