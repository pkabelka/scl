#include <stdio.h>
#include <stdlib.h>
#include <complex.h>

#define SFFT_RECURSIVE_IMPLEMENTATION
#include "../sfft_recursive.h"

int main()
{
    size_t const n = 8;

    double complex in[] = {1, 2, 3, 4, 5, 6, 7, 8};
    double complex *out = (double complex *) malloc(sizeof(double complex) * n);

    /* forward transform */
    sfft_recursive_fft(in, out, n);

    for (size_t i = 0; i < n; i++)
    {
        printf("%g %gj    ", creal(out[i]), cimag(out[i]));
    }
    printf("\n");
    /* 36 0j    -4 9.65685j    -4 4j    -4 1.65685j    -4 0j    -4 -1.65685j    -4 -4j    -4 -9.65685j    */

    /* inverse transform */
    sfft_recursive_ifft(out, in, n);

    for (size_t i = 0; i < n; i++)
    {
        printf("%g %gj    ", creal(in[i]), cimag(in[i]));
    }
    printf("\n");
    /* 1 0j    2 -2.71835e-16j    3 4.44089e-16j    4 3.82857e-16j    5 0j    6 -4.979e-17j    7 -4.44089e-16j    8 -6.12323e-17j    */

    free(out);

    return 0;
}
