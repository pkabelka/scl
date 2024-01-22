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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Simple string type. Capacity includes the NULL ('\0') terminator.
 */
typedef struct
{
    char *cstr;
    size_t length;
    size_t capacity;
} sstr;

/**
 * Calculates the optimal capacity based on the passed length. The optimal
 * capacity ceil((new_length+1) * 1.5) should achieve an amortized constant
 * time.
 *
 * @param length Desired string length.
 * @return Optimal string capacity including the NULL ('\0') terminator.
 */
size_t sstr_optimal_capacity(size_t length);

/**
 * Allocates a new `sstr` and copies the string literal to the `.cstr` field.
 *
 * @param init_string String to wrap.
 * @return New `sstr`.
 */
sstr sstr_new(char const * const init_string);

/**
 * Frees the string. Can be called repeatedly.
 *
 * @param s String to free.
 */
void sstr_free(sstr * const s);

/**
 * Sets the strings length and first char to `0`.
 *
 * @param s String to empty.
 */
void sstr_empty(sstr * const s);

/**
 * Appends the `src` to the string up to `length` chars. The `.cstr` or
 * `.capacity` field of `s` can even be `0`. If there's not enough capacity, the
 * function automatically reallocates the string.
 *
 * @param s The string to append to.
 * @param src Appended C string.
 * @param length Length of the appended C string.
 * @return `true` upon success, `false` otherwise.
 */
bool sstr_add(sstr * const s, const void * const src, size_t const length);

/**
 * Appends the `s2` to the sstr `s`. The `.cstr` or
 * `.capacity` field of `s` can even be `0`. If there's not enough capacity, the
 * function automatically reallocates the string.
 *
 * @param s The string to append to.
 * @param s2 Appended C string.
 * @return `true` upon success, `false` otherwise.
 */
bool sstr_add_const(sstr * const s, const char * const s2);

/**
 * Appends the sstr `s2` to the sstr `s`. The `.cstr` or
 * `.capacity` field of `s` can even be `0`. If there's not enough capacity, the
 * function automatically reallocates the string.
 *
 * @param s The string to append to.
 * @param s2 Appended sstr.
 * @return `true` upon success, `false` otherwise.
 */
bool sstr_add_sstr(sstr * const s, sstr const s2);

/**
 * Appends the char to the sstr `s`. The `.cstr` or
 * `.capacity` field of `s` can even be `0`. If there's not enough capacity, the
 * function automatically reallocates the string.
 *
 * @param s The string to append to.
 * @param c Appended character.
 * @return `true` upon success, `false` otherwise.
 */
bool sstr_add_char(sstr * const s, char const c);

/**
 * Compares the sstr `s` with `s2` using `memcmp`.
 *
 * @param s First string to compare.
 * @param s2 Second string to compare.
 * @return `<0` the first character that does not match has a lower value in `s` than in `s2`.
 * @return `0` the contents of both strings are equal.
 * @return `>0` the first character that does not match has a greater value in `s` than in `s2`.
 */
int sstr_cmp(sstr const s, sstr const s2);

/**
 * Compares the sstr `s` with `s2` using `strcmp`.
 *
 * @param s sstr to compare.
 * @param s2 C string to compare with.
 * @return `<0` the first character that does not match has a lower value in `s` than in `s2`.
 * @return `0` the contents of both strings are equal.
 * @return `>0` the first character that does not match has a greater value in `s` than in `s2`.
 */
int sstr_cmp_const(sstr const s, const char * const s2);

/**
 * Swaps the strings.
 */
void sstr_swap(sstr *s, sstr *s2);

/**
 * Reallocates the string's capacity. The new capacity MUST be larger than the
 * string's length + 1 for the NULL ('\0') terminator.
 *
 * @param s sstr to reallocate.
 * @param capacity New capacity.
 * @return `true` upon success, `false` otherwise.
 */
bool sstr_set_capacity(sstr * const s, size_t const capacity);

/**
 * Allocates a new empty sstr with the desired capacity.
 *
 * @param capacity Initial capacity.
 * @return `true` upon success, `false` otherwise.
 */
