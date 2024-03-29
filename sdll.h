/**
 * LICENSE
 *
 *     This file is in the public domain and also 0BSD licensed.
 *     See end of file for more information.
 *
 * Compile-time options
 *
 *     #define SDLL_MALLOC(size) malloc(size)
 *     #define SDLL_FREE(ptr)    free(ptr)
 *
 *         These defines only need to be set in the file containing
 *         #define SDLL_IMPLEMENTATION.
 *
 *         By default, sdll uses stdlib malloc() and free() for memory
 *         management. You can substitute your own functions instead by defining
 *         these symbols. You must either define both, or neither.
 */

#ifndef INCLUDE_SDLL_H
#define INCLUDE_SDLL_H

#if defined(SDLL_MALLOC) && !defined(SDLL_FREE) || !defined(SDLL_MALLOC) && defined(SDLL_FREE)
#error "You must define both SDLL_MALLOC and SDLL_FREE, or neither."
#endif
#if !defined(SDLL_MALLOC) && !defined(SDLL_FREE)
#include <stdlib.h>
#define SDLL_MALLOC(size) malloc(size)
#define SDLL_FREE(ptr) free(ptr)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct sdll_node
{
    struct sdll_node *next;
    struct sdll_node *prev;
    void *data;
} sdll_node;

typedef struct
{
    sdll_node *first;
    sdll_node *last;
    size_t length;
} sdll;

sdll *sdll_new();
void sdll_empty(sdll * const l, void (*free_func)(void *));
void sdll_free(sdll **l, void (*free_func)(void *));
void sdll_insert_after(sdll * const l, sdll_node *node, void * const data);
void sdll_insert_before(sdll * const l, sdll_node *node, void * const data);
void sdll_insert_first(sdll * const l, void * const data);
void sdll_insert_last(sdll * const l, void * const data);
void sdll_unlink(sdll * const l, sdll_node * const node);
void sdll_remove(sdll * const l, sdll_node **node, void (*free_func)(void *));
void sdll_dummy_free(void * const data);

#ifdef __cplusplus
}
#endif

#endif /*INCLUDE_SDLL_H*/

#ifdef SDLL_IMPLEMENTATION

sdll *sdll_new()
{
    sdll *l = (sdll *) SDLL_MALLOC(sizeof(sdll));
    if (l == NULL)
    {
        return NULL;
    }

    l->first = NULL;
    l->last = NULL;
    l->length = 0;

    return l;
}

void sdll_empty(sdll * const l, void (*free_func)(void *))
{
    if (l == NULL || l->first == NULL)
        return;

    sdll_node *current;
    sdll_node *next = l->first;

    while (next != NULL)
    {
        current = next;
        next = current->next;

        free_func(current->data);
        SDLL_FREE(current);
    }

    l->first = NULL;
    l->last = NULL;
    l->length = 0;
}

void sdll_free(sdll **l, void (*free_func)(void *))
{
    sdll_empty(*l, free_func);
    SDLL_FREE(*l);
    *l = NULL;
}

void sdll_insert_after(sdll * const l, sdll_node *node, void * const data)
{
    if (l == NULL)
        return;

    sdll_node *new_node = (sdll_node *) SDLL_MALLOC(sizeof(sdll_node));
    new_node->prev = node;
    new_node->data = data;

    if (node->next == NULL)
    {
        new_node->next = NULL;
        l->last = new_node;
    }
    else
    {
        new_node->next = node->next;
        node->next->prev = new_node;
    }

    node->next = new_node;
    l->length++;
}

void sdll_insert_before(sdll * const l, sdll_node *node, void * const data)
{
    if (l == NULL)
        return;

    sdll_node *new_node = (sdll_node *) SDLL_MALLOC(sizeof(sdll_node));
    new_node->next = node;
    new_node->data = data;

    if (node->prev == NULL)
    {
        new_node->prev = NULL;
        l->first = new_node;
    }
    else
    {
        new_node->prev = node->prev;
        node->prev->next = new_node;
    }

    node->prev = new_node;
    l->length++;
}

void sdll_insert_first(sdll * const l, void * const data)
{
    if (l == NULL)
        return;

    if (l->first == NULL)
    {
        sdll_node *new_node = (sdll_node *) SDLL_MALLOC(sizeof(sdll_node));
        new_node->data = data;

        l->first = new_node;
        l->last = new_node;
        new_node->next = NULL;
        new_node->prev = NULL;
        l->length++;
    }
    else
    {
        sdll_insert_before(l, l->first, data);
    }
}

void sdll_insert_last(sdll * const l, void * const data)
{
    if (l == NULL)
        return;

    if (l->last == NULL)
    {
        sdll_insert_first(l, data);
    }
    else
    {
        sdll_insert_after(l, l->last, data);
    }
}

void sdll_unlink(sdll * const l, sdll_node * const node)
{
    if (l == NULL)
        return;

    if (node->prev == NULL)
    {
        l->first = node->next;
    }
    else
    {
        node->prev->next = node->next;
    }

    if (node->next == NULL)
    {
        l->last = node->prev;
    }
    else
    {
        node->next->prev = node->prev;
    }
    l->length--;
}

void sdll_remove(sdll * const l, sdll_node **node, void (*free_func)(void *))
{
    sdll_unlink(l, *node);
    free_func((*node)->data);
    SDLL_FREE(*node);
    *node = NULL;
}

void sdll_dummy_free(void * const data)
{
    (void) data;
}

#endif /*SDLL_IMPLEMENTATION*/

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
