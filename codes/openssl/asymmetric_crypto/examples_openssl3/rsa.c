#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#define KEY_SIZE 2048

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

void handleErrorsCustom(char *msg)
{
    fprintf(stderr, "%s", msg);
    abort();
}

int main(void)
{
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    // Generate RSA keypair
    EVP_PKEY* rsa_keypair = EVP_RSA_gen(KEY_SIZE);
    if (rsa_keypair == NULL) handleErrorsCustom("Error generating RSA keypair");

    // Write RSA keypair to file
    FILE* fp;
    if ( (fp = fopen("private.pem", "wb")) == NULL) handleErrorsCustom("Error opening private.pem file for writing");
    if (PEM_write_PrivateKey(fp, rsa_keypair, NULL, NULL, 0, NULL, NULL) == 0) handleErrors();
    fclose(fp);

    // Write RSA public key to file
    if ( (fp = fopen("public.pem", "wb")) == NULL) handleErrorsCustom("Error opening public.pem file for writing");
    if (PEM_write_PUBKEY(fp, rsa_keypair) == 0) handleErrors();
    fclose(fp);

    // Allocate the context for the encryption operation.
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(rsa_keypair, NULL);
    if (ctx == NULL) handleErrors();

    // Initialize the context for encryption
    if (EVP_PKEY_encrypt_init(ctx) <= 0) handleErrors();

    // Set the padding type to RSA_PKCS1_OAEP_PADDING
    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) handleErrors();

    // Encrypt plaintext
    unsigned char msg[] = "Hello World!\n";

    // Determine buffer length
    size_t enc_len;
    if (EVP_PKEY_encrypt(ctx, NULL, &enc_len, msg, strlen(msg)) <= 0) handleErrors();

    // Encrypt
    unsigned char enc[enc_len];
    if (EVP_PKEY_encrypt(ctx, enc, &enc_len, msg, strlen(msg)) <= 0) handleErrors();

    // Allocate the context for the decryption operation.
    EVP_PKEY_CTX* ctx2 = EVP_PKEY_CTX_new(rsa_keypair, NULL);
    if (ctx2 == NULL) handleErrors();

    // Initialize the context for decryption
    if (EVP_PKEY_decrypt_init(ctx2) <= 0) handleErrors();

    // Set the padding type to RSA_PKCS1_OAEP_PADDING
    if (EVP_PKEY_CTX_set_rsa_padding(ctx2, RSA_PKCS1_OAEP_PADDING) <= 0) handleErrors();

    // Determine buffer length
    size_t dec_len;
    if (EVP_PKEY_decrypt(ctx2, NULL, &dec_len, enc, enc_len) <= 0) handleErrors();

    // Decrypt
    unsigned char dec[dec_len];
    if (EVP_PKEY_decrypt(ctx2, dec, &dec_len, enc, enc_len) <= 0) handleErrors();

    // Print decrypted message
    printf("Original message: %s\n", msg);
    printf("Encrypted message: ");
    for (int i = 0; i < enc_len; i++) printf("%02x", enc[i]);
    printf("\n\n");
    printf("Decrypted message: %s\n", dec);

    // Store to file encrypted message
    FILE *ofile;
    if ( (ofile = fopen("enc", "wb")) == NULL) handleErrorsCustom("Failed to open file");

    if (fwrite(enc, 1, enc_len, ofile) < enc_len) handleErrorsCustom("Failed to write to file");

    fclose(ofile);

    // Clean up
    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_CTX_free(ctx2);
    EVP_PKEY_free(rsa_keypair);

    ERR_free_strings();
    CRYPTO_cleanup_all_ex_data();
    return 0;
}