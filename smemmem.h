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
 * Compile-time options
 *
 *     #define SMEMMEM_REALLOC(ptr,size) realloc(ptr,size)
 *     #define SMEMMEM_FREE(ptr)         free(ptr)
 *
 *         These defines only need to be set in the file containing
 *         #define SMEMMEM_IMPLEMENTATION.
 *
 *         By default, smemmem uses stdlib realloc() and free() for memory
 *         management. You can substitute your own functions instead by defining
 *         these symbols. You must either define both, or neither.
 */

#ifndef INCLUDE_SMEMMEM_H
#define INCLUDE_SMEMMEM_H

#include <stdbool.h>
#include <string.h>

#if defined(SMEMMEM_REALLOC) && !defined(SMEMMEM_FREE) || !defined(SMEMMEM_REALLOC) && defined(SMEMMEM_FREE)
#error "You must define both SMEMMEM_REALLOC and SMEMMEM_FREE, or neither."
#endif
#if !defined(SMEMMEM_REALLOC) && !defined(SMEMMEM_FREE)
#include <stdlib.h>
#define SMEMMEM_REALLOC(ptr, size) realloc(ptr, size)
#define SMEMMEM_FREE(ptr) free(ptr)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Finds the start of the first occurrence of the substring `needle` of length
 * `needle_len` in `haystack` of length `haystack_len`.
 *
 * @param haystack Memory buffer to search through.
 * @param haystack_len Length of the memory buffer to search through in bytes.
 * @param needle Substring to find.
 * @param needle_len Length of the substring to find in bytes.
 * @return Pointer to the start of the substring.
 * @return `NULL` if the substring is not found.
 * @return `haystack` if `needle` is empty.
 */
void *smemmem_naive(const void * const haystack,
                    size_t const haystack_len,
                    const void * const needle,
                    size_t const needle_len);

/**
 * Finds the start of the first occurrence of the substring `needle` of length
 * `needle_len` in `haystack` of length `haystack_len` using
 * Boyer-Moore-Horspool algorithm.
 *
 * @param haystack Memory buffer to search through.
 * @param haystack_len Length of the memory buffer to search through in bytes.
 * @param needle Substring to find.
 * @param needle_len Length of the substring to find in bytes.
 * @param skip_table_buf Buffer for the skip table, must have enough capacity
 * for `sizeof(size_t) * 2^(sizeof(char) * 8)` elements. If `NULL` then the
 * table is allocated using the `SMEMMEM_REALLOC` macro and free'd with
 * `SMEMMEM_FREE` macro.
 * @return Pointer to the start of the substring.
 * @return `NULL` if the substring is not found.
 * @return `haystack` if `needle` is empty.
 */
void *smemmem_bmh(const void * const haystack,
                  size_t const haystack_len,
                  const void * const needle,
                  size_t const needle_len,
                  size_t * const skip_table_buf);

/**
 * Finds the start of the first occurrence of the substring `needle` of length
 * `needle_len` in `haystack` of length `haystack_len` using
 * Knuth-Morris-Pratt algorithm.
 *
 * @param haystack Memory buffer to search through.
 * @param haystack_len Length of the memory buffer to search through in bytes.
 * @param needle Substring to find.
 * @param needle_len Length of the substring to find in bytes.
 * @return Pointer to the start of the substring.
 * @return `NULL` if the substring is not found.
 * @return `haystack` if `needle` is empty.
 */
void *smemmem_kmp(const void * const haystack,
                  size_t const haystack_len,
                  const void * const needle,
                  size_t const needle_len);

/**
 * Finds the starting indices of all occurrences of the substring `needle` of
 * length `needle_len` in `haystack` of length `haystack_len` using
 * Knuth-Morris-Pratt algorithm.
 *
 * @param haystack Memory buffer to search through.
 * @param haystack_len Length of the memory buffer to search through in bytes.
 * @param needle Substring to find.
 * @param needle_len Length of the substring to find in bytes.
 * @param indices Pointer to the indices array. This function automatically
 * allocates the array.
 * @return Number of found indices.
 */
size_t smemmem_kmp_all(const void * const haystack,
                       size_t const haystack_len,
                       const void * const needle,
                       size_t const needle_len,
                       size_t **indices);

#ifdef __cplusplus
}
#endif

#endif /*INCLUDE_SMEMMEM_H*/

#ifdef SMEMMEM_IMPLEMENTATION

void *smemmem_naive(const void * const haystack,
                    size_t const haystack_len,
                    const void * const needle,
                    size_t const needle_len)
{
    if (needle == NULL || needle_len == 0)
    {
        return (void *) haystack;
    }
    if (haystack == NULL || haystack_len == 0 || needle_len > haystack_len)
    {
        return NULL;
    }

    for (size_t i = 0; i <= haystack_len - needle_len; i++)
    {
        if (memcmp(((char *) haystack + i), needle, needle_len) == 0)
        {
            return (void *) ((char *) haystack + i);
        }
    }
    return NULL;
}

