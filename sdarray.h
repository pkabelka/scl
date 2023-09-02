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

#ifndef INCLUDE_SDARRAY_H
#define INCLUDE_SDARRAY_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
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
    sdarray arr = { .data = NULL, .length = 0, .capacity = 0, .element_size = element_size };

    if (init_capacity <= 0)
    {
        return arr;
    }

    if ((arr.data = (void *) malloc(sizeof(char) * element_size * init_capacity)) == NULL)
    {
        return arr;
    }

    arr.capacity = init_capacity;

    return arr;
}

sdarray sdarray_wrap_ptr(void * const ptr, size_t const element_size, size_t const ptr_size)
{
    sdarray arr = { .data = ptr, .length = ptr_size, .capacity = ptr_size, .element_size = element_size };
    return arr;
}

bool sdarray_add_from(sdarray * const arr, const void * const src, size_t const length)
{
    size_t const new_length = arr->length + length;
    /* ceil(new_length * 1.5) */
    size_t const new_capacity = 3*new_length/2 + ((new_length % 2) != 0);

    if (arr->data == NULL)
    {

        if ((arr->data = (void *) malloc(sizeof(char) * arr->element_size * length)) == NULL)
        {
            return false;
        }
    }

    if (new_length > arr->capacity)
    {
        if ((arr->data = (void *) realloc(arr->data, sizeof(char) * arr->element_size * new_capacity)) == NULL)
        {
            return false;
        }
        arr->capacity = new_capacity;
    }

    if (length && src)
    {
        memcpy(((char *) arr->data) + arr->length * arr->element_size, src, sizeof(char) * arr->element_size * length);
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
        memset(((char *) arr->data) + index, 0, arr->element_size);
    }
    else
    {
        memmove(((char *) arr->data) + index * arr->element_size,
                ((char *) arr->data) + (index + 1) * arr->element_size,
                sizeof(char) * arr->element_size * (arr->length - index));
    }
    arr->length--;

    /* shrink the array when it reaches cap = 2 * len */
    if (arr->capacity / arr->length >= 2)
    {
        if ((arr->data = (char *) realloc(arr->data, sizeof(char) * arr->element_size * arr->length)) == NULL)
        {
            return false;
        }
        arr->capacity = arr->length;
    }

    return true;
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

    if ((arr->data = (char *) realloc(arr->data, sizeof(char) * arr->element_size * capacity)) == NULL)
    {
        return false;
    }
    arr->capacity = capacity;

    return true;
}

sdarray sdarray_clone(sdarray const arr)
{
    sdarray new_sdarray = { .data = NULL, .length = 0, .capacity = 0, .element_size = arr.element_size };
    sdarray_set_capacity(&new_sdarray, arr.capacity);
    sdarray_add_from(&new_sdarray, arr.data, arr.length);
    return new_sdarray;
}

#endif /*SDARRAY_IMPLEMENTATION*/
