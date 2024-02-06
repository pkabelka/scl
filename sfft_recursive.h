/**
 * LICENSE
 *
 *     This file is in the public domain and also 0BSD licensed.
 *     See end of file for more information.
 */

#ifndef INCLUDE_SFFT_RECURSIVE_H
#define INCLUDE_SFFT_RECURSIVE_H

#include <complex.h>
#include <math.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <complex>
typedef std::complex<double> sfft_recursive_complexdouble;
#else
typedef double complex sfft_recursive_complexdouble;
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Recursively computes the Fast Fourier transform of complex numbers in array
 * `in` into the array `out`. Both arrays must be allocated before calling this
 * function.
 *
 * @param in Input array of complex numbers.
 * @param out Output array of complex numbers. `out` must have a capacity for at
 * least `n` numbers.
 * @param n Number of elements inside array `in`.
 * @return True if the function succeedes, false otherwise.
 */
bool sfft_recursive_fft(sfft_recursive_complexdouble * const in,
                        sfft_recursive_complexdouble * const out,
                        size_t const n);

/**
 * Recursively computes the Inverse Fast Fourier transform of complex numbers in
 * array `in` into the array `out`. Both arrays must be allocated before calling
 * this function.
 *
 * @param in Input array of complex numbers.
 * @param out Output array of complex numbers. `out` must have a capacity for at
 * least `n` numbers.
 * @param n Number of elements inside array `in`.
 * @return True if the function succeedes, false otherwise.
 */
bool sfft_recursive_ifft(sfft_recursive_complexdouble * const in,
                         sfft_recursive_complexdouble * const out,
                         size_t const n);

#ifdef __cplusplus
}
#endif

#endif /*INCLUDE_SFFT_RECURSIVE_H*/

#ifdef SFFT_RECURSIVE_IMPLEMENTATION

#ifndef SFFT_RECURSIVE__PI
#define SFFT_RECURSIVE__PI 3.141592653589793
#endif

void sfft__recursive(sfft_recursive_complexdouble * const in,
                     sfft_recursive_complexdouble * const out,
                     size_t const n,
                     size_t const offset,
                     bool const forward)
{
    if (n <= 1)
    {
        out[0] = in[0];
        return;
    }

    const sfft_recursive_complexdouble omega_n = cexp((forward ? -2 : 2) * I * SFFT_RECURSIVE__PI / (double) n);
    sfft_recursive_complexdouble omega = 1;

    /* even */
    sfft__recursive(in, out, n / 2, offset * 2, forward);
    /* odd */
    sfft__recursive(in + offset, out + n / 2, n / 2, offset * 2, forward);

    for (size_t k = 0; k < n / 2; k++)
    {
        const sfft_recursive_complexdouble even = out[k];
        const sfft_recursive_complexdouble odd = out[k + n / 2];
        out[k] = even + omega * odd;
        out[k + n / 2] = even - omega * odd;
        omega = omega * omega_n;
    }
}

bool sfft_recursive_fft(sfft_recursive_complexdouble * const in,
                        sfft_recursive_complexdouble * const out,
                        size_t const n)
{
    if (n == 0 || (n & (n - 1)) != 0 || in == NULL || out == NULL)
    {
        return false;
    }

    sfft__recursive(in, out, n, 1, true);

    return true;
}

bool sfft_recursive_ifft(sfft_recursive_complexdouble * const in,
                         sfft_recursive_complexdouble * const out,
                         size_t const n)
{
    if (n == 0 || (n & (n - 1)) != 0 || in == NULL || out == NULL)
    {
        return false;
    }

    sfft__recursive(in, out, n, 1, false);

    for (size_t i = 0; i < n; i++)
    {
        out[i] /= (double) n;
    }

    return true;
}

#endif /*SFFT_RECURSIVE_IMPLEMENTATION*/

/*
-------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
-------------------------------------------------------------------------------
0BSD license:

Copyright (c) 2023 Petr Kabelka

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.
-------------------------------------------------------------------------------
Public Domain (Unlicense):

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
-------------------------------------------------------------------------------
*/