void *smemmem_bmh(const void * const haystack,
                  size_t const haystack_len,
                  const void * const needle,
                  size_t const needle_len,
                  size_t * const skip_table_buf)
{
    if (needle == NULL || needle_len == 0)
    {
        return (void *) haystack;
    }
    if (haystack == NULL || haystack_len == 0 || needle_len > haystack_len)
    {
        return NULL;
    }

    size_t const skip_table_size = 1 << (sizeof(char) * 8);
    size_t *skip_table = NULL;
    if (skip_table_buf == NULL)
    {
        skip_table = (size_t *) SMEMMEM_REALLOC(NULL, sizeof(size_t) * skip_table_size);
        if (skip_table == NULL)
        {
            return NULL;
        }
    }
    else
    {
        skip_table = skip_table_buf;
    }

    for (size_t i = 0; i < skip_table_size; i++)
    {
        skip_table[i] = needle_len;
    }
    for (size_t i = 0; i < needle_len - 1; i++)
    {
        skip_table[(size_t) ((char *) needle)[i]] = needle_len - 1 - i;
    }

    size_t skip = 0;
    while ((haystack_len - skip) >= needle_len)
    {
        if (memcmp((char *) haystack + skip, needle, needle_len) == 0)
        {
            if (skip_table_buf == NULL)
            {
                SMEMMEM_FREE(skip_table);
            }
            return (void *) ((char *) haystack + skip);
        }
        skip += skip_table[(size_t) ((char *) haystack)[skip + needle_len - 1]];
    }

    if (skip_table_buf == NULL)
    {
        SMEMMEM_FREE(skip_table);
    }
    return NULL;
}

static size_t smemmem__kmp_common(const void * const haystack,
                                  size_t const haystack_len,
                                  const void * const needle,
                                  size_t const needle_len,
                                  size_t **indices,
                                  bool const only_first)
{
    long long i = 1;
    long long j = 0;

    /* compute the KMP table */
    long long *kmp_table = (long long *) SMEMMEM_REALLOC(NULL, sizeof(long long) * (needle_len + 1));
    kmp_table[0] = -1;

    while (i < (long long) needle_len)
    {
        if (((char *) needle)[i] == ((char *) needle)[j])
        {
            kmp_table[i] = kmp_table[j];
        }
        else
        {
            kmp_table[i] = j;
            while (j >= 0 && ((char *) needle)[i] != ((char *) needle)[j])
            {
                j = kmp_table[j];
            }
        }

        i++;
        j++;
    }
    kmp_table[i] = j;

    /* start searching substrings */
    i = 0;
    j = 0;
    size_t number_of_indices = 0;

    while (i < (long long) haystack_len)
    {
        if (((char *) haystack)[i] == ((char *) needle)[j])
        {
            i++;
            j++;

            /* found */
            if (j == (long long) needle_len)
            {
                if (only_first)
                {
                    SMEMMEM_FREE(kmp_table);
                    return (size_t) (i - j);
                }
                /* allocate memory for indices */
                if (number_of_indices == 0)
                {
                    if ((*indices = (size_t *) SMEMMEM_REALLOC(NULL, sizeof(size_t))) == NULL)
                    {
                        SMEMMEM_FREE(kmp_table);
                        return 0;
                    }
                }
                else if (number_of_indices > 0)
                {
                    if ((*indices = (size_t *) SMEMMEM_REALLOC(*indices, sizeof(size_t) * (number_of_indices + 1))) == NULL)
                    {
                        SMEMMEM_FREE(kmp_table);
                        return 0;
                    }
                }
                else
                {
                    SMEMMEM_FREE(kmp_table);
                    return 0;
                }

                (*indices)[number_of_indices++] = (size_t) (i - j);
                /* go to next */
                j = kmp_table[j];
            }
        }
        else
        {
            j = kmp_table[j];
            if (j < 0)
            {
                i++;
                j++;
            }
        }
    }

    SMEMMEM_FREE(kmp_table);
    return number_of_indices;
}

void *smemmem_kmp(const void * const haystack,
                  size_t const haystack_len,
                  const void * const needle,
                  size_t const needle_len)
{
    if (needle == NULL || needle_len == 0)
    {
        return (void *) haystack;
    }
    if (haystack == NULL || haystack_len == 0 || needle_len > haystack_len)
    {
        return NULL;
    }

    size_t const offset = smemmem__kmp_common(haystack, haystack_len, needle,
                                              needle_len, NULL, true);
    if (offset == 0)
    {
        return NULL;
    }
    return (void *) ((char *) haystack + offset);
}

size_t smemmem_kmp_all(const void * const haystack,
                       size_t const haystack_len,
                       const void * const needle,
                       size_t const needle_len,
                       size_t **indices)
{
    if (haystack == NULL || needle == NULL || haystack_len == 0 || needle_len == 0 || needle_len > haystack_len)
    {
        return 0;
    }
    return smemmem__kmp_common(haystack, haystack_len, needle, needle_len, indices, false);
}

#endif /*SMEMMEM_IMPLEMENTATION*/
