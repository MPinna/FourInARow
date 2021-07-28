#ifndef _OPENSSL_X509_H_
#define _OPENSSL_X509_H_
#include <openssl/x509.h>
#include <openssl/err.h>
#include <string>

int SetupCert(std::string file_name, X509** cacert);

int RetrieveCACert(X509** ca_cert);

int RetrieveCert(X509** ca_cert);

int SetupCrl(std::string file_name, X509_CRL** crl);

int RetrieveCrl(X509_CRL** crl);

int SetupStore(X509_STORE** store);

int compareSubjectName(X509* cert, std::string str_name);

int verifyCert();

// TODO: check if needed or not
int SendCert();
#endif