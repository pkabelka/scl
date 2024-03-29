/**
 * LICENSE
 *
 *     This file is in the public domain and also 0BSD licensed.
 *     See end of file for more information.
 *
 * Compile-time options
 *
 *     #define SSCANNUM_REALLOC(ptr,size) realloc(ptr,size)
 *     #define SSCANNUM_FREE(ptr)         free(ptr)
 *
 *         These defines only need to be set in the file containing
 *         #define SSCANNUM_IMPLEMENTATION.
 *
 *         By default, sscannum uses stdlib realloc() and free() for memory
 *         management. You can substitute your own functions instead by defining
 *         these symbols. You must either define both, or neither.
 */

#ifndef INCLUDE_SSCANNUM_H
#define INCLUDE_SSCANNUM_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#if defined(SSCANNUM_REALLOC) && !defined(SSCANNUM_FREE) || !defined(SSCANNUM_REALLOC) && defined(SSCANNUM_FREE)
#error "You must define both SSCANNUM_REALLOC and SSCANNUM_FREE, or neither."
#endif
#if !defined(SSCANNUM_REALLOC) && !defined(SSCANNUM_FREE)
#include <stdlib.h>
#define SSCANNUM_REALLOC(ptr, size) realloc(ptr, size)
#define SSCANNUM_FREE(ptr) free(ptr)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Scans the characters from `getchar_func` into a buffer and converts them into
 * long long int numbers.
 *
 * This function automatically allocates memory for the `result` array.
 *
 * @param getchar_func Functions which returns a single character of the input.
 * Input end is marked with an `EOF` value.
 * @param delimiter Array of delimiters. Default delimiter is ','. Use `NULL` if
 * you don't want to specify own delimiters.
 * @param delimiter_length Length of custom `delimiter` array. Ignored if
 * `delimiter` == `NULL`.
 * @param result Pointer to the result number array pointer.
 * @param result_length Result number array length will be written to this
 * pointer.
 * @param result_capacity Result number array capacity (number of elements) will
 * be written to this pointer.
 * @return True if the function succeedes, false otherwise.
 */
bool sscannum_ll(int (*getchar_func)(),
                 char const *delimiter,
                 size_t delimiter_length,
                 long long int **result,
                 size_t *result_length,
                 size_t *result_capacity,
                 int base);

/**
 * Scans the characters from `getchar_func` into a buffer and converts them into
 * unsigned long long int numbers.
 *
 * This function automatically allocates memory for the `result` array.
 *
 * @param getchar_func Functions which returns a single character of the input.
 * Input end is marked with an `EOF` value.
 * @param delimiter Array of delimiters. Default delimiter is ','. Use `NULL` if
 * you don't want to specify own delimiters.
 * @param delimiter_length Length of custom `delimiter` array. Ignored if
 * `delimiter` == `NULL`.
 * @param result Pointer to the result number array pointer.
 * @param result_length Result number array length will be written to this
 * pointer.
 * @param result_capacity Result number array capacity (number of elements) will
 * be written to this pointer.
 * @param base Number base (radix) between `2` and `36` (inclusive).
 * @return True if the function succeedes, false otherwise.
 */
bool sscannum_ull(int (*getchar_func)(),
                  char const *delimiter,
                  size_t delimiter_length,
                  unsigned long long int **result,
                  size_t *result_length,
                  size_t *result_capacity,
                  int base);

/**
 * Scans the characters from `getchar_func` into a buffer and converts them into
 * long int numbers.
 *
 * This function automatically allocates memory for the `result` array.
 *
 * @param getchar_func Functions which returns a single character of the input.
 * Input end is marked with an `EOF` value.
 * @param delimiter Array of delimiters. Default delimiter is ','. Use `NULL` if
 * you don't want to specify own delimiters.
 * @param delimiter_length Length of custom `delimiter` array. Ignored if
 * `delimiter` == `NULL`.
 * @param result Pointer to the result number array pointer.
 * @param result_length Result number array length will be written to this
 * pointer.
 * @param result_capacity Result number array capacity (number of elements) will
 * be written to this pointer.
 * @param base Number base (radix) between `2` and `36` (inclusive).
 * @return True if the function succeedes, false otherwise.
 */
