#include "../include/digitalSignature.hpp"
#include <openssl/pem.h>
#include <openssl/err.h>
#include <iostream>
#include <cassert>

int
readRSAPrvKey(std::string file_name, EVP_PKEY **rsakey)
{
    // load my private key:
    FILE *prvkey_file = fopen(file_name.c_str(), "r");
    assert(prvkey_file != NULL);
    if (!prvkey_file)
    {
        std::cerr << "Error: cannot open file '" << file_name << "' (missing?)\n";
        return 0;
    }
    *rsakey = PEM_read_PrivateKey(prvkey_file, NULL, NULL, NULL);
    assert(rsakey != NULL);
    if (!rsakey)
    {
        std::cerr << "Error: PEM_read_PrivateKey returned NULL\n";
        return 0;
    }
    
    fclose(prvkey_file);

    return 1;
}

int
readRSAPubKey(std::string file_name, EVP_PKEY **rsakey)
{
    // load my private key:
    FILE *pubkey_file = fopen(file_name.c_str(), "r");
    assert(pubkey_file != NULL);
    if (!pubkey_file)
    {
        std::cerr << "Error: cannot open file '" << file_name << "' (missing?)\n";
        return 0;
    }
    *rsakey = PEM_read_PUBKEY(pubkey_file, NULL, NULL, NULL);
    assert(rsakey != NULL);
    if (!rsakey)
    {
        std::cerr << "Error: PEM_read_PrivateKey returned NULL\n";
        return 0;
    }
    fclose(pubkey_file);

    return 1;
}

int 
digestSign(
    unsigned char *msg, size_t msg_len, 
    unsigned char **sig, size_t *sig_len, 
    EVP_PKEY *prvkey, 
    const EVP_MD *cipher
)
{
    short int ret{-1};

    EVP_MD_CTX *sig_ctx = EVP_MD_CTX_new();
    assert(sig_ctx != NULL);
    if(!sig_ctx)
    {
        std::cerr << "Error: EVP_MD_CTX_new returned NULL\n";
        return 0;
    }

    ret = EVP_DigestSignInit(sig_ctx, NULL, cipher, NULL, prvkey);
    assert(ret > 0);
    if (ret == 0)
    {
        std::cerr << "Error: EVP_DigestSignInit returned " << ret << "\n";
        return 0;
    }
    if (ret == -2)
    {
        std::cerr << "Error: EVP_SignInit returned " << ret << "\noperation is not supported by the public key algorithm" << std::endl;
        return 0;
    }

    ret = EVP_DigestSignUpdate(sig_ctx, msg, msg_len);
    assert(ret > 0);
    if (ret == 0)
    {
        std::cerr << "Error: EVP_DigestSignUpdate returned " << ret << "\n";
        return 0;
    }
    if (ret == -2)
    {
        std::cerr << "Error: EVP_DigestSignUpdate returned " << ret << "\noperation is not supported by the public key algorithm" << std::endl;
        return 0;
    }

    /**
     * How does it work?
     * 1) We call the EVP_DigestSignFinal the first time with a NULL sig parameter to obtain the length of the signature. Length is returned in 'req' variable
     * 2) After that, we are going to allocate the required space for the signature buffer (sgnt_buff)
     * At this point, we are ready to effectively write the signature inside the sgnt_buf variable and its correspondence length inside the sgnt_len variable
     * Finally, we are going to compare the value stored in 'req' with the value stored in 'sgnt_len' if they mismatich, then something went wrong, otherwise the signature operation succeed 
     */
    size_t req;
    ret = EVP_DigestSignFinal(sig_ctx, NULL, &req);
    assert(ret > 0);
    if (ret == 0)
    {
        std::cerr << "Error: EVP_DigestSignFinal failed(1) returned " << ret << "\n";
        return 0;
    }
    if (ret == -2)
    {
        std::cerr << "Error: EVP_DigestSignFinal returned " << ret << "\noperation is not supported by the public key algorithm" << std::endl;
        return 0;
    }

    assert(req > 0);
    if (!(req > 0))
    {
        std::cerr << "EVP_DigestSignFinal failed (2), error 0x%lx\n"
                  << ERR_get_error() << std::endl;
        return 0;
    }

    // Allocate signature buffer
    *sig = new unsigned char[req];
    *sig_len = req;

    // Write the signature inside the sgnt_buf
    ret = EVP_DigestSignFinal(sig_ctx, *sig, sig_len);
    assert(ret > 0);
    if (ret != 1)
    {
        std::cerr << "EVP_DigestSignFinal failed (3), return code " << ret << " error 0x%lx\n"
                  << ERR_get_error() << std::endl;
        return 0;
    }
    if (ret == -2)
    {
        std::cerr << "Error: EVP_DigestSignFinal returned " << ret << "\noperation is not supported by the public key algorithm" << std::endl;
        return 0;
    }

    // Compare the written signature length size inside the 'req' variable with the sgnt_len one
    assert(req == *sig_len);
    if (ret != 1)
    {
        std::cerr << "EVP_DigestSignFinal() failed, mismatched signature sizes, (req): " << req << "(sgnt_len): " << sig_len << std::endl;
        return 0;
    }

    // delete the digest and the private key from memory:
    EVP_MD_CTX_free(sig_ctx);
    EVP_PKEY_free(prvkey);

    return 1;
}

int
digestVerify(
    unsigned char *msg, size_t msg_len, 
    unsigned char *sig, size_t sig_len, 
    EVP_PKEY *pubkey, 
    const EVP_MD *cipher
)
{
    short int ret{-1};

    EVP_MD_CTX *vrf_ctx = EVP_MD_CTX_new();
    assert(vrf_ctx != NULL);
    if(!vrf_ctx)
    {
        std::cerr << "Error: EVP_MD_CTX_new returned NULL\n";
        return 0;
    }
    
    ret = EVP_DigestVerifyInit(vrf_ctx, NULL, cipher, NULL, pubkey);
    assert(ret > 0);
    if (ret == 0)
    {
        std::cerr << "Error: EVP_DigestVerifyInit returned " << ret << "\n";
        return 0;
    }
    if (ret == -2)
    {
        std::cerr << "Error: EVP_DigestVerifyInit returned " << ret << "\noperation is not supported by the public key algorithm" << std::endl;
        return 0;
    }

    ret = EVP_DigestVerifyUpdate(vrf_ctx, msg, msg_len);
    assert(ret > 0);
    if (ret == 0)
    {
        std::cerr << "Error: EVP_DigestVerifyUpdate returned " << ret << "\n";
        return 0;
    }
    if (ret == -2)
    {
        std::cerr << "Error: EVP_DigestVerifyUpdate returned " << ret << "\noperation is not supported by the public key algorithm" << std::endl;
        return 0;
    }

    // Write the signature inside the sgnt_buf
    ret = EVP_DigestVerifyFinal(vrf_ctx, sig, sig_len);
    assert(ret > 0);
    if (ret != 1)
    {
        std::cerr << "EVP_DigestVerifyFinal failed (3), return code " << ret << " error 0x%lx\n"
                  << ERR_get_error() << std::endl;
        return 0;
    }
    if (ret == -2)
    {
        std::cerr << "Error: EVP_DigestVerifyFinal returned " << ret << "\noperation is not supported by the public key algorithm" << std::endl;
        return 0;
    }

    // delete the digest and the private key from memory:
    EVP_MD_CTX_free(vrf_ctx);
    EVP_PKEY_free(pubkey);

    return 1;
}