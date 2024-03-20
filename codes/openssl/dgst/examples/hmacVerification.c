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

    // Key
    unsigned char key[] = "0123456789abcdef";                                           // ASCII characters
    EVP_PKEY* hmac_key = EVP_PKEY_new_mac_key(EVP_PKEY_HMAC, NULL, key, strlen(key));   // hmac key structure

    // Allocate the context
    EVP_MD_CTX* hmac_ctx = EVP_MD_CTX_new(); 
    if (hmac_ctx == NULL) handleErrorsCustom("Failed allocate the context.");

    // Initialize the hashing context
    if (!EVP_DigestSignInit(hmac_ctx, NULL, md, NULL, hmac_key)) handleErrors();

    unsigned char buffer[BUFSIZE];
    int readBytes = 0;

    // Update the hashing context with the message read from the file
    while ( (readBytes = fread(buffer, 1, BUFSIZE, ifile)) > 0)
        if (!EVP_DigestSignUpdate(hmac_ctx, buffer, readBytes)) handleErrors();

    // Finalize the hashing context and obtain the digest value
    unsigned char hmac_value[4096];
    size_t hmac_length;

    if (!EVP_DigestSignFinal(hmac_ctx, hmac_value, &hmac_length)) handleErrors();
    
    // Deallocate the context
    EVP_MD_CTX_free(hmac_ctx);
    EVP_MD_free((EVP_MD*)md);

    // Print out the digest value
    printf("The HMAC is: ");
    for (int i = 0; i < hmac_length; i++)
        printf("%02x", hmac_value[i]);

    printf("\n");

    // VERIFICATION

    unsigned char hmac_value2[] = "c8803dbd2dc6791f525deda322a445c8e70564deaefe30731a8003ecbda6be08a82d258c654e87857e86733aae0c35cbc78ba86189c8c4ee9b3c1aea6a341303";
    unsigned char hmac_valueBin[strlen(hmac_value2)/2];

    for (int i = 0; i < strlen(hmac_value2)/2; i++) sscanf(hmac_value2 + 2*i, "%2hhx", hmac_valueBin + i);

    // Check the length of the digest value and compare the two digest values
    if ( (hmac_length == strlen(hmac_value2)/2) && (CRYPTO_memcmp(hmac_valueBin, hmac_value, hmac_length) == 0) )
        printf("The HMAC is verified.\n");
    else
        printf("The HMAC is not verified.\n");

    

    // Release the file and the other resources
    fclose(ifile);
    ERR_free_strings();
    CRYPTO_cleanup_all_ex_data();

    return 0;
}