sstr sstr_new_empty(size_t const capacity);

/**
 * Clones the sstr to a newly allocated string. The new string has even the same
 * capacity.
 *
 * @param s sstr to clone.
 * @return Cloned string.
 */
sstr sstr_clone(sstr const s);

/**
 * Allocates a new slice of the string.
 *
 * @param s sstr to cut.
 * @param start Starting index.
 * @param length Substring length.
 * @return Slice of the string.
 */
sstr sstr_substr(sstr const s, size_t const start, size_t const length);

/**
 * Checks if the string's prefix matches `prefix`.
 *
 * @param s sstr to check.
 * @param prefix Prefix to find.
 * @return `true` if the prefix matches, `false` otherwise.
 */
bool sstr_has_prefix(sstr const s, const char * const prefix);

/**
 * Checks if the string's suffix matches `suffix`.
 *
 * @param s sstr to check.
 * @param suffix Suffix to find.
 * @return `true` if the suffix matches, `false` otherwise.
 */
bool sstr_has_suffix(sstr const s, const char * const suffix);

/**
 * Checks if the string's prefix matches `prefix`.
 *
 * @param s sstr to check.
 * @param prefix Prefix to find.
 * @return `true` if the prefix matches, `false` otherwise.
 */
bool sstr_has_prefix_sstr(sstr const s, sstr const prefix);

/**
 * Checks if the string's suffix matches `suffix`.
 *
 * @param s sstr to check.
 * @param suffix Suffix to find.
 * @return `true` if the suffix matches, `false` otherwise.
 */
bool sstr_has_suffix_sstr(sstr const s, sstr const suffix);

/**
 * Allocates a new sstr without the prefix characters in `trim_char_set`. Stops
 * trimming upon first prefix character not in `trim_char_set`.
 *
 * @param s sstr to trim.
 * @param trim_char_set A C string containing the characters to trim from left.
 * @return New sstr without the characters trimmed from left.
 */
sstr sstr_trim_left(sstr const s, const char * const trim_char_set);

/**
 * Allocates a new sstr without the prefix characters in `trim_char_set`. Stops
 * trimming upon first prefix character not in `trim_char_set`.
 *
 * @param s sstr to trim.
 * @param trim_char_set An sstr containing the characters to trim from left.
 * @return New sstr without the characters trimmed from left.
 */
sstr sstr_trim_left_sstr(sstr const s, sstr const trim_char_set);

/**
 * Allocates a new sstr without the suffix characters in `trim_char_set`. Stops
 * trimming upon first suffix character not in `trim_char_set`.
 *
 * @param s sstr to trim.
 * @param trim_char_set A C string containing the characters to trim from right.
 * @return New sstr without the characters trimmed from right.
 */
sstr sstr_trim_right(sstr const s, const char * const trim_char_set);

/**
 * Allocates a new sstr without the suffix characters in `trim_char_set`. Stops
 * trimming upon first suffix character not in `trim_char_set`.
 *
 * @param s sstr to trim.
 * @param trim_char_set An sstr containing the characters to trim from right.
 * @return New sstr without the characters trimmed from right.
 */
sstr sstr_trim_right_sstr(sstr const s, sstr trim_char_set);

/**
 * Finds the index of the first occurrence of `c`. Check if the function
 * returned `true` before using `index`.
 *
 * @param s sstr to search.
 * @param c Searched character.
 * @param index Index of the found character.
 * @return `true` if the character is present, `false` otherwise.
 */
bool sstr_index_of(sstr const s, char const c, size_t * const index);

/**
 * Finds the index of the last occurrence of `c`. Check if the function
 * returned `true` before using `index`.
 *
 * @param s sstr to search.
 * @param c Searched character.
 * @param index Index of the found character.
 * @return `true` if the character is present, `false` otherwise.
 */
bool sstr_index_of_last(sstr const s, char const c, size_t * const index);

/**
 * Allocates a new sstr with all occurrences of `old_str` replaced with
 * `new_str`. Requires the function `memmem`.
 *
 * @param s Original sstr to search through.
 * @param old_str The replaced sstr.
 * @param new_str The replacement sstr.
 * @return New sstr with all occurrences of `old_str` replaced with
 * `new_str`.
 */