bool sscannum_l(int (*getchar_func)(),
                char const *delimiter,
                size_t delimiter_length,
                long int **result,
                size_t *result_length,
                size_t *result_capacity,
                int base);

/**
 * Scans the characters from `getchar_func` into a buffer and converts them into
 * unsigned long int numbers.
 *
 * This function automatically allocates memory for the `result` array.
 *
 * @param getchar_func Functions which returns a single character of the input.
 * Input end is marked with an `EOF` value.
 * @param delimiter Array of delimiters. Default delimiter is ','. Use `NULL` if
 * you don't want to specify own delimiters.
 * @param delimiter_length Length of custom `delimiter` array. Ignored if
 * `delimiter` == `NULL`.
 * @param result Pointer to the result number array pointer.
 * @param result_length Result number array length will be written to this
 * pointer.
 * @param result_capacity Result number array capacity (number of elements) will
 * be written to this pointer.
 * @param base Number base (radix) between `2` and `36` (inclusive).
 * @return True if the function succeedes, false otherwise.
 */
bool sscannum_ul(int (*getchar_func)(),
                 char const *delimiter,
                 size_t delimiter_length,
                 unsigned long int **result,
                 size_t *result_length,
                 size_t *result_capacity,
                 int base);

/**
 * Scans the characters from `getchar_func` into a buffer and converts them into
 * long double numbers.
 *
 * This function automatically allocates memory for the `result` array.
 *
 * @param getchar_func Functions which returns a single character of the input.
 * Input end is marked with an `EOF` value.
 * @param delimiter Array of delimiters. Default delimiter is ','. Use `NULL` if
 * you don't want to specify own delimiters.
 * @param delimiter_length Length of custom `delimiter` array. Ignored if
 * `delimiter` == `NULL`.
 * @param result Pointer to the result number array pointer.
 * @param result_length Result number array length will be written to this
 * pointer.
 * @param result_capacity Result number array capacity (number of elements) will
 * be written to this pointer.
 * @return True if the function succeedes, false otherwise.
 */
bool sscannum_ld(int (*getchar_func)(),
                 char const *delimiter,
                 size_t delimiter_length,
                 long double **result,
                 size_t *result_length,
                 size_t *result_capacity);

/**
 * Scans the characters from `getchar_func` into a buffer and converts them into
 * double numbers.
 *
 * This function automatically allocates memory for the `result` array.
 *
 * @param getchar_func Functions which returns a single character of the input.
 * Input end is marked with an `EOF` value.
 * @param delimiter Array of delimiters. Default delimiter is ','. Use `NULL` if
 * you don't want to specify own delimiters.
 * @param delimiter_length Length of custom `delimiter` array. Ignored if
 * `delimiter` == `NULL`.
 * @param result Pointer to the result number array pointer.
 * @param result_length Result number array length will be written to this
 * pointer.
 * @param result_capacity Result number array capacity (number of elements) will
 * be written to this pointer.
 * @return True if the function succeedes, false otherwise.
 */
bool sscannum_d(int (*getchar_func)(),
                char const *delimiter,
                size_t delimiter_length,
                double **result,
                size_t *result_length,
                size_t *result_capacity);

/**
 * Scans the characters from `getchar_func` into a buffer and converts them into
 * float numbers.
 *
 * This function automatically allocates memory for the `result` array.
 *
 * @param getchar_func Functions which returns a single character of the input.
 * Input end is marked with an `EOF` value.
 * @param delimiter Array of delimiters. Default delimiter is ','. Use `NULL` if
 * you don't want to specify own delimiters.
 * @param delimiter_length Length of custom `delimiter` array. Ignored if
 * `delimiter` == `NULL`.
 * @param result Pointer to the result number array pointer.
 * @param result_length Result number array length will be written to this
 * pointer.
 * @param result_capacity Result number array capacity (number of elements) will
 * be written to this pointer.
 * @return True if the function succeedes, false otherwise.
 */
