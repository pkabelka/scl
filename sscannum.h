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

#ifndef INCLUDE_SSCANNUM_H
#define INCLUDE_SSCANNUM_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /*INCLUDE_SSCANNUM_H*/

#define SSCANNUM_IMPLEMENTATION
#ifdef SSCANNUM_IMPLEMENTATION

enum _sscannum_number_type
{
    _SSCANNUM_LONGLONGINT,
    _SSCANNUM_DOUBLE,
};

/**
 * Checks if char `c` is in the `arr` array.
 *
 * @param c Character.
 * @param arr Array of characters.
 * @param length Length of `arr`.
 * @return True if `arr` contains `c`, false otherwise.
 */
static bool _sscannum_char_in_array(int c, char const * const arr, size_t const length)
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

static bool _sscannum_common(int (*getchar_func)(),
                  char const * delimiter,
                  size_t delimiter_length,
                  void ** result,
                  size_t *result_length,
                  size_t *result_capacity,
                  enum _sscannum_number_type number_type,
                  int base)
{
    char digit_buffer[128];
    size_t digit_buffer_index = 0;

    /* define the array element size */
    size_t numbers_element_size = 0;
    switch (number_type) {
        case _SSCANNUM_LONGLONGINT:
            numbers_element_size = sizeof(long long int);
            break;
        case _SSCANNUM_DOUBLE:
            numbers_element_size = sizeof(double);
            break;
        default:
            return false;
            break;
    }

    /* allocate the number array */
    size_t numbers_length = 0;
    size_t numbers_capacity = 1;
    void *numbers = (void *) malloc(numbers_element_size * numbers_capacity);
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
        if (digit_buffer_index < 128-1)
        {
            c = getchar_func();

            /* ignore whitespace by default */
            if ((c == '\n' && !_sscannum_char_in_array('\n', delimiter, delimiter_length))
                || (c == '\r' && !_sscannum_char_in_array('\r', delimiter, delimiter_length))
                || (c == '\t' && !_sscannum_char_in_array('\t', delimiter, delimiter_length))
                || (c == ' ' && !_sscannum_char_in_array(' ', delimiter, delimiter_length)))
            {
                continue;
            }

            /* add non-delimiter chars to buffer */
            if (c != EOF)
            {
                if (!_sscannum_char_in_array(c, delimiter, delimiter_length))
                {
                    digit_buffer[digit_buffer_index++] = c;
                    continue;
                }
            }
        }

        digit_buffer[digit_buffer_index] = '\0';
        digit_buffer_index = 0;

        /* ceil((numbers_length+1) * 1.5) */
        size_t const new_capacity = (3*(numbers_length+1)/2 + (((numbers_length+1) % 2) != 0));
        if (numbers_length+1 > numbers_capacity)
        {
            if ((numbers = (void *) realloc(numbers, numbers_element_size * new_capacity)) == NULL)
            {
                return false;
            }
            numbers_capacity = new_capacity;
        }

        switch (number_type) {
            case _SSCANNUM_LONGLONGINT:
                ((long long int *) numbers)[numbers_length++] = strtoll(digit_buffer, NULL, base);
                break;
            case _SSCANNUM_DOUBLE:
                ((double *) numbers)[numbers_length++] = strtod(digit_buffer, NULL);
                break;
            default:
                free(numbers);
                return false;
                break;
        }
    }

    *result_length = numbers_length;
    *result_capacity = numbers_capacity;
    *result = numbers;
    return true;
}

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
                  char const * delimiter,
                  size_t delimiter_length,
                  long long int ** result,
                  size_t *result_length,
                  size_t *result_capacity,
                  int base)
{
    return _sscannum_common(
        getchar_func,
        delimiter,
        delimiter_length,
        (void **) result,
        result_length,
        result_capacity,
        _SSCANNUM_LONGLONGINT,
        base);
}

bool sscannum_d(int (*getchar_func)(),
                  char const * delimiter,
                  size_t delimiter_length,
                  double ** result,
                  size_t *result_length,
                  size_t *result_capacity)
{
    return _sscannum_common(
        getchar_func,
        delimiter,
        delimiter_length,
        (void **) result,
        result_length,
        result_capacity,
        _SSCANNUM_DOUBLE,
        0);
}

#endif /*SSCANNUM_IMPLEMENTATION*/