sstr sstr_replace(sstr const s, sstr const old_str, sstr const new_str);

/**
 * Allocates a new sstr with all occurrences of `old_str` replaced with
 * `new_str`. Requires the function `memmem`.
 *
 * @param s Original sstr to search through.
 * @param old_str The replaced C string.
 * @param new_str The replacement C string.
 * @return New sstr with all occurrences of `old_str` replaced with
 * `new_str`.
 */
sstr sstr_replace_const(sstr const s, const char * const old_str, const char * const new_str);

/**
 * Counts the number of occurrences of `searched` in sstr `s`.
 *
 * @param s sstr to search through.
 * @param searched String to count.
 * @param searched_len Length of the searched string in bytes.
 * @return Number of occurrences of the searched string.
 */
size_t sstr_count(sstr const s, const void * const searched, size_t const searched_len);

/**
 * Counts the number of occurrences of `searched` in sstr `s`.
 *
 * @param s sstr to search through.
 * @param searched String to count.
 * @return Number of occurrences of the searched string.
 */
size_t sstr_count_sstr(sstr const s, sstr const searched);

/**
 * Counts the number of occurrences of `searched` in sstr `s`.
 *
 * @param s sstr to search through.
 * @param searched String to count.
 * @return Number of occurrences of the searched string.
 */
size_t sstr_count_const(sstr const s, const char * const searched);

#ifdef __cplusplus
}
#endif

#endif /*INCLUDE_SSTR_H*/

#ifdef SSTR_IMPLEMENTATION

size_t sstr_optimal_capacity(size_t length)
{
    /* ceil((new_length+1) * 1.5) */
    return 3*(length+1)/2 + (((length+1) % 2) != 0);
}

