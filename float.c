#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef union {
    float f;
    struct {
        unsigned int mantissa : 23;
        unsigned int exponent : 8;
        unsigned int sign : 1;
    } binary_parts;
} float_cast;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Wrong number of arguments\n");
        return 1;
    }

    char *temp = argv[1];

    if (strcmp(argv[1], "f") == 0) {
        float num = strtof(argv[2], NULL);
        float_cast d1 = { .f = num };

        printf("Binary: ");
        for (int i = 31; i >= 0; i--) {
            int mask = (1 << i);
            if (d1.binary_parts.mantissa & mask)
                printf("1");
            else
                printf("0");
        }

        printf("\nsign : %x\n", d1.binary_parts.sign);
        printf("exponent : %x\n", d1.binary_parts.exponent);
        printf("mantissa : %x\n", d1.binary_parts.mantissa);
        printf("Value : %f\n", num);
    } 
    else if (strcmp(argv[1], "b") == 0) {
        printf("Binary : %s\n", temp);

        // Assuming that the input argv[2] is a string of '0's and '1's representing the binary
        int num1 = atoi(argv[2]);
        int decimal = 0, base = 1;

        while (num1 > 0) {
            int rem = num1 % 10;
            decimal = decimal + rem * base;
            num1 = num1 / 10;
            base = base * 2;
        }

        printf("Decimal : %d\n", decimal);
    } 
    else {
        printf("Invalid option. Use 'f' or 'b'\n");
    }

    return 0;
}
