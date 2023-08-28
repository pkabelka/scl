#include <stdio.h>

#define SFFT_IMPLEMENTATION
#include "../sfft.h"

int main()
{
    size_t n = 8;
    sfft fft;
    sfft_new(&fft, n);

    double real[] = {1, 2, 3, 4, 5, 6, 7, 8};
    double imag[] = {0, 0, 0, 0, 0, 0, 0, 0};

    /* forward transform */
    sfft_fft(&fft, real, imag);

    for (size_t i = 0; i < n; i++)
    {
        printf("%g %gj    ", real[i], imag[i]);
    }
    printf("\n");
    /* 36 0j    -4 9.65685j    -4 4j    -4 1.65685j    -4 0j    -4 -1.65685j    -4 -4j    -4 -9.65685j    */

    /* inverse transform */
    sfft_ifft(&fft, real, imag);

    for (size_t i = 0; i < n; i++)
    {
        printf("%g %gj    ", real[i], imag[i]);
    }
    printf("\n");
    /* 1 0j    2 1.66533e-16j    3 1.22465e-16j    4 2.77556e-16j    5 0j    6 -2.77556e-16j    7 -1.22465e-16j    8 -1.66533e-16j    */

    sfft_free(&fft);

    return 0;
}
