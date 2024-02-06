/**
 * LICENSE
 *
 *     This file is in the public domain and also 0BSD licensed.
 *     See end of file for more information.
 *
 * Compile-time options
 *
 *     #define STRIE_MALLOC(size) malloc(size)
 *     #define STRIE_FREE(ptr)    free(ptr)
 *
 *         These defines only need to be set in the file containing
 *         #define STRIE_IMPLEMENTATION.
 *
 *         By default, strie uses stdlib malloc() and free() for memory
 *         management. You can substitute your own functions instead by defining
 *         these symbols. You must either define both, or neither.
 */

#ifndef INCLUDE_STRIE_H
#define INCLUDE_STRIE_H

#include "sdll.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#if defined(STRIE_MALLOC) && !defined(STRIE_FREE) || !defined(STRIE_MALLOC) && defined(STRIE_FREE)
#error "You must define both STRIE_MALLOC and STRIE_FREE, or neither."
#endif
#if !defined(STRIE_MALLOC) && !defined(STRIE_FREE)
#include <stdlib.h>
#define STRIE_MALLOC(size) malloc(size)
#define STRIE_FREE(ptr) free(ptr)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct strie
{
    struct strie *prev;
    struct strie **next;
    size_t next_count;
    void *data;
} strie;

strie *strie_insert(strie ** const node, void * const key, size_t const key_length_bytes, void * const data);
strie *strie_search(strie * const node, void * const key, size_t const key_length_bytes);
bool strie_remove(strie ** const root, void * const key, size_t const key_length_bytes, void (*free_func)(void *));

#ifdef __cplusplus
}
#endif

#endif /*INCLUDE_STRIE_H*/

#ifdef STRIE_IMPLEMENTATION

strie *strie_insert(strie ** const root, void * const key, size_t const key_length_bytes, void * const data)
{
    if (*root == NULL)
    {
        *root = (strie *) STRIE_MALLOC(sizeof(strie));
        if (*root == NULL)
        {
            return NULL;
        }

        (*root)->prev = NULL;
        (*root)->data = NULL;
        (*root)->next_count = 0;
        (*root)->next = (strie **) STRIE_MALLOC(sizeof(strie *) * 256);
        if ((*root)->next == NULL)
        {
            return NULL;
        }
        memset((*root)->next, 0, 256);
    }

    unsigned char const * const key_bytes = (unsigned char const *) key;
    strie *current = *root;

    for (size_t i = 0; i < key_length_bytes; i++)
    {
        strie *next_node = current->next[key_bytes[i]];
        if (next_node == NULL)
        {
            next_node = (strie *) STRIE_MALLOC(sizeof(strie));
            if (next_node == NULL)
            {
                return NULL;
            }

            next_node->next = (strie **) STRIE_MALLOC(sizeof(strie *) * 256);
            if (next_node->next == NULL)
            {
                return NULL;
            }
            memset(next_node->next, 0, 256);
            next_node->prev = current;
            next_node->data = NULL;
            next_node->next_count = 0;

            current->next_count++;
            current->next[key_bytes[i]] = next_node;
        }

        current = next_node;
    }

    current->data = data;
    return current;
}

strie *strie_search(strie * const node, void * const key, size_t const key_length_bytes)
{
    if (node == NULL)
    {
        return NULL;
    }

    unsigned char const * const key_bytes = (unsigned char const *) key;
    strie *current = node;

    for (size_t i = 0; i < key_length_bytes; i++)
    {
        strie *next_node = current->next[key_bytes[i]];
        if (next_node == NULL)
        {
            return NULL;
        }

        current = next_node;
    }

    return current;
}

bool strie_remove(strie ** const root, void * const key, size_t const key_length_bytes, void (*free_func)(void *))
{
    if (*root == NULL)
    {
        return false;
    }

    unsigned char const * const key_bytes = (unsigned char const *) key;
    strie *current = strie_search(*root, key, key_length_bytes);
    if (current == NULL)
    {
        return false;
    }

    free_func(current->data);
    current->data = NULL;
    if (current->next_count > 0)
    {
        return false;
    }

    size_t i = key_length_bytes - 1;
    while (current->prev != (*root)->prev)
    {
        STRIE_FREE(current->next);
        current->next = NULL;
        current->prev->next[key_bytes[i]] = NULL;
        current->prev->next_count--;

        if (current->prev->next_count > 0)
        {
            STRIE_FREE(current);
            return true;
        }

        strie *tmp = current;
        current = current->prev;
        i--;

        STRIE_FREE(tmp);
    }

    STRIE_FREE((*root)->next);
    STRIE_FREE(*root);
    *root = NULL;

    return true;
}

#endif /*STRIE_IMPLEMENTATION*/

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