bool sscannum_f(int (*getchar_func)(),
                char const *delimiter,
                size_t delimiter_length,
                float **result,
                size_t *result_length,
                size_t *result_capacity);

#ifdef __cplusplus
}
#endif

#endif /*INCLUDE_SSCANNUM_H*/

#ifdef SSCANNUM_IMPLEMENTATION

enum sscannum__number_type
{
    SSCANNUM__LONGLONGINT,
    SSCANNUM__UNSIGNEDLONGLONGINT,
    SSCANNUM__LONGINT,
    SSCANNUM__UNSIGNEDLONGINT,
    SSCANNUM__LONGDOUBLE,
    SSCANNUM__DOUBLE,
    SSCANNUM__FLOAT,
};

static bool sscannum__common(int (*getchar_func)(),
                             char const *delimiter,
                             size_t delimiter_length,
                             void **result,
                             size_t *result_length,
                             size_t *result_capacity,
                             enum sscannum__number_type number_type,
                             int base)
{
    char digit_buffer[128];
    size_t digit_buffer_index = 0;

    /* define the array element size */
    size_t numbers_element_size = 0;
    switch (number_type)
    {
    case SSCANNUM__LONGLONGINT:
        numbers_element_size = sizeof(long long int);
        break;
    case SSCANNUM__UNSIGNEDLONGLONGINT:
        numbers_element_size = sizeof(unsigned long long int);
        break;
    case SSCANNUM__LONGINT:
        numbers_element_size = sizeof(long int);
        break;
    case SSCANNUM__UNSIGNEDLONGINT:
        numbers_element_size = sizeof(unsigned long int);
        break;
    case SSCANNUM__LONGDOUBLE:
        numbers_element_size = sizeof(long double);
        break;
    case SSCANNUM__DOUBLE:
        numbers_element_size = sizeof(double);
        break;
    case SSCANNUM__FLOAT:
        numbers_element_size = sizeof(float);
        break;
    default:
        return false;
        break;
    }

    /* allocate the number array */
    size_t numbers_length = 0;
    size_t numbers_capacity = 1;
    void *numbers = (void *) SSCANNUM_REALLOC(NULL, numbers_element_size * numbers_capacity);
    if (numbers == NULL)
    {
        return false;
    }

    /* use default delimiter of ',' or custom delimiters */
    char const default_delimiter[] = {','};
    if (delimiter == NULL)
    {
        delimiter = default_delimiter;
        delimiter_length = 1;
    }

    /* read characters from `getchar_func` */
    int c = 0;
    while (c != EOF)
    {
        if (digit_buffer_index < 128 - 1)
        {
            c = getchar_func();

            /* ignore whitespace by default if it is not specified as a delimiter */
            if (isspace(c) && memchr(delimiter, c, delimiter_length) == NULL)
            {
                continue;
            }

            /* add non-delimiter chars to buffer */
            if (c != EOF)
            {
                if (memchr(delimiter, c, delimiter_length) == NULL)
                {
                    digit_buffer[digit_buffer_index++] = (char) c;
                    continue;
                }
            }
        }

        digit_buffer[digit_buffer_index] = '\0';
        digit_buffer_index = 0;

        /* ceil((numbers_length+1) * 1.5) */
        size_t const new_capacity = (3 * (numbers_length + 1) / 2 + (((numbers_length + 1) % 2) != 0));
        if (numbers_length + 1 > numbers_capacity)
        {
            if ((numbers = (void *) SSCANNUM_REALLOC(numbers, numbers_element_size * new_capacity)) == NULL)
            {
                return false;
            }
            numbers_capacity = new_capacity;
        }

        switch (number_type)
        {
        case SSCANNUM__LONGLONGINT:
            ((long long int *) numbers)[numbers_length++] = strtoll(digit_buffer, NULL, base);
            break;
        case SSCANNUM__UNSIGNEDLONGLONGINT:
            ((unsigned long long int *) numbers)[numbers_length++] = strtoull(digit_buffer, NULL, base);
            break;
        case SSCANNUM__LONGINT:
            ((long int *) numbers)[numbers_length++] = strtol(digit_buffer, NULL, base);
            break;
        case SSCANNUM__UNSIGNEDLONGINT:
            ((unsigned long int *) numbers)[numbers_length++] = strtoul(digit_buffer, NULL, base);
            break;
        case SSCANNUM__LONGDOUBLE:
            ((long double *) numbers)[numbers_length++] = strtold(digit_buffer, NULL);
            break;
        case SSCANNUM__DOUBLE:
            ((double *) numbers)[numbers_length++] = strtod(digit_buffer, NULL);
            break;
        case SSCANNUM__FLOAT:
            ((float *) numbers)[numbers_length++] = strtof(digit_buffer, NULL);
            break;
        default:
            SSCANNUM_FREE(numbers);
            return false;
            break;
        }
    }

    *result_length = numbers_length;
    *result_capacity = numbers_capacity;
    *result = numbers;
    return true;
}

