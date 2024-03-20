#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/err.h>

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

    EVP_MD_CTX* mdctx;

    mdctx = EVP_MD_CTX_new();
    if (mdctx == NULL) handleErrorsCustom("Failed allocate the context.");

    // Get the hash algorithm
    const EVP_MD* md = EVP_get_digestbyname(argv[2]);
    if (md == NULL) handleErrorsCustom("Failed to get the hash algorithm.");

    // Initialize the hashing context
    if (!EVP_DigestInit(mdctx, (EVP_MD*)md)) handleErrors();

    unsigned char buffer[BUFSIZE];
    int readBytes = 0;

    // Update the hashing context with the message read from the file 
    while ( (readBytes = fread(buffer, 1, BUFSIZE, ifile)) > 0)
        if (!EVP_DigestUpdate(mdctx, buffer, readBytes)) handleErrors();
    
    unsigned char md_value[EVP_MD_size(md)];
    int md_length;

    // Finalize the hashing context and obtain the digest value
    if (!EVP_DigestFinal(mdctx, md_value, &md_length)) handleErrors();

    // Deallocate the context
    EVP_MD_CTX_free(mdctx);
    EVP_MD_free((EVP_MD*)md);

    // Print out the digest value
    printf("The digest is: ");
    for (int i = 0; i < md_length; i++)
        printf("%02x", md_value[i]);

    printf("\n");

    // Release the file and the other resources
    fclose(ifile);
    ERR_free_strings();
    CRYPTO_cleanup_all_ex_data();

    return 0;
}