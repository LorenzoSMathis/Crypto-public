/**
 * Alice wants to confidentially send Bob the content of a 1MB file through an insecure
 * channel.
 * 
 * Write a program in C, using the OpenSSL library, which Alice can execute to send
 * Bob the file.
 * 
 * Assume that:
 * - Bob's public key is stored into the RSA *bob_pubkey data structure;
 * - The file to send is available in the FILE *file_in data structure;
 * - Alice cannot establish TLS channels or resort to other protocols 
 * - You have access to a high-level communication primitive that sends and receives data
 * and probably format them (e.g., based on a BIO), so that you don't have to think about
 * the communication issues for this exercise
 *
 **/

#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#define FILE_ENC_ALGO EVP_aes_128_cbc()
#define KEY_SIZE 16
#define IV_SIZE 16
#define BUF_SIZE 1024

#define ENCRYPT 1
#define DECRYPT 0

#define PRNG "/dev/random"


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
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    RSA* bob_pubkey = 0xdeadbeaf;       // Some where in memory is stored the public key of Bob
    FILE* file_in = 0xbeafdead;         // Some where in memory there is a file pointer to the input file

    if (file_in == NULL) handleErrorWMessage("Invalid file pointer.");

    // Generate the symmetric key and IV for encryption
    unsigned char key[KEY_SIZE];
    unsigned char iv[IV_SIZE];

    if (RAND_load_file(PRNG, 64) != 64) handleError();      // Init PRNG
    if (!RAND_bytes(key, KEY_SIZE)) handleError();          // Get key random bytes
    if (!RAND_bytes(iv, IV_SIZE)) handleError();            // Get iv random bytes

    // KEK w/ RSA
    int enc_len;
    unsigned char enc_key[RSA_size(bob_pubkey)];

    if ( (enc_len = RSA_public_encrypt(KEY_SIZE, key, enc_key, bob_pubkey, RSA_PKCS1_OAEP_PADDING)) == -1) handleError();

    send_bob(enc_key, enc_len);
    send_bob(iv, IV_SIZE);

    // Setup for symmetric encryption
    EVP_CIPHER_CTX* sctx = EVP_CIPHER_CTX_new();
    if (sctx == NULL) handleErrorWMessage("Impossible to create the context.");
    if (!EVP_CipherInit(sctx, FILE_ENC_ALGO, key, iv, ENCRYPT)) handleError();

    // Buffer for the ciphertext
    int length;
    unsigned char* ct_buffer[BUF_SIZE + EVP_CIPHER_CTX_block_size(sctx)];       // Input size + 1 block

    // Buffer for the plaintext
    int n_read;
    unsigned char pt_buffer[BUF_SIZE];                                          // Input size

    while ( (n_read = fread(pt_buffer, 1, BUF_SIZE, file_in)) > 0)
    {
        // Encrypt
        if (!EVP_CipherUpdate(sctx, ct_buffer, &length, pt_buffer, n_read)) handleError();

        // Send the buffer to Bob
        send_bob(ct_buffer, length);
    }

    // Finalize the encryption
    if (!EVP_CipherFinal(sctx, ct_buffer, &length)) handleError();

    // Send to Bob the last part of the ciphertext
    send_bob(ct_buffer, length);


    // Final cleanup
    RSA_free(bob_pubkey);

    EVP_CIPHER_CTX_free(sctx);
    fclose(file_in);

    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();
}