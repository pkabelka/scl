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
 *     #define SHASHTAB_CALLOC(count,size) calloc(count,size)
 *     #define SHASHTAB_FREE(ptr)          free(ptr)
 *
 *         These defines only need to be set in the file containing
 *         #define SHASHTAB_IMPLEMENTATION.
 *
 *         By default, shashtab uses stdlib calloc() and free() for memory
 *         management. You can substitute your own functions instead by defining
 *         these symbols. You must either define both, or neither.
 */

#ifndef INCLUDE_SHASHTAB_H
#define INCLUDE_SHASHTAB_H

#include "sbintree.h"
#include <stdbool.h>
#include <string.h>

#if defined(SHASHTAB_CALLOC) && !defined(SHASHTAB_FREE) || !defined(SHASHTAB_CALLOC) && defined(SHASHTAB_FREE)
#error "You must define both SHASHTAB_CALLOC and SHASHTAB_FREE, or neither."
#endif
#if !defined(SHASHTAB_CALLOC) && !defined(SHASHTAB_FREE)
#include <stdlib.h>
#define SHASHTAB_CALLOC(count, size) calloc(count, size)
#define SHASHTAB_FREE(ptr) free(ptr)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    sbintree **data;
    size_t capacity;
} shashtab;

/**
 * Allocates a new hash table. You MUST check if the returned struct's .capacity
 * field is > 0.
 *
 * @param capacity Initial capacity of the hash table.
 * @return Hash table struct.
 */
shashtab shashtab_new(size_t const capacity);

bool shashtab_set(shashtab const ht,
                  void * const key,
                  size_t const key_len,
                  int (*key_func)(void *key, void *node_key),
                  void * const data);

void *shashtab_get(shashtab const ht,
                   void * const key,
                   size_t const key_len,
                   int (*key_func)(void *key, void *node_key));

void shashtab_del(shashtab const ht,
                  void * const key,
                  size_t const key_len,
                  int (*key_func)(void *key, void *node_key),
                  void (*free_func)(sbintree *));

void shashtab_free(shashtab ht, void (*free_func)(sbintree *));

#ifdef __cplusplus
}
#endif

#endif /*INCLUDE_SHASHTAB_H*/

#ifdef SHASHTAB_IMPLEMENTATION

static unsigned int shashtab__fnv_1a_32(void * const buf, size_t const buf_len)
{
    unsigned char *bp = (unsigned char *) buf;
    unsigned char * const be = bp + buf_len;

    unsigned int hash = 0x811c9dc5;

    while (bp < be)
    {
        hash ^= (unsigned int) *bp++;
        hash += (hash << 1) + (hash << 4) + (hash << 7) + (hash << 8) + (hash << 24);
    }

    return hash;
}

static bool shashtab__is_prime(size_t const n)
{
    if (n <= 1)
    {
        return false;
    }
    if (n <= 3)
    {
        return true;
    }

    if (n % 2 == 0 || n % 3 == 0)
    {
        return false;
    }

    for (size_t i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
        {
            return false;
        }
    }

    return true;
}

static size_t shashtab__next_prime(size_t const n)
{
    for (size_t i = n; i < 2 * n; i++)
    {
        if (shashtab__is_prime(i))
        {
            return i;
        }
    }
    return n;
}

static size_t shashtab__optimal_capacity(size_t const length)
{
    /* ceil(length * 1.4) */
    return 7 * length / 5 + ((length % 2) != 0);
}

shashtab shashtab_new(size_t const capacity)
{
    shashtab ht = {.data = NULL, .capacity = 0};
    /* about 70 % load factor */
    size_t const init_capacity = shashtab__next_prime(shashtab__optimal_capacity(capacity));

    if ((ht.data = (sbintree **) SHASHTAB_CALLOC(init_capacity, sizeof(sbintree *))) == NULL)
    {
        return ht;
    }

    ht.capacity = init_capacity;
    return ht;
}

bool shashtab_set(shashtab const ht,
                  void * const key,
                  size_t const key_len,
                  int (*key_func)(void *key, void *node_key),
                  void * const data)
{
    unsigned int const hash = shashtab__fnv_1a_32(key, key_len);
    sbintree_insert(&ht.data[hash % ht.capacity], key, key_func, data);
    return true;
}

void *shashtab_get(shashtab const ht,
                   void * const key,
                   size_t const key_len,
                   int (*key_func)(void *key, void *node_key))
{
    unsigned int const hash = shashtab__fnv_1a_32(key, key_len);
    sbintree *node = sbintree_search(ht.data[hash % ht.capacity], key, key_func);
    if (node == NULL)
    {
        return NULL;
    }
    return node->data;
}

void shashtab_del(shashtab const ht,
                  void * const key,
                  size_t const key_len,
                  int (*key_func)(void *key, void *node_key),
                  void (*free_func)(sbintree *))
{
    unsigned int const hash = shashtab__fnv_1a_32(key, key_len);
    sbintree *node = sbintree_search(ht.data[hash % ht.capacity], key, key_func);
    if (node == NULL)
    {
        return;
    }
    sbintree_remove(&ht.data[hash % ht.capacity], node, free_func);
}

void shashtab_free(shashtab ht, void (*free_func)(sbintree *))
{
    for (size_t i = 0; i < ht.capacity; i++)
    {
        if (ht.data[i] != NULL)
        {
            sbintree_free(&ht.data[i], free_func);
        }
    }

    SHASHTAB_FREE(ht.data);
    ht.capacity = 0;
}

#endif /*SHASHTAB_IMPLEMENTATION*/
