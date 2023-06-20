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

#ifndef INCLUDE_SBINTREE_H
#define INCLUDE_SBINTREE_H

#include <stdio.h>
#include <stdlib.h>
#include "sdll.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sbintree
{
    struct sbintree *left;
    struct sbintree *right;
    void *key;
    void *data;
} sbintree;

sbintree* sbintree_insert(sbintree ** const root, void * const key, int (*key_func)(void *key, void *node_key), void * const data);
sbintree* sbintree_search(sbintree * const root, void * const key, int (*key_func)(void *searched_key, void *node_key));
void sbintree_inorder(sbintree * const root, void (*callback)(sbintree * const node));
void sbintree_free(sbintree ** const root, void (*free_func)(sbintree*));

#ifdef __cplusplus
}
#endif

#endif /*INCLUDE_SBINTREE_H*/

#ifdef SBINTREE_IMPLEMENTATION

sbintree* sbintree_insert(sbintree ** const root, void * const key, int (*key_func)(void *key, void *node_key), void * const data)
{
    if (*root == NULL)
    {
        sbintree *new_node = (sbintree*) malloc(sizeof(sbintree));
        if (new_node == NULL)
        {
            return NULL;
        }

        new_node->left = NULL;
        new_node->right = NULL;
        new_node->key = key;
        new_node->data = data;

        *root = new_node;
        return new_node;
    }

    sbintree *current = *root;
    while (current != NULL)
    {
        int cmp = key_func(key, current->key);
        /* left subtree */
        if (cmp < 0)
        {
            if (current->left != NULL)
            {
                current = current->left;
                continue;
            }

            sbintree *new_node = (sbintree*) malloc(sizeof(sbintree));
            if (new_node == NULL)
            {
                return NULL;
            }

            new_node->left = NULL;
            new_node->right = NULL;
            new_node->key = key;
            new_node->data = data;

            current->left = new_node;
            return new_node;
        }
        /* right subtree */
        else if (cmp > 0)
        {
            if (current->right != NULL)
            {
                current = current->right;
                continue;
            }

            sbintree *new_node = (sbintree*) malloc(sizeof(sbintree));
            if (new_node == NULL)
            {
                return NULL;
            }

            new_node->left = NULL;
            new_node->right = NULL;
            new_node->key = key;
            new_node->data = data;

            current->right = new_node;
            return new_node;
        }
        else
        {
            return NULL;
        }
    }
    return NULL;
}

sbintree* sbintree_search(sbintree * const root, void * const key, int (*key_func)(void *searched_key, void *node_key))
{
    if (root == NULL)
    {
        return NULL;
    }

    sbintree *current = root;
    while (current != NULL)
    {
        int cmp = key_func(key, current->key);
        /* left subtree */
        if (cmp < 0)
        {
            current = current->left;
        }
        /* right subtree */
        else if (cmp > 0)
        {
            current = current->right;
        }
        else
        {
            return current;
        }
    }
    return NULL;
}

static void sbintree_left_inorder(sbintree * const root, sdll * const list)
{
    for (sbintree *current = root; current != NULL; current = current->left)
    {
        sdll_insert_first(list, current);
    }
}

void sbintree_inorder(sbintree * const root, void (*callback)(sbintree * const node))
{
    if (root == NULL) return;

    sdll *list = sdll_new();
    sbintree_left_inorder(root, list);

    while (list->first != NULL)
    {
        sdll_node *first = list->first;
        sdll_unlink(list, list->first);
        sbintree_left_inorder(((sbintree*)first->data)->right, list);
        callback((sbintree*)first->data);
        free(first);
    }

    sdll_free(&list, sdll_dummy_free);
}

void sbintree_free(sbintree ** const root, void (*free_func)(sbintree*))
{
    sbintree_inorder(*root, free_func);
    *root = NULL;
}

#endif /*SBINTREE_IMPLEMENTATION*/
