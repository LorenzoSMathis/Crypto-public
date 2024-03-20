# OpenSSL - Asymmetric Encryption (RSA)

> ***Library***: `#include <openssl/evp.h>`

The EVP library provides all functions for the key generation with different asymmetric algorithms (RSA, DSA, DH, EC). 

## RSA (OpenSSL 1.1.1)

> ***Library***: `#include <openssl/rsa.h>`

The three main function for RSA encryption are:

- `int RSA_generate_key_ex(RSA *rsa, int bits, BIGNUM *e, BN_GENCB *cb);`: generate a new RSA key pair with `bits` bits and public exponent `e`. The function returns `1` if success, `0` if error.

- `int RSA_public_encrypt(int flen, const unsigned char *from, unsigned char *to, RSA *rsa, int padding);`: encrypt a message with RSA. The function returns the size of the ciphertext if success, `-1` if error.

- `int RSA_private_decrypt(int flen, const unsigned char *from, unsigned char *to, RSA *rsa, int padding);`: decrypt a message with RSA. The function returns the size of the plaintext if success, `-1` if error.

## Public Key Management in OpenSSL 3.0.0

> ***Library***: `#include <openssl/evp.h>`

The approach of OpenSSL 3.0.0 for public key crpytography is completely different from OpenSSL 1.1.1. The library define new data structures and function to manage with more control the keys.

### Data structures

- `EVP_PKEY`: the main data structure that represents a public key. It's an opaque data type.

- `EVP_PKEY_CTX`: the context of the public key. It's an opaque data type.

### Functions

- `EVP_PKEY* EVP_RSA_gen(unsigned int bits)`: generate a new RSA key pair with `bits` bits. The function returns a pointer to the `EVP_PKEY` object if success, `NULL` if error.
- `EVP_PKEY* EVP_PKEY_new(void)`: allocate a new `EVP_PKEY` object. The function returns a pointer to the `EVP_PKEY` object if success, `NULL` if error.
- `EVP_PKEY_CTX* EVP_PKEY_CTX_new_from_name(EVP_PKEY *pkey, const char *name, ENGINE *e)`: allocate a new `EVP_PKEY_CTX` object. The function returns a pointer to the `EVP_PKEY_CTX` object if success, `NULL` if error.
- `int EVP_PKEY_keygen_init(EVP_PKEY_CTX *ctx)`: initialize the context for key generation. The function returns `1` if success, `0` if error.
- `int EVP_PKEY_generate(EVP_PKEY_CTX *ctx, EVP_PKEY **ppkey)`: generate a new key pair. The function returns `1` if success, `0` if error.

### Example: easy way

```c
    EVP_PKEY *pkey;
    
    pkey = EVP_RSA_gen(2048); // generate a new RSA key pair with 2048 bits

    if (pkey == NULL) {
        /* Error */
    }

    EVP_PKEY_free(pkey); // deallocate the key
```

### Example: advanced way

```c

    EVP_PKEY *pkey = NULL;
    EVP_PKEY_CTX *ctx;

    pkey = EVP_PKEY_new(); // allocate a new EVP_PKEY object

    if (pkey == NULL) {
        /* Error */
    }

    ctx = EVP_PKEY_CTX_new_from_name(NULL, "RSA", NULL); // allocate a new EVP_PKEY_CTX object

    if (ctx == NULL) {
        /* Error */
    }

    EVP_PKEY_keygen_init(ctx); // initialize the context for key generation

    EVP_PKEY_generate(ctx, &pkey); // generate a new key pair

    EVP_PKEY_CTX_free(ctx); // deallocate the context

    EVP_PKEY_free(pkey); // deallocate the key

```

## Encryption and Decryption in OpenSSL 3.0.0

```c
int RSA_public_encrypt(int flen, const unsigned char *from, unsigned char *to, RSA *rsa, int padding);
```

For padding, the library provide some definitions for the most common padding schemes:

- `RSA_PKCS1_PADDING`: PKCS#1 v1.5 padding.

- `RSA_PKCS1_OAEP_PADDING`: PKCS#1 OAEP padding. (**Suggested one**)

- `RSA_SSLV23_PADDING`: SSLv23 padding.

- `RSA_NO_PADDING`: no padding.

```c
int RSA_private_decrypt(int flen, const unsigned char *from, unsigned char *to, RSA *rsa, int padding);
```

## Managing PEM data

> ***Library***: `#include <openssl/pem.h>`

```c
EVP_PKEY_RSA* PEM_read_RSAPrivateKey(FILE *fp, NULL, NULL, NULL);                               // RSA Key
```

```c
EVP_PKEY* PEM_read_RSAPrivateKey(FILE *fp, EVP_PKEY_RSA **x, pem_password_cb *cb, void *u);     // Genenric key
```

```c
EVP_PKEY_RSA* PEM_read_RSAPublicKey(FILE *fp, EVP_PKEY_RSA **x, pem_password_cb *cb, void *u);
```

## Some notes about the signing process

The signing process is implemented by `EVP_DigestSignInit`, `EVP_DigestSignUpdate` and `EVP_DigestSignFinal` functions.

First of all, we need to create and initialize the `EVP_MD_CTX` context with `EVP_MD_CTX_new` and `EVP_DigestSignInit`.

Then we need to update the context with the data to sign with `EVP_DigestSignUpdate`.

Finally we can finalize the signing process with `EVP_DigestSignFinal`:

 - We call the `EVP_DigestSignFinal` function with `NULL` as second parameter to finalize the digest computation: `EVP_DigestSignFinal(ctx, NULL, &dgstlen)`.
 - We call the `EVP_DigestSignFinal` function with a buffer as second parameter to sign the digest and store the digest in the buffer: `EVP_DigestSignFinal(ctx, signature, &dgstlen)`.
