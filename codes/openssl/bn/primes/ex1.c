#include <stdio.h>
#include <openssl/bn.h>
#include <openssl/err.h>

#define MIN_SIZE 1024

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

int main(void)
{
    BIGNUM* prime1 = BN_new();
    BIGNUM* prime2 = BN_new();
    BN_CTX* ctx = BN_CTX_new();

    /*
        int BN_generate_prime_ex(BIGNUM* ret, int size, int safe, const BIGNUM* add, const BIGNUM* rem, BN_GENCB* cb);
        
            ret: Where the prime number will be stored.
            size: The minimum size of the prime number in bits.
            safe: If 1, the prime number will be safe: (p-1)/2 will also be prime.
            add, rem: If not NULL, p such that p % add == rem.
            cb: Callback function.
    */


    // Generate prime numbers OpenSSL 1.1.1 [Deprecated in OpenSSL 3.0]
    if (!BN_generate_prime_ex(prime1, MIN_SIZE, 0, NULL, NULL, NULL)) {
        handleErrors();
    }
    
    /*
        int BN_generate_prime_ex(BIGNUM* ret, int size, int safe, const BIGNUM* add, const BIGNUM* rem, BN_GENCB* cb, BN_CTX* ctx);
        
            ret: Where the prime number will be stored.
            size: The minimum size of the prime number in bits.
            safe: If 1, the prime number will be safe: (p-1)/2 will also be prime.
            add, rem: If not NULL, p such that p % add == rem.
            cb: Callback function.
            ctx: A pointer to a BN_CTX object.
    */
    
    
    // Generate prime numbers OpenSSL 3.0
    if (!BN_generate_prime_ex2(prime2, MIN_SIZE, 0, NULL, NULL, NULL, ctx)) {
        handleErrors();
    }

    // Print prime numbers
    BN_print_fp(stdout, prime1);
    printf("\n");
    BN_print_fp(stdout, prime2);
    printf("\n");

    /*
        int BN_is_prime_ex(const BIGNUM* p, int nchecks, BN_CTX* ctx, BN_GENCB* cb);
        
            p: The BIGNUM to check.
            nchecks: The number of Miller-Rabin primality tests to perform.
            ctx: A pointer to a BN_CTX object.
            cb: Callback function.
    */

    // Check if the prime1 are really prime, OpenSSL 1.1.1 [Deprecated in OpenSSL 3.0 | Compiler warning]
    if (BN_is_prime_ex(prime1, 16, NULL, NULL)) {
        printf("prime1 is prime\n");
    }
    else {
        printf("prime1 is not prime\n");
    }

    // Check if the numbers are really prime, OpenSSL 3.0
    if (BN_check_prime(prime2, NULL, NULL)) {
        printf("prime2 is prime\n");
    }
    else {
        printf("prime2 is not prime\n");
    }

    // Set a non prime number in prime2
    BN_set_word(prime2, 4);

    // Print prime2
    BN_print_fp(stdout, prime2);
    printf("\n");

    // Check if the numbers are really prime, OpenSSL 3.0
    if (BN_check_prime(prime2, NULL, NULL)) {
        printf("prime2 is prime\n");
    }
    else {
        printf("prime2 is not prime\n");
    }


    // Deallocate BIGNUMs
    BN_free(prime1);
    BN_free(prime2);
    BN_CTX_free(ctx);
    ERR_free_strings();

    return 0;
}