# OpenSSL - Big Numbers

> ***Library***: `#include <openssl/bn.h>`

## Underlying problem

The standard library of C language provides built in integers with size up to `64bits`, this is not enough for cryptography. If we are working with RSA or, in general, with public key cryptography, we need to work with integers with size up to `2048bits` or more.

The OpenSSL BigNum library provides a data type and a set of functions to work with big numbers. Note that this function are optimized for working with big numbers, they implement specific algorithms for modular arithmetic and exponentiation.

## Data type - BIGNUM

The data type `BIGNUM` is an opaque structure that represents a big number without size upper bound. The structure is define the object/context of the number, the data structure il dynamically allocated.

### Basic Example - Allocation and Deallocation

```c
BIGNUM* bn;

bn = BN_new(); // allocate a new BIGNUM

BN_free(bn); // deallocate the BIGNUM
```

### Example - RSA Key Definition

```c
struct {
    BIGNUM *n; // modulus
    BIGNUM *e; // public exponent
    BIGNUM *d; // private exponent
    BIGNUM *p; // secret prime factor
    BIGNUM *q; // secret prime factor
    BIGNUM *dmp1; // d mod (p-1)
    BIGNUM *dmq1; // d mod (q-1)
    BIGNUM *iqmp; // q^-1 mod p
} RSA;
```

> ***Note***: also the normal numbers, like `e`, are rapresented as `BIGNUM`. This choice is made to uniform the interface and the functions.

## Function

### Copying

It's important to remember that the `BIGNUM` data structure is not a simple data type, but it's a opaque pointer to a dynamically allocated data structure. This means that the variables are pointer, not the actual data.

```c
    BIGNUM *a, *b;

    a = b;      // WRONG, this assign a pointer memorized in a to the variable b. The BIGNUM data is not copied!
```

To copy a `BIGNUM` we need to use the function `BN_copy`:

```c
    BIGNUM *a, *b;

    a = BN_new();
    b = BN_new();

    BN_copy(a, b);  // CORRECT, this copy the data from b to a
```

To clone a `BIGNUM` to a new variable we need to use the function `BN_dup`:

```c
    BIGNUM *a, *b;

    a = BN_new();
    b = BN_dup(a);  // CORRECT, this clone the data from a to b
```

> ***Note***: the `BN_dup` function allocates a new `BIGNUM` and copy the data from the source to the destination. It does the same thing of `BN_new` + `BN_copy`.

### Conversion

When we need to use the numbers stored in a `BIGNUM` outside the library, for example to send them over the network or store them in a file, we need to convert them to a standard data type.

