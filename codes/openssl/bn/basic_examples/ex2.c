#include <stdio.h>
#include <openssl/bn.h>

int main(void)
{
    // Input from strings
    char dec_str[] = "1234567890123456789012345678901234567890";
    char hex_str[] = "3A0C92075C0DBF3B8ACBC5F96CE3F0AD2";

    BIGNUM* bn1 = BN_new();
    BIGNUM* bn2 = BN_new();

    // Conversion from strings to BIGNUMs (decimal representation)
    BN_dec2bn(&bn1, dec_str);

    // Conversion from strings to BIGNUMs (hexadecimal representation)
    BN_hex2bn(&bn2, hex_str);

    // Print the values of the BIGNUMs
    BN_print_fp(stdout, bn1);
    printf("\n");
    BN_print_fp(stdout, bn2);
    printf("\n");

    // Comparison of BIGNUMs
    if (BN_cmp(bn1, bn2) == 0) {
        printf("bn1 and bn2 are equal\n");
    } else {
        printf("bn1 and bn2 are not equal\n");
    }

    // Conversion from BIGNUMs to strings (decimal representation)
    char* dec_out = BN_bn2dec(bn1);
    fprintf(stdout, "bn1 (dec): %s\n", dec_out);

    // Conversion from BIGNUMs to strings (hexadecimal representation)
    char* hex_out = BN_bn2hex(bn1);
    fprintf(stdout, "bn1 (hex): %s\n", hex_out);

    // Deallocation of BIGNUMs
    BN_free(bn1);
    BN_free(bn2);

    return 0;
}