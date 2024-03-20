# OpenSSL - Random Numbers

> ***Pseudo Random Number Generator***: a PRNG is a deterministic algorithm that generates a sequence of unpredictable numbers.

> ***Library***: `#include <openssl/rand.h>`

OpenSSL/RAND library provides a **cryptographically-secure PRNG**. The function is a DBRG (Deterministic Bit Generator), the quality of the output depends on the quality of the seed used for the initialization.

> ***Important***: if two generator uses the same seed, they will generate the same sequence of numbers. **The seed must be truly random** (hardware generators or other sources of entropy).

> **/dev/random** and especially **rand()** (function of C standard library) are not cryptographically secure.\

## PRNG of OpenSSL

OpenSSL has three PRNG:

1. Public PRNG: the main one, this is the only PRNG that can be used by the user for its applications.

2. Private PRNG: used by the library to generate random numbers for internal use.

3. Primary PRNG: used to seed the other two PRNG. 

> ***Note***: why two PRNG? A public PRNG can generate a very high amount of random number, this expose the PRNG to studies and attacks.

## Functions

```c
int RAND_bytes(unsigned char *buf, int num);
```

This function generates `num` random bytes and stores them in `buf` from the public PRNG.

---

```c
int RAND_priv_bytes(unsigned char *buf, int num);
```

This function generates `num` random bytes and stores them in `buf` from the private PRNG.

---

```c
int RAND_load_file(const char *file, long max_bytes);
```

Normally the primary PRNG is seeded from `/dev/urandom` or `/dev/random`. This function allows to seed the primary PRNG from a file.

> ***Note***: the "file" is a source of entropy, it can be a **HRNG** (Hardware Random Number Generator) or **TRNG** (True Random Number Generator).

### Example

```c

int rc = RAND_load_file("/dev/random", 32);

if (rc != 32) {
    /* Failed */
}
```

## Alternative libraries

- [libsodium](https://libsodium.gitbook.io/doc/) [code](https://github.com/jedisct1/libsodium/blob/master/src/libsodium/randombytes/sysrandom/randombytes_sysrandom.c)