> Full documentation [here](https://www.openssl.org/docs/manmaster/man3/BN_bn2bin.html)

#### Size of BIGNUM

To measure the size of the buffer needed to export a `BIGNUM` we can use the function `BN_num_bytes`:

```c
    BIGNUM *a;
    int size;

    a = BN_new();

    size = BN_num_bytes(a); // this return the size of the buffer needed to export the BIGNUM
```

#### Exporting BIGNUM (BIGNUM -> bin|dec|hex)

To export a `BIGNUM` to a buffer we can use the function `BN_bn2bin`:

```c
int BN_bn2bin(const BIGNUM *a, unsigned char *to);  // returns the number of bytes written
```

```c
    BIGNUM *a;
    unsigned char *buf;

    a = BN_new();

    buf = malloc(BN_num_bytes(a)); // allocate the buffer

    BN_bn2bin(a, buf); // export the BIGNUM to the buffer
```

To export a `BIGNUM` to a decimal string we can use the function `BN_bn2dec`:

```c
char *BN_bn2dec(const BIGNUM *a);  // returns a string
```

```c
    BIGNUM *a;
    char *buf;

    a = BN_new();

    buf = BN_bn2dec(a); // export the BIGNUM to the buffer
```

To export a `BIGNUM` to a hexadecimal string we can use the function `BN_bn2hex`:

```c
char *BN_bn2hex(const BIGNUM *a); // returns a string
```

```c
    BIGNUM *a;
    char *buf;

    a = BN_new();

    buf = BN_bn2hex(a); // export the BIGNUM to the buffer
```

#### Importing BIGNUM (bin|dec|hex -> BIGNUM)

To import a `BIGNUM` from a buffer we can use the function `BN_bin2bn`:

```c
BIGNUM *BN_bin2bn(const unsigned char *s, int len, BIGNUM *ret); // returns a BIGNUM
```

```c
    unsigned char *buf;
    BIGNUM *a;

    buf = malloc(32); // allocate the buffer

    a = BN_bin2bn(buf, 32, NULL); // import the BIGNUM from the buffer
```

To import a `BIGNUM` from a decimal string we can use the function `BN_dec2bn`:

```c
int BN_dec2bn(BIGNUM **a, const char *str); // returns the number of bytes written
```

```c
    char *buf;
    BIGNUM *a;

    buf = malloc(32); // allocate the buffer where the string is stored

    BN_dec2bn(&a, buf); // import the BIGNUM from the buffer
            // ^ this is a pointer to a pointer
```

To import a `BIGNUM` from a hexadecimal string we can use the function `BN_hex2bn`:

```c
int BN_hex2bn(BIGNUM **a, const char *str); // returns the number of bytes written
```

```c
    char *buf;
    BIGNUM *a;

    buf = malloc(32); // allocate the buffer where the string is stored

    BN_hex2bn(&a, buf); // import the BIGNUM from the buffer
            // ^ this is a pointer to a pointer
```

### Arithmetic

- ***Addition***: `BN_add`

    ```c
    int BN_add(BIGNUM *r, const BIGNUM *a, const BIGNUM *b); // returns 1 if success, 0 if error
    ```

    ```c
        BIGNUM *a, *b, *c;

        a = BN_new();
        b = BN_new();
        c = BN_new();

        BN_add(c, a, b); // c = a + b
    ```

- ***Modular Exponentiation***: `BN_mod_exp`

    ```c
    int BN_mod_exp(BIGNUM *r, const BIGNUM *a, const BIGNUM *p, const BIGNUM *m, BN_CTX *ctx); // returns 1 if success, 0 if error
    ```

    ```c
        BIGNUM *a, *p, *m, *r;
        BN_CTX *ctx;

        a = BN_new();   // base
        p = BN_new();   // exponent
        m = BN_new();   // modulus
        r = BN_new();   // result
        ctx = BN_CTX_new();

        BN_mod_exp(r, a, p, m, ctx); // r = a^p mod m
    ```

> ***Full list [here](https://www.openssl.org/docs/manmaster/man3/BN_add.html)***

#### The context - `BN_CTX`

The context is a data structure that contains temporary variables used by the library. Some functions need this additional memory to work properly, a example is the modular exponentiation but a lot of other functions need it.

```c
    BN_CTX *ctx;

    ctx = BN_CTX_new(); // allocate a new context

    BN_CTX_free(ctx); // deallocate the context
```

### Tests and logical operations

- ***Comparison***: `BN_cmp`

    ```c
    int BN_cmp(const BIGNUM *a, const BIGNUM *b); // returns 0 if equal, 1 if a > b, -1 if a < b
    ```

    ```c
        BIGNUM *a, *b;
        int res;

        a = BN_new();
        b = BN_new();

        res = BN_cmp(a, b); // res = 0 if a == b, res = 1 if a > b, res = -1 if a < b
    ```

- ***Test if zero***: `BN_is_zero`

    ```c
    int BN_is_zero(const BIGNUM *a); // returns 1 if a == 0, 0 if a != 0
    ```

    ```c
        BIGNUM *a;
        int res;

        a = BN_new();

        res = BN_is_zero(a); // res = 1 if a == 0, res = 0 if a != 0
    ```

- ***Test if the conversion of BN in C int is possible***: `BN_is_word`

    ```c
    int BN_is_word(const BIGNUM *a, BN_ULONG w); // returns 1 if a == w, 0 if a != w
    ```

    ```c
        BIGNUM *a;
        int res;

        a = BN_new();

        res = BN_is_word(a, 0); // res = 1 if a == 0, res = 0 if a != 0
    ```

> ***Full list [here](https://www.openssl.org/docs/manmaster/man3/BN_cmp.html)***

### Printing to output

- ***Printing to file pointer***: `BN_print_fp`

    ```c
    int BN_print_fp(FILE *fp, const BIGNUM *a); // returns 1 if success, 0 if error
    ```

    ```c
        BIGNUM *a;
        FILE *fp;

        a = BN_new();
        fp = fopen("file.txt", "w");

        BN_print_fp(fp, a); // print the BIGNUM to the file
    ```

### Full list of resources

- [Arithmetic operations](https://www.openssl.org/docs/manmaster/man3/BN_add.html)
- [Bit operation](https://www.openssl.org/docs/manmaster/man3/BN_lshift.html)
- [Comparision](https://www.openssl.org/docs/manmaster/man3/BN_cmd.html)
- [Random numbers](https://www.openssl.org/docs/manmaster/man3/BN_rand.html)
- [Prime numbers](https://www.openssl.org/docs/manmaster/man3/BN_generate_prime.html)
