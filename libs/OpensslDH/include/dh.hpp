#ifndef _OPENSSL_DH_H_
#define _OPENSSL_DH_H_

#include <openssl/evp.h>
#include <openssl/dh.h>
#include <iostream>

int initDHparams(EVP_PKEY *dh_key, DH *dh_params);

int setupDHparams(EVP_PKEY *dh_key, DH *dh_params);

int initDHctx(EVP_PKEY_CTX *dh_ctx, EVP_PKEY *dh_key);

int initDHKey(EVP_PKEY *my_dhkey, EVP_PKEY_CTX *dh_ctx);

int deriveSecret();
#endif