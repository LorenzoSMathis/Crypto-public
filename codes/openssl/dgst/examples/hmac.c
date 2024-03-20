#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/hmac.h>

#define BUFSIZE 128

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

void handleErrorsCustom(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    abort();
}

int main(int argc, char** argv)
{
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    if (argc != 3) handleErrorsCustom("Usage: hashFile <file> <algorithm>");

    FILE *ifile;
    if ((ifile = fopen(argv[1], "rb")) == NULL) handleErrorsCustom("Failed to open the file");

    // Get the hash algorithm
    const EVP_MD* md = EVP_get_digestbyname(argv[2]);
    if (md == NULL) handleErrorsCustom("Failed to get the hash algorithm.");

    // Key (16 ascii characters)
    unsigned char key[] = "0123456789abcdef";           // ASCII characters

    // Allocate the context
    HMAC_CTX* hmac_ctx = HMAC_CTX_new();
    if (hmac_ctx == NULL) handleErrorsCustom("Failed allocate the context.");

    // Initialize the hashing context
    if (!HMAC_Init_ex(hmac_ctx, key, strlen(key), md, NULL)) handleErrors();

    unsigned char buffer[BUFSIZE];
    int readBytes = 0;

    // Update the hashing context with the message read from the file
    while ( (readBytes = fread(buffer, 1, BUFSIZE, ifile)) > 0)
        if (!HMAC_Update(hmac_ctx, buffer, readBytes)) handleErrors();

    // Finalize the hashing context and obtain the digest value
    unsigned char hmac_value[HMAC_size(hmac_ctx)];
    int hmac_length;

    if (!HMAC_Final(hmac_ctx, hmac_value, &hmac_length)) handleErrors();
    
    // Deallocate the context
    HMAC_CTX_free(hmac_ctx);
    EVP_MD_free((EVP_MD*)md);

    // Print out the digest value
    printf("The HMAC is: ");
    for (int i = 0; i < hmac_length; i++)
        printf("%02x", hmac_value[i]);

    printf("\n");

    // Release the file and the other resources
    fclose(ifile);
    ERR_free_strings();
    CRYPTO_cleanup_all_ex_data();

    return 0;
}