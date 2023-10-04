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

#ifndef INCLUDE_SGETNUM_H
#define INCLUDE_SGETNUM_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Scans the characters from `getchar_func` into a buffer and converts them into
 * a long long int number.
 *
 * @param getchar_func Functions which returns a single character of the input.
 * Input end is marked with an `EOF` value.
 * @param delimiter Array of delimiters. Default delimiter is ','. Use `NULL` if
 * you don't want to specify own delimiters.
 * @param delimiter_length Length of custom `delimiter` array. Ignored if
 * `delimiter` == `NULL`.
 * @param result Pointer to the result number.
 * @return True if the function can be called again for another number, false if
 * `getchar_func` returned `EOF`.
 */
bool sgetnum_ll(int (*getchar_func)(),
                 char const * delimiter,
                 size_t delimiter_length,
                 long long int * result,
                 int base);

/**
 * Scans the characters from `getchar_func` into a buffer and converts them into
 * an unsigned long long int number.
 *
 * @param getchar_func Functions which returns a single character of the input.
 * Input end is marked with an `EOF` value.
 * @param delimiter Array of delimiters. Default delimiter is ','. Use `NULL` if
 * you don't want to specify own delimiters.
 * @param delimiter_length Length of custom `delimiter` array. Ignored if
 * `delimiter` == `NULL`.
 * @param result Pointer to the result number.
 * @return True if the function can be called again for another number, false if
 * `getchar_func` returned `EOF`.
 */
bool sgetnum_ull(int (*getchar_func)(),
                  char const * delimiter,
                  size_t delimiter_length,
                  unsigned long long int * result,
                  int base);

/**
 * Scans the characters from `getchar_func` into a buffer and converts them into
 * a long int number.
 *
 * @param getchar_func Functions which returns a single character of the input.
 * Input end is marked with an `EOF` value.
 * @param delimiter Array of delimiters. Default delimiter is ','. Use `NULL` if
 * you don't want to specify own delimiters.
 * @param delimiter_length Length of custom `delimiter` array. Ignored if
 * `delimiter` == `NULL`.
 * @param result Pointer to the result number.
 * @return True if the function can be called again for another number, false if
 * `getchar_func` returned `EOF`.
 */
bool sgetnum_l(int (*getchar_func)(),
                char const * delimiter,
                size_t delimiter_length,
                long int * result,
                int base);

/**
 * Scans the characters from `getchar_func` into a buffer and converts them into
 * an unsigned long int number.
 *
 * @param getchar_func Functions which returns a single character of the input.
 * Input end is marked with an `EOF` value.
 * @param delimiter Array of delimiters. Default delimiter is ','. Use `NULL` if
 * you don't want to specify own delimiters.
 * @param delimiter_length Length of custom `delimiter` array. Ignored if
 * `delimiter` == `NULL`.
 * @param result Pointer to the result number.
 * @return True if the function can be called again for another number, false if
 * `getchar_func` returned `EOF`.
 */
bool sgetnum_ul(int (*getchar_func)(),
                 char const * delimiter,
                 size_t delimiter_length,
                 unsigned long int * result,
                 int base);

/**
 * Scans the characters from `getchar_func` into a buffer and converts them into
 * a long double number.
 *
 * @param getchar_func Functions which returns a single character of the input.
 * Input end is marked with an `EOF` value.
 * @param delimiter Array of delimiters. Default delimiter is ','. Use `NULL` if
 * you don't want to specify own delimiters.
 * @param delimiter_length Length of custom `delimiter` array. Ignored if
 * `delimiter` == `NULL`.
 * @param result Pointer to the result number.
 * @return True if the function can be called again for another number, false if
 * `getchar_func` returned `EOF`.
 */
bool sgetnum_ld(int (*getchar_func)(),
                 char const * delimiter,
                 size_t delimiter_length,
                 long double * result);

/**
 * Scans the characters from `getchar_func` into a buffer and converts them into
 * a double number.
 *
 * @param getchar_func Functions which returns a single character of the input.
 * Input end is marked with an `EOF` value.
 * @param delimiter Array of delimiters. Default delimiter is ','. Use `NULL` if
 * you don't want to specify own delimiters.
 * @param delimiter_length Length of custom `delimiter` array. Ignored if
 * `delimiter` == `NULL`.
 * @param result Pointer to the result number.
 * @return True if the function can be called again for another number, false if
 * `getchar_func` returned `EOF`.
 */
bool sgetnum_d(int (*getchar_func)(),
                char const * delimiter,
                size_t delimiter_length,
                double * result);

/**
 * Scans the characters from `getchar_func` into a buffer and converts them into
 * a float number.
 *
 * @param getchar_func Functions which returns a single character of the input.
 * Input end is marked with an `EOF` value.
 * @param delimiter Array of delimiters. Default delimiter is ','. Use `NULL` if
 * you don't want to specify own delimiters.
 * @param delimiter_length Length of custom `delimiter` array. Ignored if
 * `delimiter` == `NULL`.
 * @param result Pointer to the result number.
 * @return True if the function can be called again for another number, false if
 * `getchar_func` returned `EOF`.
 */
bool sgetnum_f(int (*getchar_func)(),
                char const * delimiter,
                size_t delimiter_length,
                float * result);

#ifdef __cplusplus
}
#endif

