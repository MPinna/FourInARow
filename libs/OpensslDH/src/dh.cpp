#include "../include/dh.hpp"

int setupDHctx(EVP_PKEY *dh_params, DH *temp_params, EVP_PKEY_CTX *dh_ctx)
{
    /* Load Diffie-Hellman parameters in dh_params */
    if (NULL == (dh_params = EVP_PKEY_new()))
    {
        std::cerr << "Error setting up DH parameters" << std::endl;
        return 0;
    }

    temp_params = DH_get_2048_224();
    if (1 != EVP_PKEY_set1_DH(dh_params, temp_params))
    {
        std::cerr << "EVP_PKEY_set1_DH failed (fail to set the dh key)" << std::endl;
        return 0;
    }

    /* Create context for the key generation */
    EVP_PKEY_CTX *dh_ctx;
    if (!(dh_ctx = EVP_PKEY_CTX_new(dh_params, NULL)))
    {
        std::cerr << "DH Context failed (error during context creation)" << std::endl;
        return 0;
    }

    return 1;
}

int setupDHKey(EVP_PKEY *my_dhkey, EVP_PKEY_CTX *dh_ctx)
{
    /* Generation of private/public key pair */
    if (1 != EVP_PKEY_keygen_init(dh_ctx))
    {
        std::cerr << "EVP_PKEY_keygen_init() failed" << std::endl;
        return 0;
    }
    
    /* Store inside EVP_PKEY variable */
    if (1 != EVP_PKEY_keygen(dh_ctx, &my_dhkey))
    {
        std::cerr << "EVP_PKEY_keygen() failed generating DH Keys" << std::endl;
        return 0;
    }

    return 1;
}   