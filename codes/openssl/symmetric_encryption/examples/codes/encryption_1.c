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

    // Plaintext
    unsigned char plaintext[44] = "This variable contains the data to encrypt.";  // 43 chars + '\0' terminator
    
    // Ciphertext
    unsigned char ciphertext[48];
    int ciphertext_len = 0;

    int length;


    // Create a new cipher context (not initialized)
    ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL) {
        fprintf(stderr, "Error creating cipher context\n");
        abort();
    }
        

    // Initialize the cipher context (0 on failure)
    if (!EVP_CipherInit(ctx, EVP_aes_128_cbc(), key, iv, ENCRYPT)) 
        handleErrors();

    // Encrypt plaintext (0 on failure)
    if (!EVP_CipherUpdate(ctx, ciphertext, &length, plaintext, strlen(plaintext))) 
        handleErrors();

    fprintf(stdout, "After the update: %d bytes processed\n", length);

    ciphertext_len += length;

    // Finalize the encryption
    if (!EVP_CipherFinal(ctx, ciphertext + ciphertext_len, &length))
        handleErrors();

    fprintf(stdout, "After the final: %d bytes processed\n", length);
    
    ciphertext_len += length;
    fprintf(stdout, "Ciphertext length: %d\n", ciphertext_len);

    // Print ciphertext
    fprintf(stdout, "Ciphertext: ");
    for (int i = 0; i < ciphertext_len; i++) {
        fprintf(stdout, "%02x", ciphertext[i]);
    }
    fprintf(stdout, "\n");

    // Deallocate all
    EVP_CIPHER_CTX_free(ctx);
    ERR_free_strings();
    CRYPTO_cleanup_all_ex_data();

    return 0;
}