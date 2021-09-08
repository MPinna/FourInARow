#ifndef _OPENSSL_DH_H_
#define _OPENSSL_DH_H_

#include <openssl/evp.h>
#include <openssl/dh.h>
#include <iostream>

int setupDHctx(EVP_PKEY *dh_params, DH *temp_params, EVP_PKEY_CTX *dh_ctx);

int setupDHKey(EVP_PKEY *my_dhkey, EVP_PKEY_CTX *dh_ctx);

int deriveSecret();

#endif