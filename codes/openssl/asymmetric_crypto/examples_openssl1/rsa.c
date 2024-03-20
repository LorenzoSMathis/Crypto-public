#include <stdio.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#define KEY_LENGTH  2048

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

int main()
{
    // Some initializations
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    // Generate BIGNUM for RSA exponent
    BIGNUM* bne = BN_new();
    if (bne == NULL) handleErrorsCustom("Failed to create BIGNUM");
    if (!BN_set_word(bne, RSA_F4)) handleErrors();

    // Generate RSA keypair
    RSA* rsa_keypair = RSA_new();
    if (rsa_keypair == NULL) handleErrorsCustom("Failed to create RSA data structure");
    if (!RSA_generate_key_ex(rsa_keypair, KEY_LENGTH, bne, NULL)) handleErrors();

    // Write RSA private key to file
    FILE *rsa_file;
    if ( (rsa_file = fopen("private.pem", "w")) == NULL) handleErrorsCustom("Failed to open file");

    if (!PEM_write_RSAPrivateKey(rsa_file, rsa_keypair, NULL, NULL, 0, NULL, NULL)) handleErrors();

    fclose(rsa_file);

    // Write RSA public key to file
    if ( (rsa_file = fopen("public.pem", "w")) == NULL) handleErrorsCustom("Failed to open file");

    if (!PEM_write_RSAPublicKey(rsa_file, rsa_keypair)) handleErrors();

    fclose(rsa_file);

    // Encrypt message
    unsigned char* msg = "Hello World!\n";
    unsigned char* enc[RSA_size(rsa_keypair)];
    int enc_len;
    
    enc_len = RSA_public_encrypt(strlen(msg)+1, msg, enc, rsa_keypair, RSA_PKCS1_OAEP_PADDING);
    if (enc_len == -1) handleErrors();

    // Decrypt message
    unsigned char dec[RSA_size(rsa_keypair)];
    int dec_len;

    dec_len = RSA_private_decrypt(enc_len, enc, dec, rsa_keypair, RSA_PKCS1_OAEP_PADDING);
    if (dec_len == -1) handleErrors();

    FILE* ofile;
    if ( (ofile = fopen("enc", "wb")) == NULL) handleErrorsCustom("Failed to open file");
    if (fwrite(enc, 1, enc_len, ofile) != enc_len) handleErrorsCustom("Failed to write to file");
    fclose(ofile);

    // Release memory
    RSA_free(rsa_keypair);

    // Final cleanup
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();
    return 0;
}