/** This is free and unencumbered software released into the public domain.
  * 
  * Anyone is free to copy, modify, publish, use, compile, sell, or
  * distribute this software, either in source code form or as a compiled
  * binary, for any purpose, commercial or non-commercial, and by any
  * means.
  * 
  * In jurisdictions that recognize copyright laws, the author or authors
  * of this software dedicate any and all copyright interest in the
  * software to the public domain. We make this dedication for the benefit
  * of the public at large and to the detriment of our heirs and
  * successors. We intend this dedication to be an overt act of
  * relinquishment in perpetuity of all present and future rights to this
  * software under copyright law.
  * 
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
  * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
  * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
  * OTHER DEALINGS IN THE SOFTWARE.
  * 
  * For more information, please refer to <http://unlicense.org/>
  */

/**
 * Algorithm taken from:
 * https://github.com/983/fft/blob/7c9b0295be19b93b8aa36066da715eaf66a86a3f/fft.c
 * which is also in public domain thanks to the "UNLICENSE" license.
 * This implementation adds safety checks, better formatting, naming, double
 * instead of float.
 */

#ifndef INCLUDE_SFFT_H
#define INCLUDE_SFFT_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    size_t n;
    size_t *bit_reverse;
    double *sine;
    double *cosine;
} sfft;

bool sfft_new(sfft * const fft, size_t const n);
void sfft_free(sfft * const fft);
bool sfft_fft(sfft * const fft, double * const real, double * const imag);
bool sfft_ifft(sfft * const fft, double * const real, double * const imag);

#ifdef __cplusplus
}
#endif

#endif /*INCLUDE_SFFT_H*/

#ifdef SFFT_IMPLEMENTATION

static size_t sfft_bit_size(size_t n)
{
    size_t size = 0;
    for (; n; n >>= 1) size++;
    return size;
}

static size_t sfft_bit_reverse(size_t n, size_t bit_size)
{
    size_t reversed = 0;
    for (size_t i = 0; i < bit_size; i++)
    {
        reversed = (reversed << 1) | ((n >> i) & 1);
    }
    return reversed;
}

bool sfft_new(sfft * const fft, size_t const n)
{
    fft->n = 0;

    if ((fft->bit_reverse = (size_t *) malloc(sizeof(size_t) * n)) == NULL)
    {
        return false;
    }

    if ((fft->sine = (double *) malloc(sizeof(double) * n)) == NULL)
    {
        return false;
    }

    if ((fft->cosine = (double *) malloc(sizeof(double) * n)) == NULL)
    {
        return false;
    }

    fft->n = n;

    static const double pi = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117068;
    size_t log2_n = sfft_bit_size(n) - 1;

    for (size_t i = 0; i < n; i++)
    {
        double w = -2.0 * pi / n * i;
        fft->bit_reverse[i] = sfft_bit_reverse(i, log2_n);
        fft->sine[i] = sin(w);
        fft->cosine[i] = cos(w);
    }

    return true;
}

void sfft_free(sfft * const fft)
{
    free(fft->bit_reverse);
    free(fft->sine);
    free(fft->cosine);

    fft->n = 0;
    fft->bit_reverse = NULL;
    fft->sine = NULL;
    fft->cosine = NULL;
}

static void sfft_swap_and_run_first(sfft * const fft, double *real, double *imag)
{
    size_t const n = fft->n;
    for (size_t i = 0; i < n; i++)
    {
        size_t j = fft->bit_reverse[i];
        if (i < j)
        {
            double tmp = real[i];
            real[i] = real[j];
            real[j] = tmp;

            tmp = imag[i];
            imag[i] = imag[j];
            imag[j] = tmp;
        }
    }

    if (n >= 2)
    {
        for (size_t i = 0; i < n; i += 2)
        {
            double real_0 = real[i];
            double imag_0 = imag[i];
            double real_1 = real[i + 1];
            double imag_1 = imag[i + 1];

            real[i] = real_0 + real_1;
            imag[i] = imag_0 + imag_1;
            real[i + 1] = real_0 - real_1;
            imag[i + 1] = imag_0 - imag_1;
        }
    }
}

static bool is_of_power2(size_t n)
{
    return (n & (n - 1)) == 0;
}

