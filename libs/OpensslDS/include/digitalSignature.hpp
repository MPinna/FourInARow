#ifndef _OPENSSL_DS_H_
#define _OPENSSL_DS_H_
#include <openssl/evp.h>
#include <cstring>

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