sstr sstr_new(char const * const init_string)
{
    sstr s = { .cstr = NULL, .length = 0, .capacity = 0 };
    size_t const init_string_len = strlen(init_string);
    size_t const init_capacity = sstr_optimal_capacity(init_string_len);

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

void sstr_free(sstr * const s)
{
    if (s->cstr != NULL)
    {
        free(s->cstr);
        s->cstr = NULL;
    }
    s->length = 0;
    s->capacity = 0;
}

void sstr_empty(sstr * const s)
{
    s->cstr[0] = '\0';
    s->length = 0;
}

bool sstr_add(sstr * const s, const void * const src, size_t const length)
{
    size_t const new_length = s->length + length;
    size_t const new_capacity = sstr_optimal_capacity(new_length);

    if (new_length+1 > s->capacity)
    {
        if ((s->cstr = (char *) realloc(s->cstr, sizeof(char) * new_capacity)) == NULL)
        {
            return false;
        }
        s->capacity = new_capacity;
    }

    if (length && src)
    {
        memcpy(s->cstr+s->length, src, length);
    }
    s->length = new_length;
    s->cstr[s->length] = '\0';

    return true;
}

bool sstr_add_const(sstr * const s, const char * const s2)
{
    return sstr_add(s, s2, strlen(s2));
}

bool sstr_add_sstr(sstr * const s, sstr const s2)
{
    return sstr_add(s, s2.cstr, s2.length);
}

bool sstr_add_char(sstr * const s, char const c)
{
    return sstr_add(s, &c, 1);
}

int sstr_cmp(sstr const s, sstr const s2)
{
    if (s.length < s2.length)
    {
        return -1;
    }
    else if (s.length > s2.length)
    {
        return 1;
    }
    return memcmp(s.cstr, s2.cstr, s.length);
}

int sstr_cmp_const(sstr const s, const char * const s2)
{
    return strcmp(s.cstr, s2);
}

void sstr_swap(sstr *s, sstr *s2)
{
    sstr const tmp = *s;
    *s = *s2;
    *s2 = tmp;
}

bool sstr_set_capacity(sstr * const s, size_t const capacity)
{
    size_t const new_capacity = sizeof(char) * capacity;

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

sstr sstr_new_empty(size_t const capacity)
{
    sstr new_sstr = { .cstr = NULL, .length = 0, .capacity = 0 };
    sstr_set_capacity(&new_sstr, capacity);
    return new_sstr;
}

sstr sstr_clone(sstr const s)
{
    sstr new_sstr = sstr_new_empty(0);
    sstr_set_capacity(&new_sstr, s.capacity);
    sstr_add(&new_sstr, s.cstr, s.length);
    return new_sstr;
}

sstr sstr_substr(sstr const s, size_t const start, size_t const length)
{
    sstr new_sstr = sstr_new_empty(0);
    if ((start < s.length) && (s.length - start >= length))
    {
        sstr_add(&new_sstr, s.cstr+start, length);
    }
    return new_sstr;
}

bool sstr_has_prefix(sstr const s, const char * const prefix)
{
    size_t const prefix_len = strlen(prefix);
    if (prefix_len > s.length)
    {
        return false;
    }
    return memcmp(s.cstr, prefix, prefix_len) == 0;
}

bool sstr_has_suffix(sstr const s, const char * const suffix)
{
    size_t const suffix_len = strlen(suffix);
    if (suffix_len > s.length)
    {
        return false;
    }
    return memcmp(s.cstr + s.length - suffix_len, suffix, suffix_len) == 0;
}

bool sstr_has_prefix_sstr(sstr const s, sstr const prefix)
{
    if (prefix.length > s.length)
    {
        return false;
    }
    return memcmp(s.cstr, prefix.cstr, prefix.length) == 0;
}

bool sstr_has_suffix_sstr(sstr const s, sstr const suffix)
{
    if (suffix.length > s.length)
    {
        return false;
    }
    return memcmp(s.cstr + s.length - suffix.length, suffix.cstr, suffix.length) == 0;
}

static bool _sstr_const_contains_char(const char * const string, char const ch)
{
    for (char const *c = string; *c; c++)
    {
        if (*c == ch)
        {
            return true;
        }
    }
    return false;
}

sstr sstr_trim_left(sstr const s, const char * const trim_char_set)
{
    size_t char_count = 0;
    for (size_t i = 0; i < s.length; i++)
    {
        if (_sstr_const_contains_char(trim_char_set, s.cstr[i]))
        {
            char_count++;
        }
        else
        {
            break;
        }
    }

    sstr new_sstr = sstr_new_empty(0);
    sstr_add(&new_sstr, s.cstr + char_count, s.length - char_count);
    return new_sstr;
}

sstr sstr_trim_left_sstr(sstr const s, sstr const trim_char_set)
{
    size_t char_count = 0;
    size_t unused;
    for (size_t i = 0; i < s.length; i++)
    {
        if (sstr_index_of(trim_char_set, s.cstr[i], &unused))
        {
            char_count++;
        }
        else
        {
            break;
        }
    }

    sstr new_sstr = sstr_new_empty(0);
    sstr_add(&new_sstr, s.cstr + char_count, s.length - char_count);
    return new_sstr;
}

sstr sstr_trim_right(sstr const s, const char * const trim_char_set)
{
    size_t char_count = 0;
    for (size_t i = s.length - 1; i != 0; i--)
    {
        if (_sstr_const_contains_char(trim_char_set, s.cstr[i]))
        {
            char_count++;
        }
        else
        {
            break;
        }
    }

    sstr new_sstr = sstr_new_empty(0);
    sstr_add(&new_sstr, s.cstr, s.length - char_count);
    return new_sstr;
}

sstr sstr_trim_right_sstr(sstr const s, sstr trim_char_set)
{
    size_t char_count = 0;
    size_t unused;
    for (size_t i = s.length - 1; i != 0; i--)
    {
        if (sstr_index_of(trim_char_set, s.cstr[i], &unused))
        {
            char_count++;
        }
        else
        {
            break;
        }
    }

    sstr new_sstr = sstr_new_empty(0);
    sstr_add(&new_sstr, s.cstr, s.length - char_count);
    return new_sstr;
}

bool sstr_index_of(sstr const s, char const c, size_t * const index)
{
    for (size_t i = 0; i < s.length; i++)
    {
        if (c == s.cstr[i])
        {
            *index = i;
            return true;
        }
    }
    return false;
}

bool sstr_index_of_last(sstr const s, char const c, size_t * const index)
{
    for (size_t i = s.length - 1; i != 0; i--)
    {
        if (c == s.cstr[i])
        {
            *index = i;
            return true;
        }
    }
    return false;
}

size_t sstr_count(sstr const s, const void * const searched, size_t const searched_len)
{
    char *next_occurrence = s.cstr;
    size_t count = 0;
    char *tmp;
    while ((tmp = (char *) memmem(next_occurrence, (size_t) (s.cstr + s.length - next_occurrence), searched, searched_len)))
    {
        next_occurrence = tmp + searched_len;
        count++;
    }
    return count;
}

size_t sstr_count_sstr(sstr const s, sstr const searched)
{
    return sstr_count(s, searched.cstr, searched.length);
}

size_t sstr_count_const(sstr const s, const char * const searched)
{
    return sstr_count(s, searched, strlen(searched));
}

sstr sstr_replace(sstr const s, sstr const old_str, sstr const new_str)
{
    size_t const old_str_len = old_str.length;
    size_t const new_str_len = new_str.length;

    /* place `new_str` at each "empty" character */
    if (old_str_len == 0)
    {
        size_t const new_length = new_str_len + s.length + s.length * new_str_len;
        size_t const new_capacity = sstr_optimal_capacity(new_length);

        sstr replaced = sstr_new_empty(new_capacity);
        if (replaced.capacity == 0 || replaced.cstr == NULL)
        {
            return sstr_new_empty(0);
        }
        replaced.length = new_length;

        char *new_moving_ptr = replaced.cstr;
        for (size_t i = 0; i < s.length; i++)
        {
            new_moving_ptr = (char *) memcpy(new_moving_ptr, new_str.cstr, new_str_len) + new_str_len;
            *new_moving_ptr++ = s.cstr[i];
        }
        memcpy(new_moving_ptr, new_str.cstr, new_str_len);
        replaced.cstr[new_length] = '\0';

        return replaced;
    }

    size_t const replacement_count = sstr_count(s, old_str.cstr, old_str_len);
    size_t const new_length = s.length + (new_str_len - old_str_len) * replacement_count;
    size_t const new_capacity = sstr_optimal_capacity(new_length);

    sstr replaced = sstr_new_empty(new_capacity);
    if (replaced.capacity == 0 || replaced.cstr == NULL)
    {
        return sstr_new_empty(0);
    }
    replaced.length = new_length;

    char *new_moving_ptr = replaced.cstr;
    char *orig_moving_ptr = s.cstr;
    size_t len_to_next_replacement = 0;
    for (size_t i = 0; i < replacement_count; i++)
    {
        char * const next_occurrence = (char *) memmem(orig_moving_ptr, (size_t) (s.cstr + s.length - orig_moving_ptr), old_str.cstr, old_str_len);
        len_to_next_replacement = (size_t) (next_occurrence - orig_moving_ptr);
        new_moving_ptr = (char *) memcpy(new_moving_ptr, orig_moving_ptr, len_to_next_replacement) + len_to_next_replacement;
        new_moving_ptr = (char *) memcpy(new_moving_ptr, new_str.cstr, new_str_len) + new_str_len;
        orig_moving_ptr += len_to_next_replacement + old_str_len;
    }
    memcpy(new_moving_ptr, orig_moving_ptr, (size_t) (s.cstr + s.length - orig_moving_ptr));
    replaced.cstr[new_length] = '\0';

    return replaced;
}

sstr sstr_replace_const(sstr const s, const char * const old_str, const char * const new_str)
{
    sstr old_sstr = sstr_new(old_str);
    sstr new_sstr = sstr_new(new_str);
    sstr const replaced = sstr_replace(s, old_sstr, new_sstr);
    sstr_free(&old_sstr);
    sstr_free(&new_sstr);
    return replaced;
}

#endif /*SSTR_IMPLEMENTATION*/
