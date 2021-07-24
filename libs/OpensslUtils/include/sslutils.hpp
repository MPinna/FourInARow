/**
 * SECTION
 * "utils.h" it is used to implement all required functions which are helpful for openssl library:
 *      - Computationally Secure Pseudo Random Number Generator
 *      - Reading and Writing ".pem" format file
 */
#ifndef _OPENSSL_UTILS_H_
#define _OPENSSL_UTILS_H_
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <iostream>
#include <cassert>
#include <string>

int ReadPrvKey(std::string file_name, EVP_PKEY **prvkey);

int RetrievePrvKey(EVP_PKEY **prvkey);

int ReadPubKey(std::string file_name, EVP_PKEY **pubkey);

int RetrievePubKey(EVP_PKEY **pubkey);

#endif