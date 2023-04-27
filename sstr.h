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

#ifndef INCLUDE_SSTR_H
#define INCLUDE_SSTR_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifndef SSTR_ALLOC_SIZE
#define SSTR_ALLOC_SIZE 32
#endif

typedef struct
{
    char *cstr;
    size_t length;
    size_t capacity;
} sstr;

#endif /*INCLUDE_SSTR_H*/

#ifdef SSTR_IMPLEMENTATION

static inline sstr sstr_new(char const * const init_string)
{
    sstr s = { .cstr = NULL, .length = 0, .capacity = 0 };
    size_t init_string_len = strlen(init_string);
    size_t init_capacity = init_string_len+1 < SSTR_ALLOC_SIZE ? SSTR_ALLOC_SIZE : init_string_len+1;

    if ((s.cstr = (char *) malloc(sizeof(char) * init_capacity)) == NULL)
    {
        return s;
    }

    if (init_string_len && init_string)
    {
        memcpy(s.cstr, init_string, init_string_len);
    }

    s.cstr[init_string_len] = '\0';
    s.length = init_string_len;
    s.capacity = init_capacity;

    return s;
}

static inline void sstr_free(sstr * const s)
{
    free(s->cstr);
    s->cstr = NULL;
    s->length = 0;
    s->capacity = 0;
}

static inline void sstr_empty(sstr * const s)
{
    s->cstr[0] = '\0';
    s->length = 0;
}

static inline bool sstr_add_from(sstr * const s, const void * const src, size_t length)
{
    size_t new_length = s->length + length;
    size_t new_capacity = sizeof(char) * (new_length + 1 + SSTR_ALLOC_SIZE);

    if (new_length+1 > s->capacity)
    {
        if ((s->cstr = (char *) realloc(s->cstr, new_capacity)) == NULL)
        {
            return false;
        }
        s->capacity = new_capacity;
    }

    memcpy(s->cstr+s->length, src, length);
    s->length = new_length;
    s->cstr[s->length] = '\0';

    return true;
}

static inline bool sstr_add_const(sstr * const s, const char * const s2)
{
    size_t s2_length = strlen(s2);
    return sstr_add_from(s, s2, s2_length);
}

static inline bool sstr_add_sstr(sstr * const s, sstr const s2)
{
    return sstr_add_const(s, s2.cstr);
}

static inline bool sstr_add_char(sstr * const s, char const c)
{
    size_t new_length = s->length + 1;
    size_t new_capacity = sizeof(char) * (new_length + 1 + SSTR_ALLOC_SIZE);

    if ((new_length + 1) >= s->capacity)
    {
        if ((s->cstr = (char *) realloc(s->cstr, new_capacity)) == NULL)
        {
            return false;
        }
        s->capacity = new_capacity;
    }

    s->cstr[s->length] = c;
    s->length = new_length;
    s->cstr[s->length] = '\0';

    return true;
}

static inline int sstr_cmp(sstr const s, sstr const s2)
{
    return strcmp(s.cstr, s2.cstr);
}

static inline int sstr_cmp_const(sstr const s, const char * const s2)
{
    return strcmp(s.cstr, s2);
}

static inline void sstr_swap(sstr *s, sstr *s2)
{
    sstr tmp = *s;
    *s = *s2;
    *s2 = tmp;
}

static inline bool sstr_set_capacity(sstr * const s, size_t capacity)
{
    size_t new_capacity = sizeof(char) * capacity;

    if (new_capacity < (s->length + 1))
    {
        return false;
    }

    if ((s->cstr = (char *) realloc(s->cstr, new_capacity)) == NULL)
    {
        return false;
    }
    s->capacity = new_capacity;

    return true;
}

static inline sstr sstr_clone(sstr const s)
{
    sstr new_sstr = { .cstr = NULL, .length = 0, .capacity = 0 };
    sstr_set_capacity(&new_sstr, s.capacity);
    sstr_add_from(&new_sstr, s.cstr, s.length);
    return new_sstr;
}

static inline sstr sstr_substr(sstr * const s, size_t start, size_t length)
{
    sstr new_sstr = sstr_new("");
    sstr_add_from(&new_sstr, s->cstr+start, length);
    return new_sstr;
}

#endif /*SSTR_IMPLEMENTATION*/
