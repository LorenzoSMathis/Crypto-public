/**
 * The specification of the CRAZY protocol includes the following operations:
 * 
 * 1. Generate two strong random 128-bit integers, name them rand1 and rand2
 * 
 * 2. Obtain the first key as
 * k1 = (rand1 + rand2) * (rand1 - rand2) mod 2^128
 * 
 * 3. Obtain the second key as
 * k2 = (rand1 * rand2) / (rand1 - rand2) mod 2^128
 * 
 * 4. Encrypt k2 using k1 using a strong encryption algorithm (and mode) of your choice
 * call it enc_k2.
 * 
 * 5. Generate an RSA keypair with a 2048 bit modulus.
 * 
 * 6. Encrypt enc_k2 using the just generated RSA key.
 * 
 * Implement in C the protocol steps described above, make the proper decisions when
 * the protocol omits information.
 * 
 **/

#include <stdio.h>
#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

// 128 bits -> 16 bytes
#define RAND_SIZE 16
#define PRNG "/dev/random"

#define ENCRYPT 1
#define DECRYPT 0

#define RSA_SIZE 2048

void handleError()
{
    ERR_print_errors_fp(stderr);
    abort();
}


void handleErrorWMessage(char* message)
{
    fprintf(stderr, "%s\n", message);
    abort();
}

int main()
{
    unsigned char rand1[RAND_SIZE];
    unsigned char rand2[RAND_SIZE];

// 1
    // Setup PRNG
    if (RAND_load_file(PRNG, 64) != 64) handleError();

    if (!RAND_bytes(rand1, RAND_SIZE)) handleError();
    if (!RAND_bytes(rand2, RAND_SIZE)) handleError();

// Setup for 2 and 3
    BIGNUM* rand1bn = BN_bin2bn(rand1, RAND_SIZE, NULL);
    if (rand1bn == NULL) handleErrorWMessage("Failed to create rand1");
    BIGNUM* rand2bn = BN_bin2bn(rand2, RAND_SIZE, NULL);
    if (rand2bn == NULL) handleErrorWMessage("Failed to create rand2");

    BIGNUM* k1 = BN_new();
    if (k1 == NULL) handleErrorWMessage("Failed to create k1");
    BIGNUM* k2 = BN_new();
    if (k2 == NULL) handleErrorWMessage("Failed to create k2");
    BIGNUM* tmp = BN_new();
    if (tmp == NULL) handleErrorWMessage("Failed to create tmp");

    BIGNUM* m = BN_new();
    if (m == NULL) handleErrorWMessage("Failed to create m");
    char mhex = "10000000000000000000000000000000"; // hex(2^128)
    BN_hex2bn(&m, mhex);

    BN_CTX* bn_ctx = BN_CTX_new();
    if (bn_ctx == NULL) handleErrorWMessage("Failed to create the bignum context");

// 2
    BN_mod_add(k1, rand1, rand2, m, bn_ctx);
    BN_mod_sub(tmp, rand1, rand2, m, bn_ctx);
    BN_mod_mul(k1, k1, tmp, m, bn_ctx);

// 3
    BN_mod_mul(k2, rand1, rand2, m, bn_ctx);
    // tmp still rand1 - rand2
    BN_mod_inverse(tmp, tmp, m, bn_ctx);
    BN_mod_mul(k2, k2, tmp, m, bn_ctx);

// Setup for 4
    unsigned char iv[RAND_SIZE];
    if (!RAND_bytes(iv, RAND_SIZE)) handleError();

    unsigned char key[BN_num_bytes(k1)];        // It is mod 2^128, so it it a 128 bit number
    BN_bn2bin(k1, key);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!EVP_CipherInit(ctx, EVP_aes_128_cbc(), key, iv, ENCRYPT)) handleError();

    int enc_len;
    int full_len = 0;
    unsigned char enc_k2[BN_num_bytes(k2) + EVP_CIPHER_CTX_block_size(ctx)];

    int k2_bin_s = BN_num_bytes(k2);
    unsigned char k2_bin[k2_bin_s];
    BN_bn2bin(k2, k2_bin);

// 4
    if (!EVP_CipherUpdate(ctx, enc_k2, &enc_len, k2_bin, k2_bin_s)) handleError();
    full_len += enc_len;

    if (!EVP_CipherFinal(ctx, enc_k2 + full_len, &enc_len)) handleError();
    full_len += enc_len;

// 5
    RSA* kpair = NULL;
    BIGNUM* bne = BN_new();
    if (bne == NULL) handleErrorWMessage("Failed to create bne");
    if (!BN_set_word(bne, RSA_F4)) handleError();   // e = 65537

    kpair = RSA_new();
    if (kpair == NULL) handleErrorWMessage("Failed to create kpair");
    if (!RSA_generate_key_ex(kpair, RSA_SIZE, bne, NULL)) handleError();

// 6
    int asym_enc_len;
    unsigned char asym_enc_data[RSA_size(kpair)];

    if ((asym_enc_len = RSA_public_encrypt(full_len, enc_k2, asym_enc_data, kpair, RSA_PKCS1_OAEP_PADDING))) handleError();

}