#ifndef _OPENSSL_DS_H_
#define _OPENSSL_DS_H_
#include <openssl/evp.h>
#include <string>

int readRSAPrvKey(std::string file_name, EVP_PKEY **rsakey);

int readRSAPubKey(std::string file_name, EVP_PKEY **rsakey);

int digestSign(
    unsigned char *msg, size_t msg_len, 
    unsigned char **sig, size_t *sig_len, 
    EVP_PKEY *prvkey, const EVP_MD *cipher
);

int digestVerify(
    unsigned char *msg, size_t msg_len, 
    unsigned char *sig, size_t sig_len, 
    EVP_PKEY *pubkey, const EVP_MD *cipher
);

#endif