#endif /*INCLUDE_SGETNUM_H*/

#ifdef SGETNUM_IMPLEMENTATION

enum _sgetnum_number_type
{
    _SGETNUM_LONGLONGINT,
    _SGETNUM_UNSIGNEDLONGLONGINT,
    _SGETNUM_LONGINT,
    _SGETNUM_UNSIGNEDLONGINT,
    _SGETNUM_LONGDOUBLE,
    _SGETNUM_DOUBLE,
    _SGETNUM_FLOAT,
};

/**
 * Checks if char `c` is in the `arr` array.
 *
 * @param c Character.
 * @param arr Array of characters.
 * @param length Length of `arr`.
 * @return True if `arr` contains `c`, false otherwise.
 */
static bool _sgetnum_char_in_array(int c, char const * const arr, size_t const length)
{
    for (size_t i = 0; i < length; i++)
    {
        if (arr[i] == c)
        {
            return true;
        }
    }
    return false;
}

static bool _sgetnum_common(int (*getchar_func)(),
                             char const * delimiter,
                             size_t delimiter_length,
                             void * result,
                             enum _sgetnum_number_type number_type,
                             int base)
{
    char digit_buffer[128];
    size_t digit_buffer_index = 0;

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
        if (digit_buffer_index < 128-1)
        {
            c = getchar_func();

            /* ignore whitespace by default if it is not specified as a delimiter */
            if (isspace(c) && !_sgetnum_char_in_array(c, delimiter, delimiter_length))
            {
                continue;
            }

            /* add non-delimiter chars to buffer */
            if (c != EOF)
            {
                if (!_sgetnum_char_in_array(c, delimiter, delimiter_length))
                {
                    digit_buffer[digit_buffer_index++] = c;
                    continue;
                }
            }
        }

        digit_buffer[digit_buffer_index] = '\0';
        digit_buffer_index = 0;

        switch (number_type) {
            case _SGETNUM_LONGLONGINT:
                *((long long int *) result) = strtoll(digit_buffer, NULL, base);
                break;
            case _SGETNUM_UNSIGNEDLONGLONGINT:
                *((unsigned long long int *) result) = strtoull(digit_buffer, NULL, base);
                break;
            case _SGETNUM_LONGINT:
                *((long int *) result) = strtol(digit_buffer, NULL, base);
                break;
            case _SGETNUM_UNSIGNEDLONGINT:
                *((unsigned long int *) result) = strtoul(digit_buffer, NULL, base);
                break;
            case _SGETNUM_LONGDOUBLE:
                *((long double *) result) = strtold(digit_buffer, NULL);
                break;
            case _SGETNUM_DOUBLE:
                *((double *) result) = strtod(digit_buffer, NULL);
                break;
            case _SGETNUM_FLOAT:
                *((float *) result) = strtof(digit_buffer, NULL);
                break;
            default:
                return false;
                break;
        }

        if (c == EOF)
        {
            return false;
        }

        return true;
    }

    return false;
}

bool sgetnum_ll(int (*getchar_func)(),
                 char const * delimiter,
                 size_t delimiter_length,
                 long long int * result,
                 int base)
{
    return _sgetnum_common(
        getchar_func,
        delimiter,
        delimiter_length,
        (void *) result,
        _SGETNUM_LONGLONGINT,
        base);
}

bool sgetnum_ull(int (*getchar_func)(),
                  char const * delimiter,
                  size_t delimiter_length,
                  unsigned long long int * result,
                  int base)
{
    return _sgetnum_common(
        getchar_func,
        delimiter,
        delimiter_length,
        (void *) result,
        _SGETNUM_UNSIGNEDLONGLONGINT,
        base);
}

bool sgetnum_l(int (*getchar_func)(),
                char const * delimiter,
                size_t delimiter_length,
                long int * result,
                int base)
{
    return _sgetnum_common(
        getchar_func,
        delimiter,
        delimiter_length,
        (void *) result,
        _SGETNUM_LONGINT,
        base);
}

bool sgetnum_ul(int (*getchar_func)(),
                 char const * delimiter,
                 size_t delimiter_length,
                 unsigned long int * result,
                 int base)
{
    return _sgetnum_common(
        getchar_func,
        delimiter,
        delimiter_length,
        (void *) result,
        _SGETNUM_UNSIGNEDLONGINT,
        base);
}

bool sgetnum_ld(int (*getchar_func)(),
                 char const * delimiter,
                 size_t delimiter_length,
                 long double * result)
{
    return _sgetnum_common(
        getchar_func,
        delimiter,
        delimiter_length,
        (void *) result,
        _SGETNUM_LONGDOUBLE,
        0);
}

bool sgetnum_d(int (*getchar_func)(),
                char const * delimiter,
                size_t delimiter_length,
                double * result)
{
    return _sgetnum_common(
        getchar_func,
        delimiter,
        delimiter_length,
        (void *) result,
        _SGETNUM_DOUBLE,
        0);
}

bool sgetnum_f(int (*getchar_func)(),
                char const * delimiter,
                size_t delimiter_length,
                float * result)
{
    return _sgetnum_common(
        getchar_func,
        delimiter,
        delimiter_length,
        (void *) result,
        _SGETNUM_FLOAT,
        0);
}

#endif /*SGETNUM_IMPLEMENTATION*/