bool sscannum_ll(int (*getchar_func)(),
                 char const *delimiter,
                 size_t delimiter_length,
                 long long int **result,
                 size_t *result_length,
                 size_t *result_capacity,
                 int base)
{
    return sscannum__common(
        getchar_func,
        delimiter,
        delimiter_length,
        (void **) result,
        result_length,
        result_capacity,
        SSCANNUM__LONGLONGINT,
        base);
}

bool sscannum_ull(int (*getchar_func)(),
                  char const *delimiter,
                  size_t delimiter_length,
                  unsigned long long int **result,
                  size_t *result_length,
                  size_t *result_capacity,
                  int base)
{
    return sscannum__common(
        getchar_func,
        delimiter,
        delimiter_length,
        (void **) result,
        result_length,
        result_capacity,
        SSCANNUM__UNSIGNEDLONGLONGINT,
        base);
}

bool sscannum_l(int (*getchar_func)(),
                char const *delimiter,
                size_t delimiter_length,
                long int **result,
                size_t *result_length,
                size_t *result_capacity,
                int base)
{
    return sscannum__common(
        getchar_func,
        delimiter,
        delimiter_length,
        (void **) result,
        result_length,
        result_capacity,
        SSCANNUM__LONGINT,
        base);
}

bool sscannum_ul(int (*getchar_func)(),
                 char const *delimiter,
                 size_t delimiter_length,
                 unsigned long int **result,
                 size_t *result_length,
                 size_t *result_capacity,
                 int base)
{
    return sscannum__common(
        getchar_func,
        delimiter,
        delimiter_length,
        (void **) result,
        result_length,
        result_capacity,
        SSCANNUM__UNSIGNEDLONGINT,
        base);
}

bool sscannum_ld(int (*getchar_func)(),
                 char const *delimiter,
                 size_t delimiter_length,
                 long double **result,
                 size_t *result_length,
                 size_t *result_capacity)
{
    return sscannum__common(
        getchar_func,
        delimiter,
        delimiter_length,
        (void **) result,
        result_length,
        result_capacity,
        SSCANNUM__LONGDOUBLE,
        0);
}

bool sscannum_d(int (*getchar_func)(),
                char const *delimiter,
                size_t delimiter_length,
                double **result,
                size_t *result_length,
                size_t *result_capacity)
{
    return sscannum__common(
        getchar_func,
        delimiter,
        delimiter_length,
        (void **) result,
        result_length,
        result_capacity,
        SSCANNUM__DOUBLE,
        0);
}

bool sscannum_f(int (*getchar_func)(),
                char const *delimiter,
                size_t delimiter_length,
                float **result,
                size_t *result_length,
                size_t *result_capacity)
{
    return sscannum__common(
        getchar_func,
        delimiter,
        delimiter_length,
        (void **) result,
        result_length,
        result_capacity,
        SSCANNUM__FLOAT,
        0);
}

#endif /*SSCANNUM_IMPLEMENTATION*/

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
