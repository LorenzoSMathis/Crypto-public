# OpenSSL - Symmetric Encryption

> ***Library***: `#include <openssl/evp.h>`

OpenSSL implements symmetric encryption with incremental API (imcremental functions): this means that there is an encryption context and it manages the state of the encryption process (with incremental updates).

## Pseudo Code for Encryption and Decryption

> ---
> ***Encryption***
> ```java
>   ctx = context_initialize(encrypt, cipher, mode, key, iv, ...);
>   
>   cycle:
>       ciphertext_fragment = encrypt_update(ctx, plaintext_fragment);
>   end
>
>   ciphertext = encrypt_finalize(ctx);
> ```
> ---

> ---
> ***Decryption***
> ```java
>   ctx = context_initialize(decrypt, cipher, mode, key, iv, ...);
>
>   cycle:
>       plaintext_fragment = decrypt_update(ctx, ciphertext_fragment);
>   end
>
>   plaintext = decrypt_finalize(ctx);
> ```
> ---

## Context: `EVP_CIPHER_CTX`

The context is a data structure implemented by an opaque data type. It contains all the information about the encryption/decryption process.

```c
    EVP_CIPHER_CTX *ctx;

    ctx = EVP_CIPHER_CTX_new(); // allocate a new context

    EVP_CIPHER_CTX_free(ctx); // deallocate the context
```

> ***Note***: Generally, we prefer to use the generic set of function and set the information about the encryption/decryption in the context. So we will use:
> - `EVP_CipherInit`
> - `EVP_CipherUpdate`
> - `EVP_CipherFinal`
> ---

### Initialization

For the inizialization we have three functions:

- `EVP_EncryptInit`: initialize the context for encryption.
- `EVP_DecryptInit`: initialize the context for decryption.
- `EVP_CipherInit`: initialize the context for encryption or decryption, the choice between encryption or decryption is made by the `enc` parameter.

There are two versions for each function, one `EVP_XXX_ex` defined in OpenSSL 1.1.1 and one `EVP_XXX_ex2` defined in OpenSSL 3.0.0.

### Encryption/Decryption

For the encryption/decryption we have three functions:

- `EVP_EncryptUpdate`: update the context with a new plaintext fragment and return the corresponding ciphertext fragment.
- `EVP_DecryptUpdate`: update the context with a new ciphertext fragment and return the corresponding plaintext fragment.
- `EVP_CipherUpdate`: update the context with a new plaintext or ciphertext fragment and return the corresponding plaintext or ciphertext fragment, the choice between encryption or decryption is made according to the context.

There are two versions for each function, one `EVP_XXX_ex` defined in OpenSSL 1.1.1 and one `EVP_XXX_ex2` defined in OpenSSL 3.0.0.

> ***Note***: if the context is initialized with a specific function (for example `EVP_EncryptInit`), the update function must be the same operation (for example `EVP_EncryptUpdate`).

### Finalization

For the finalization we have three functions:

- `EVP_EncryptFinal`: finalize the encryption process and return the last ciphertext fragment.
- `EVP_DecryptFinal`: finalize the decryption process and return the last plaintext fragment.
- `EVP_CipherFinal`: finalize the encryption or decryption process and return the last plaintext or ciphertext fragment, the choice between encryption or decryption is made according to the context.

There are two versions for each function, one `EVP_XXX_ex` defined in OpenSSL 1.1.1 and one `EVP_XXX_ex2` defined in OpenSSL 3.0.0.

> ***Note***: if the context is initialized with a specific function (for example `EVP_EncryptInit`), the finalization function must be the same operation (for example `EVP_EncryptFinal`).

## Ciphers

The ciphers are defined in 'objects' of type `EVP_CIPHER`. The library provides a set of predefined ciphers, but it's possible to define a custom cipher.

The get the built-in ciphers we can use functions like: `EVP_cipher_mode()`, the function returns a pointer to the `EVP_CIPHER` object.

> ***Tip***: to obtain fastly the name of the function you can run `openssl enc -ciphers` in the terminal, it will print the list of the available ciphers. Now the functin name is `EVP_<cipher>_<mode>`.
>
> Example: `-aes-128-cbc` -> `EVP_aes_128_cbc()`.

---
> ***To see some example see the codes in `./codes/openssl/symmetric_encryption`***

- [Simple Encryption](../../codes/openssl/symmetric_encryption/encryption_1.c)

- [Simple Decryption](../../codes/openssl/symmetric_encryption/decryption_1.c)

---
