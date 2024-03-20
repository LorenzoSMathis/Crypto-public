#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#define ENCRYPT 1
#define DECRYPT 0


void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}


int main(void)
{
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    EVP_CIPHER_CTX* ctx;

    // AES-128 CBC key and IV
    unsigned char key[16] = "0123456789abcdef";
    unsigned char iv[16] = "fedcba9876543210";

    //Ciphertext
    unsigned char ciphertext[97] = "6a5ee7157a0d9d4068d89e35a55a4bbfa03bb8fa7c85c6bc6d3e14d171b7901bd687a863164cefdcbf7fb3f535be362d";
    unsigned char ciphertext_bin[48];
    
    // Ciphertext from hex to bytes
    for (int i = 0; i < strlen(ciphertext)/2; i++) sscanf(ciphertext + (2*i), "%2hhx", ciphertext_bin + i);

    //Plaintext
    unsigned char plaintext[48];
    int plaintext_len = 0;

    int length;

    // Create a new cipher context (not initialized)
    ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL) {
        fprintf(stderr, "Error creating cipher context\n");
        abort();
    }

    // Initialize the cipher context
    if (!EVP_CipherInit(ctx, EVP_aes_128_cbc(), key, iv, DECRYPT))
        handleErrors();

    // Decrypt ciphertext 
    if (!EVP_CipherUpdate(ctx, plaintext, &length, ciphertext_bin, strlen(ciphertext)/2))
        handleErrors();

    fprintf(stdout, "After the update: %d bytes processed\n", length);

    plaintext_len += length;

    // Finalize the decryption
    if(!EVP_CipherFinal(ctx, plaintext + plaintext_len, &length))
        handleErrors();

    fprintf(stdout, "After the final: %d bytes processed\n", length);


    plaintext_len += length;
    plaintext[plaintext_len] = '\0';
    fprintf(stdout, "Plaintext length: %d\n", plaintext_len);

    // Print plaintext
    fprintf(stdout, "Plaintext: %s\n", plaintext);

    // Deallocate all
    EVP_CIPHER_CTX_free(ctx);
    ERR_free_strings();
    CRYPTO_cleanup_all_ex_data();

    return 0;
}