# OpenSSL - Digests and MACs

> ***Library***: `#include <openssl/evp.h>`

The approach of OpenSSL for digests and MACs is the same of the symmetric encryption: incremental API. This means that there is a context that manages the state of the process and mainly three functions:

- `InitializeContext`.
- `UpdateContext`.
- `FinalizeContext`.

## Pseudo codes

> ---
> ***Hashing***
> ```java
>   ctx = context_initialize(hash_algo)
>   while (data != NULL):
>       update_context(ctx, data)
>   end
>
>   digest = finalize_context(ctx)
>   output(data, digest)
> ```
> ---

> ---
> ***Verification***
> ```java
>   ctx = context_initialize(hash_algo)
>   while (data_recv != NULL):
>       update_context(ctx, data_recv)
>   end
>
>   digest_computed = finalize_context(ctx)
>   if (cmp(digest_computed, digest_recv) == 0):
>       output("OK")
>   else:
>       output("KO")
>   end
> ```
> ---

## Context: `EVP_MD_CTX`

The context is a data structure implemented by an opaque data type. It contains all the information about the hashing process.

```c
    EVP_MD_CTX *ctx;

    ctx = EVP_MD_CTX_new(); // allocate a new context

    EVP_MD_CTX_free(ctx); // deallocate the context
```

## Initialization

For the inizialization we have two functions:

- `EVP_DigestInit`: initialize the context for hashing.
- `EVP_DigestInit_ex`: initialize the context for hashing, this is the version defined in OpenSSL 1.1.1.

## Update

For the update we have two functions:

- `EVP_DigestUpdate`: update the context with a new data fragment.
- `EVP_DigestUpdate_ex`: update the context with a new data fragment, this is the version defined in OpenSSL 1.1.1.

## Finalization

For the finalization we have two functions:

- `EVP_DigestFinal`: finalize the hashing process and return the digest.
- `EVP_DigestFinal_ex`: finalize the hashing process and return the digest, this is the version defined in OpenSSL 1.1.1.

## Explicit Hash libraries

An explicit API is also available for each hash algorithm, for example:

- `openssl/sha.h`

But they are been deprecated in OpenSSL 3.0.0.

## Useful functions

- `EVP_MD* EVP_<algo_name>(void)`: returns a pointer to the `EVP_MD` object of the algorithm.

- `int EVP_MD_size(EVP_MD*)`: returns the size (in bytes) of the digest of the algorithm.

- `int CRYPTO_memcmp(const void *a, const void *b, size_t len)`: compare two buffers of size `len` in constant time. (**Time safe comparison**)
