#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/err.h>

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

int main(void)
{
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    char message[] = "This is the message to hash!!!";

    EVP_MD_CTX *mdctx;

    // Allocate the context for hashing
    mdctx = EVP_MD_CTX_new();
    if (mdctx == NULL) handleErrorsCustom("Failed to allocate the context");

    // Initialize the hashing context
    if (1 != EVP_DigestInit(mdctx, EVP_sha1())) handleErrors();

    // Update the hashing context with the message
    if (1 != EVP_DigestUpdate(mdctx, message, strlen(message))) handleErrors();

    unsigned char md_value[EVP_MD_size(EVP_sha1())];
    int md_length;

    // Finalize the hashing context and obtain the digest value
    if (1 != EVP_DigestFinal(mdctx, md_value, &md_length)) handleErrors();

    EVP_MD_CTX_free(mdctx);

    printf("The digest is: ");
    for (int i = 0; i < md_length; i++)
        printf("%02x", md_value[i]);

    printf("\n");

    ERR_free_strings();
    CRYPTO_cleanup_all_ex_data();

    return 0;
}