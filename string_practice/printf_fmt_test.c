#include <stdio.h>

// http://www.cplusplus.com/reference/cstdio/printf/

int main() {
    double pi = 3.1415926;

    // %[flags][width][.precision][length]specifier
    // For a, A, e, E, f and F specifiers: this is the number of digits to be printed
    //   after the decimal point (by default, this is 6).
    // f: Decimal floating point, lowercase
    printf("%f \n", pi); // 3.141593
    printf("%.2f \n", pi); // 3.14
    return 0;
}
