/**
 * LICENSE
 *
 *     This file is in the public domain and also 0BSD licensed.
 *     See end of file for more information.
 *
 * Compile-time options
 *
 *     #define SDARRAY_REALLOC(ptr,size) realloc(ptr,size)
 *     #define SDARRAY_FREE(ptr)         free(ptr)
 *
 *         These defines only need to be set in the file containing
 *         #define SDARRAY_IMPLEMENTATION.
 *
 *         By default, sdarray uses stdlib realloc() and free() for memory
 *         management. You can substitute your own functions instead by defining
 *         these symbols. You must either define both, or neither.
 */

#ifndef INCLUDE_SDARRAY_H
#define INCLUDE_SDARRAY_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#if defined(SDARRAY_REALLOC) && !defined(SDARRAY_FREE) || !defined(SDARRAY_REALLOC) && defined(SDARRAY_FREE)
#error "You must define both SDARRAY_REALLOC and SDARRAY_FREE, or neither."
#endif
#if !defined(SDARRAY_REALLOC) && !defined(SDARRAY_FREE)
#include <stdlib.h>
#define SDARRAY_REALLOC(ptr, size) realloc(ptr, size)
#define SDARRAY_FREE(ptr) free(ptr)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    void *data;
    size_t length;
    size_t capacity;
    size_t element_size;
} sdarray;

sdarray sdarray_new(size_t const element_size, size_t const init_capacity);
sdarray sdarray_wrap_ptr(void * const ptr, size_t const element_size, size_t const ptr_size);
bool sdarray_add_from(sdarray * const arr, const void * const src, size_t const length);
bool sdarray_add_sdarray(sdarray * const arr, sdarray const arr2);
bool sdarray_remove(sdarray * const arr, size_t const index);
void *sdarray_at(sdarray * const arr, size_t const index);
void sdarray_swap(sdarray *arr, sdarray *arr2);
bool sdarray_set_capacity(sdarray * const arr, size_t const capacity);
sdarray sdarray_clone(sdarray const arr);

#ifdef __cplusplus
}
#endif

#endif /*INCLUDE_SDARRAY_H*/

#ifdef SDARRAY_IMPLEMENTATION

sdarray sdarray_new(size_t const element_size, size_t const init_capacity)
{
    sdarray arr = {.data = NULL, .length = 0, .capacity = 0, .element_size = element_size};

    if (init_capacity <= 0)
    {
        return arr;
    }

    if ((arr.data = (void *) SDARRAY_REALLOC(NULL, sizeof(uint8_t) * element_size * init_capacity)) == NULL)
    {
        return arr;
    }

    arr.capacity = init_capacity;

    return arr;
}

sdarray sdarray_wrap_ptr(void * const ptr, size_t const element_size, size_t const ptr_size)
{
    sdarray arr = {.data = ptr, .length = ptr_size, .capacity = ptr_size, .element_size = element_size};
    return arr;
}

bool sdarray_add_from(sdarray * const arr, const void * const src, size_t const length)
{
    size_t const new_length = arr->length + length;
    /* ceil(new_length * 1.5) */
    size_t const new_capacity = 3 * new_length / 2 + ((new_length % 2) != 0);

    if (arr->data == NULL)
    {

        if ((arr->data = (void *) SDARRAY_REALLOC(NULL, sizeof(uint8_t) * arr->element_size * length)) == NULL)
        {
            return false;
        }
    }

    if (new_length > arr->capacity)
    {
        if ((arr->data = (void *) SDARRAY_REALLOC(arr->data, sizeof(uint8_t) * arr->element_size * new_capacity)) == NULL)
        {
            return false;
        }
        arr->capacity = new_capacity;
    }

    if (length && src)
    {
        memcpy(((uint8_t *) arr->data) + arr->length * arr->element_size, src, sizeof(uint8_t) * arr->element_size * length);
    }
    arr->length = new_length;

    return true;
}

bool sdarray_add_sdarray(sdarray * const arr, sdarray const arr2)
{
    return sdarray_add_from(arr, arr2.data, arr2.length);
}

bool sdarray_remove(sdarray * const arr, size_t const index)
{
    if (index >= arr->length)
    {
        return false;
    }

    if (index + 1 == arr->length)
    {
        memset(((uint8_t *) arr->data) + index, 0, arr->element_size);
    }
    else
    {
        memmove(((uint8_t *) arr->data) + index * arr->element_size,
                ((uint8_t *) arr->data) + (index + 1) * arr->element_size,
                sizeof(uint8_t) * arr->element_size * (arr->length - index));
    }
    arr->length--;

    /* shrink the array when it reaches cap = 2 * len */
    if (arr->capacity / arr->length >= 2)
    {
        if ((arr->data = (uint8_t *) SDARRAY_REALLOC(arr->data, sizeof(uint8_t) * arr->element_size * arr->length)) == NULL)
        {
            return false;
        }
        arr->capacity = arr->length;
    }

    return true;
}

void *sdarray_at(sdarray * const arr, size_t const index)
{
    return (void *) (((uint8_t *) arr->data) + index * arr->element_size);
}

void sdarray_swap(sdarray *arr, sdarray *arr2)
{
    sdarray const tmp = *arr;
    *arr = *arr2;
    *arr2 = tmp;
}

bool sdarray_set_capacity(sdarray * const arr, size_t const capacity)
{
    if (capacity < arr->length)
    {
        return false;
    }

    if ((arr->data = (uint8_t *) SDARRAY_REALLOC(arr->data, sizeof(uint8_t) * arr->element_size * capacity)) == NULL)
    {
        return false;
    }
    arr->capacity = capacity;

    return true;
}

sdarray sdarray_clone(sdarray const arr)
{
    sdarray new_sdarray = {.data = NULL, .length = 0, .capacity = 0, .element_size = arr.element_size};
    sdarray_set_capacity(&new_sdarray, arr.capacity);
    sdarray_add_from(&new_sdarray, arr.data, arr.length);
    return new_sdarray;
}

#endif /*SDARRAY_IMPLEMENTATION*/

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
