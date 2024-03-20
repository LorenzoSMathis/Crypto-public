#include <stdio.h>
#include <openssl/bn.h>

int main(void)
{
    BIGNUM* bn1 = BN_new();
    BIGNUM* bn2 = BN_new();
    BIGNUM* bn3 = BN_new();

    BN_CTX* ctx = BN_CTX_new();

    // Print the value of bn1
    BN_print_fp(stdout, bn1);
    printf("\n");

    // Set bn1 to 123 (from a C long)
    BN_set_word(bn1, 123);

    // Print the value of bn1 again
    BN_print_fp(stdout, bn1);
    printf("\n");

    // Set bn2 to 456 (from a decimal string)
    BN_dec2bn(&bn2, "456");

    // Print the value of bn2
    BN_print_fp(stdout, bn2);
    printf("\n");

    // Add bn1 and bn2 and store the result in bn3
    BN_add(bn3, bn1, bn2);

    // Print the value of bn3
    BN_print_fp(stdout, bn3);
    printf("\n");

    // Modulo bn3 by bn2 and store the result in bn3 (bn3 = bn3 % bn2)
    BN_mod(bn3, bn3, bn2, ctx);

    // Print the value of bn3
    BN_print_fp(stdout, bn3);
    printf("\n");

    // Free the BIGNUMs and the context
    BN_free(bn1);
    BN_free(bn2);
    BN_free(bn3);
    BN_CTX_free(ctx);

    return 0;
}