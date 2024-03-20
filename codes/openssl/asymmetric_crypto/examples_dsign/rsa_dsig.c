#include <stdio.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#define BUFSIZE 512

void handleErrors()
{
    ERR_print_errors_fp(stderr);
    abort();
}

void handleErrorsCustom(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    abort();
}

int main(int argc, char *argv[])
{
    // Some initializations
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    // Check command line arguments
    if (argc != 3) handleErrorsCustom("Usage: ./rsa_dsig <ifile> <private_key>");

    FILE* ifile;
    FILE* priv_keyfile;

    // Open files
    if ( (ifile = fopen(argv[1], "rb")) == NULL) handleErrorsCustom("Error opening input file");
    if ( (priv_keyfile = fopen(argv[2], "rb")) == NULL) handleErrorsCustom("Error opening private key file");

    // DigestSign --> EVP_PKEY
    EVP_PKEY* priv_key = PEM_read_PrivateKey(priv_keyfile, NULL, NULL, NULL);

    // Allocate the context for digest/signature
    EVP_MD_CTX *sig_ctx = EVP_MD_CTX_new();
    if (sig_ctx == NULL) handleErrorsCustom("Error creating context for signature");

    // Initialize the context for signing
    if(!EVP_DigestSignInit(sig_ctx, NULL, EVP_sha256(), NULL, priv_key)) handleErrors();

    // Read the input file and update the digest
    unsigned char buffer[BUFSIZE];
    size_t readBytes;

    while ( (readBytes = fread(buffer, 1, BUFSIZE, ifile)) != 0)
        if (!EVP_DigestSignUpdate(sig_ctx, buffer, readBytes)) handleErrors();
    
    // Finalize the digest
    size_t dgst_len;
    if (!EVP_DigestSignFinal(sig_ctx, NULL, &dgst_len)) handleErrors();

    // Get the signature
    size_t sig_len;
    unsigned char sig[dgst_len];
    if (!EVP_DigestSignFinal(sig_ctx, sig, &sig_len)) handleErrors();
    
    // Write the signature to file
    FILE* ofile;
    if ( (ofile = fopen("signature.sig", "wb")) == NULL) handleErrorsCustom("Error opening output file");

    if (fwrite(sig, 1, sig_len, ofile) < sig_len) handleErrorsCustom("Error writing signature to file");

    // Release resources
    EVP_MD_CTX_free(sig_ctx);
    EVP_PKEY_free(priv_key);
    fclose(ifile);
    fclose(priv_keyfile);
    fclose(ofile);

    // Final cleanup
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();
    return 0;
}