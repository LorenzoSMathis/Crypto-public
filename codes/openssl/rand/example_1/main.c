#include <stdio.h>
#include <openssl/rand.h>
#include <openssl/err.h>

#define MAX 128


void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}


int main(void)
{
    // Allocate the memory for the random string
    unsigned char random_str[MAX];

    // Initialize the random generator (explicit seeding)
    if ( RAND_load_file("/dev/random", 64) != 64 ) handleErrors();

    // Generate the random string
    if ( RAND_bytes(random_str, MAX) != 1 ) handleErrors();

    // Print the random string
    printf("Random string: \n --- \n");
    for (int i = 0; i < MAX; i++) {
        printf("%02x", random_str[i]);
    }
    printf("\n --- \n");

    ERR_free_strings();
    
    return 0;
}