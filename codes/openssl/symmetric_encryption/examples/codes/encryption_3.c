#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#define ENCRYPT 1
#define DECRYPT 0

#define BUFFER_SIZE_BYTES 64


void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

/*
    argv[1] = plaintext | argv[2] = outfile | argv[3] = key | argv[4] = iv
*/
int main(int argc, char** argv)
{

    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    EVP_CIPHER_CTX* ctx;

    // Check arguments
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <plaintext> <outfile> <key> <iv>\n", argv[0]);
        abort();
    }

    // Open plaintext file
    FILE* fp_in;
    if ((fp_in = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Error opening input file %s\n", argv[1]);
        abort();
    }

    // Open output file
    FILE* fp_out;
    if ((fp_out = fopen(argv[2], "wb")) == NULL) {
        fprintf(stderr, "Error opening output file %s\n", argv[1]);
        abort();
    }

    // Test key length
    if (strlen(argv[3]) != 32) {
        fprintf(stderr, "Key must be 16 bytes long in hex format\n");
        abort();
    }

    // Test IV length
    if (strlen(argv[4]) != 32) {
        fprintf(stderr, "IV must be 16 bytes long in hex format\n");
        abort();
    }

    // AES-128 CBC key and IV from hex to bytes
    unsigned char key[16];
    unsigned char iv[16];

    for (int i = 0; i < 32; i++) sscanf(argv[3] + (2*i), "%2hhx", key + i);
    for (int i = 0; i < 32; i++) sscanf(argv[4] + (2*i), "%2hhx", iv + i);


    // Create a new cipher context (not initialized)
    ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL) {
        fprintf(stderr, "Error creating cipher context\n");
        abort();
    }

    // Initialize the cipher context
    if (!EVP_CipherInit(ctx, EVP_aes_128_cbc(), key, iv, ENCRYPT))
        handleErrors();

    int read_bytes = 0;                             // Bytes read from file
    unsigned char buffer[BUFFER_SIZE_BYTES];        // Buffer to read from file
    unsigned char ciphertext[BUFFER_SIZE_BYTES];    // Buffer to write ciphertext

    int length;                                     // Bytes processed in each update
    int ciphertext_len = 0;                         // Total bytes encrypted

    // Read from file until something is read
    while ( (read_bytes = fread(buffer, 1, BUFFER_SIZE_BYTES, fp_in)) > 0 ) 
    {
        // Encrypt
        if (!EVP_CipherUpdate(ctx, ciphertext, &length, buffer, read_bytes))
            handleErrors();

        ciphertext_len += length;

        if (fwrite(ciphertext, 1, length, fp_out) < length) {
            fprintf(stderr, "Error writing ciphertext to file\n");
            abort();
        }

        // Print round info
        fprintf(stdout, "%d bytes read\n", read_bytes);
        fprintf(stdout, "%d bytes processed\n", length);
        fprintf(stdout, "%d bytes encrypted from start\n", ciphertext_len);
        fprintf(stdout, "\n-----------------------------\n\n");
    }

    // Finalize encryption
    if (!EVP_CipherFinal(ctx, ciphertext, &length))
        handleErrors();

    // Write final ciphertext to file
    if (fwrite(ciphertext, 1, length, fp_out) < length) {
        fprintf(stderr, "Error writing ciphertext to file\n");
        abort();
    }

    ciphertext_len += length;

    // Print final info
    fprintf(stdout, "%d bytes processed\n", length);
    fprintf(stdout, "%d bytes encrypted from start\n", ciphertext_len);
   
    // Deallocate all
    EVP_CIPHER_CTX_free(ctx);
    ERR_free_strings();
    CRYPTO_cleanup_all_ex_data();
    fclose(fp_in);
    fclose(fp_out);

    return 0;
}