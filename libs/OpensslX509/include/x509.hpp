#ifndef _OPENSSL_X509_H_
#define _OPENSSL_X509_H_
#include <openssl/x509.h>
#include <string>

int SetupCert(std::string file_name, X509** cacert);

X509* RetrieveCert();

int SetupCrl(std::string file_name, X509_CRL** crl);

X509_CRL* RetrieveCrl();

int compareSubjectName(X509* cert, std::string str_name);

int SetupStore(X509_STORE** store, std::string ca_cert_file_name, std::string crl_file_name);

// TODO: check if needed or not
int SendCert();
#endif