bool sfft_fft(sfft * const fft, double * const real, double * const imag)
{
    size_t const n = fft->n;
    double const * const sine = fft->sine;
    double const * const cosine = fft->cosine;
    size_t const * const bit_reverse = fft->bit_reverse;

    if (n == 0 || sine == NULL || cosine == NULL || bit_reverse == NULL || !is_of_power2(n))
    {
        return false;
    }

    sfft_swap_and_run_first(fft, real, imag);

    if (n >= 4)
    {
        for (size_t i = 0; i < n; i += 4)
        {
            double real_0 = real[i];
            double imag_0 = imag[i];
            double real_1 = real[i + 1];
            double imag_1 = imag[i + 1];
            double real_2 = real[i + 2];
            double imag_2 = imag[i + 2];
            double real_3 = real[i + 3];
            double imag_3 = imag[i + 3];

            real[i] = real_0 + real_2;
            imag[i] = imag_0 + imag_2;
            real[i + 1] = real_1 - imag_3;
            imag[i + 1] = imag_1 - real_3;
            real[i + 2] = real_0 - real_2;
            imag[i + 2] = imag_0 - imag_2;
            real[i + 3] = real_1 + imag_3;
            imag[i + 3] = imag_1 + real_3;
        }
    }

    for (size_t block_size = 8; block_size <= n; block_size *= 2)
    {
        size_t cos_sin_stride = n / block_size;
        for (size_t block_offset = 0; block_offset < n; block_offset += block_size)
        {
            for (size_t i = 0; i < block_size / 2; i++)
            {
                double real_0 = real[block_offset + i];
                double imag_0 = imag[block_offset + i];
                double real_1 = real[block_offset + i + block_size / 2];
                double imag_1 = imag[block_offset + i + block_size / 2];
                double real_2 = cosine[i * cos_sin_stride];
                double imag_2 = sine[i * cos_sin_stride];
                double real_3 = real_1 * real_2 - imag_1 * imag_2;
                double imag_3 = real_1 * imag_2 + imag_1 * real_2;

                real[block_offset + i] = real_0 + real_3;
                imag[block_offset + i] = imag_0 + imag_3;
                real[block_offset + i + block_size / 2] = real_0 - real_3;
                imag[block_offset + i + block_size / 2] = imag_0 - imag_3;
            }
        }
    }

    return true;
}

bool sfft_ifft(sfft * const fft, double * const real, double * const imag)
{
    size_t const n = fft->n;
    double const * const sine = fft->sine;
    double const * const cosine = fft->cosine;
    size_t const * const bit_reverse = fft->bit_reverse;

    if (n == 0 || sine == NULL || cosine == NULL || bit_reverse == NULL || !is_of_power2(n))
    {
        return false;
    }

    sfft_swap_and_run_first(fft, real, imag);

    if (n >= 4)
    {
        for (size_t i = 0; i < n; i += 4)
        {
            double real_0 = real[i];
            double imag_0 = imag[i];
            double real_1 = real[i + 1];
            double imag_1 = imag[i + 1];
            double real_2 = real[i + 2];
            double imag_2 = imag[i + 2];
            double real_3 = -real[i + 3];
            double imag_3 = imag[i + 3];

            real[i] = real_0 + real_2;
            imag[i] = imag_0 + imag_2;
            real[i + 1] = real_1 - imag_3;
            imag[i + 1] = imag_1 - real_3;
            real[i + 2] = real_0 - real_2;
            imag[i + 2] = imag_0 - imag_2;
            real[i + 3] = real_1 + imag_3;
            imag[i + 3] = imag_1 + real_3;
        }
    }

    for (size_t block_size = 8; block_size <= n; block_size *= 2)
    {
        size_t cos_sin_stride = n / block_size;
        for (size_t block_offset = 0; block_offset < n; block_offset += block_size)
        {
            for (size_t i = 0; i < block_size / 2; i++)
            {
                double real_0 = real[block_offset + i];
                double imag_0 = imag[block_offset + i];
                double real_1 = real[block_offset + i + block_size / 2];
                double imag_1 = imag[block_offset + i + block_size / 2];
                double real_2 = cosine[i * cos_sin_stride];
                double imag_2 = -sine[i * cos_sin_stride];
                double real_3 = real_1 * real_2 - imag_1 * imag_2;
                double imag_3 = real_1 * imag_2 + imag_1 * real_2;

                real[block_offset + i] = real_0 + real_3;
                imag[block_offset + i] = imag_0 + imag_3;
                real[block_offset + i + block_size / 2] = real_0 - real_3;
                imag[block_offset + i + block_size / 2] = imag_0 - imag_3;
            }
        }
    }

    for (size_t i = 0; i < n; i++)
    {
        real[i] /= n;
        imag[i] /= n;
    }

    return true;
}

#endif /*SFFT_IMPLEMENTATION*/
