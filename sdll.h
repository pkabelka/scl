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

#ifndef INCLUDE_SDLL_H
#define INCLUDE_SDLL_H

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
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

sdll* sdll_new();
void sdll_empty(sdll * const l, void (*free_func)(void*));
void sdll_free(sdll **l, void (*free_func)(void*));
void sdll_insert_after(sdll * const l, sdll_node *node, void * const data);
void sdll_insert_before(sdll * const l, sdll_node *node, void * const data);
void sdll_insert_first(sdll * const l, void * const data);
void sdll_insert_last(sdll * const l, void * const data);
void sdll_unlink(sdll * const l, sdll_node * const node);

#ifdef __cplusplus
}
#endif

#endif /*INCLUDE_SDLL_H*/

#ifdef SDLL_IMPLEMENTATION

sdll* sdll_new()
{
    sdll *l = (sdll*) malloc(sizeof(sdll));
    if (l == NULL)
    {
        return NULL;
    }

    l->first = NULL;
    l->last = NULL;
    l->length = 0;

    return l;
}

void sdll_empty(sdll * const l, void (*free_func)(void*))
{
    if (l == NULL || l->first == NULL) return;

    sdll_node *current;
    sdll_node *next = l->first;

    while (next != NULL)
    {
        current = next;
        next = current->next;

        free_func(current->data);
        free(current);
    }

    l->first = NULL;
    l->last = NULL;
    l->length = 0;
}

void sdll_free(sdll **l, void (*free_func)(void*))
{
    sdll_empty(*l, free_func);
    free(*l);
    *l = NULL;
}

void sdll_insert_after(sdll * const l, sdll_node *node, void * const data)
{
    if (l == NULL) return;

    sdll_node *new_node = (sdll_node*) malloc(sizeof(sdll_node));
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
    if (l == NULL) return;

    sdll_node *new_node = (sdll_node*) malloc(sizeof(sdll_node));
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
    if (l == NULL) return;

    if (l->first == NULL)
    {
        sdll_node *new_node = (sdll_node*) malloc(sizeof(sdll_node));
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
    if (l == NULL) return;

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
    if (l == NULL) return;

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

#endif /*SDLL_IMPLEMENTATION*/
