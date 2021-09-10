#include "../include/dh.hpp"

int
initDHparams(EVP_PKEY *dh_key, DH *dh_params)
{
    /* Load Diffie-Hellman parameters in dh_key */
    if (NULL == (dh_key = EVP_PKEY_new()))
    {
        std::cerr << "Error setting up DH parameters";
        return 0;
    }

    dh_params = DH_get_2048_224();
    if (1 != EVP_PKEY_set1_DH(dh_key, dh_params))
    {
        std::cerr << "EVP_PKEY_set1_DH failed (fail to set the dh key)";
        return 0;
    }

    return 1;
}

int 
setupDHparams(EVP_PKEY *dh_key, DH *dh_params)
{
    /* Load Diffie-Hellman parameters in dh_key */
    if (NULL == (dh_key = EVP_PKEY_new()))
    {
        std::cerr << "Error setting up DH parameters";
        return 0;
    }

    if (1 != EVP_PKEY_set1_DH(dh_key, dh_params))
    {
        std::cerr << "EVP_PKEY_set1_DH failed (fail to set the dh key)";
        return 0;
    }

    return 1;
}

int
initDHctx(EVP_PKEY_CTX *dh_ctx, EVP_PKEY *dh_key)
{
    /* Create context for the key generation */
    if (!(dh_ctx = EVP_PKEY_CTX_new(dh_key, NULL)))
    {
        std::cerr << "DH Context Initialization failed!";
        return 0;
    }

    return 1;
}

int 
initDHKey(EVP_PKEY *my_dhkey, EVP_PKEY_CTX *dh_ctx)
{
    /* Generation of private/public key pair */
    if (1 != EVP_PKEY_keygen_init(dh_ctx))
    {
        std::cerr << "EVP_PKEY_keygen_init() failed";
        return 0;
    }
    
    /* Store inside EVP_PKEY variable */
    if (1 != EVP_PKEY_keygen(dh_ctx, &my_dhkey))
    {
        std::cerr << "EVP_PKEY_keygen() failed generating DH Keys";
        return 0;
    }

    return 1;
}   