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
 *     #define SBINTREE_MALLOC(size) malloc(size)
 *     #define SBINTREE_FREE(ptr)    free(ptr)
 *
 *         These defines only need to be set in the file containing
 *         #define SBINTREE_IMPLEMENTATION.
 *
 *         By default, sbintree uses stdlib malloc() and free() for memory
 *         management. You can substitute your own functions instead by defining
 *         these symbols. You must either define both, or neither.
 */

#ifndef INCLUDE_SBINTREE_H
#define INCLUDE_SBINTREE_H

#include "sdll.h"
#include <stdio.h>

#if defined(SBINTREE_MALLOC) && !defined(SBINTREE_FREE) || !defined(SBINTREE_MALLOC) && defined(SBINTREE_FREE)
#error "You must define both SBINTREE_MALLOC and SBINTREE_FREE, or neither."
#endif
#if !defined(SBINTREE_MALLOC) && !defined(SBINTREE_FREE)
#include <stdlib.h>
#define SBINTREE_MALLOC(size) malloc(size)
#define SBINTREE_FREE(ptr) free(ptr)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct sbintree
{
    struct sbintree *parent;
    struct sbintree *left;
    struct sbintree *right;
    void *key;
    void *data;
} sbintree;

sbintree *sbintree_insert(sbintree ** const root, void * const key, int (*key_func)(void *key, void *node_key), void * const data);
sbintree *sbintree_search(sbintree * const root, void * const key, int (*key_func)(void *searched_key, void *node_key));
void sbintree_inorder(sbintree * const root, void (*callback)(sbintree * const node));
void sbintree_free(sbintree ** const root, void (*free_func)(sbintree *));
sbintree *sbintree_leftmost(sbintree * const node);
sbintree *sbintree_rightmost(sbintree * const node);
sbintree *sbintree_successor(sbintree * const node);
sbintree *sbintree_predecessor(sbintree * const node);
void sbintree_remove(sbintree ** const root, sbintree * const node, void (*free_func)(sbintree *));

#ifdef __cplusplus
}
#endif

#endif /*INCLUDE_SBINTREE_H*/

#ifdef SBINTREE_IMPLEMENTATION

sbintree *sbintree_insert(sbintree ** const root, void * const key, int (*key_func)(void *key, void *node_key), void * const data)
{
    if (*root == NULL)
    {
        sbintree *new_node = (sbintree *) SBINTREE_MALLOC(sizeof(sbintree));
        if (new_node == NULL)
        {
            return NULL;
        }

        new_node->parent = NULL;
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

            sbintree *new_node = (sbintree *) SBINTREE_MALLOC(sizeof(sbintree));
            if (new_node == NULL)
            {
                return NULL;
            }

            new_node->parent = current;
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

            sbintree *new_node = (sbintree *) SBINTREE_MALLOC(sizeof(sbintree));
            if (new_node == NULL)
            {
                return NULL;
            }

            new_node->parent = current;
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

sbintree *sbintree_search(sbintree * const root, void * const key, int (*key_func)(void *searched_key, void *node_key))
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

static void sbintree__left_inorder(sbintree * const root, sdll * const list)
{
    for (sbintree *current = root; current != NULL; current = current->left)
    {
        sdll_insert_first(list, current);
    }
}

void sbintree_inorder(sbintree * const root, void (*callback)(sbintree * const node))
{
    if (root == NULL)
        return;

    sdll *list = sdll_new();
    sbintree__left_inorder(root, list);

    while (list->first != NULL)
    {
        sdll_node *first = list->first;
        sdll_unlink(list, list->first);
        sbintree__left_inorder(((sbintree *) first->data)->right, list);
        callback((sbintree *) first->data);
        SBINTREE_FREE(first);
    }

    sdll_free(&list, sdll_dummy_free);
}

void sbintree_free(sbintree ** const root, void (*free_func)(sbintree *))
{
    sbintree_inorder(*root, free_func);
    *root = NULL;
}

sbintree *sbintree_leftmost(sbintree * const node)
{
    sbintree *predecessor = node;
    for (sbintree *current = node; current != NULL; current = current->left)
    {
        predecessor = current;
    }
    return predecessor;
}

sbintree *sbintree_rightmost(sbintree * const node)
{
    sbintree *predecessor = node;
    for (sbintree *current = node; current != NULL; current = current->right)
    {
        predecessor = current;
    }
    return predecessor;
}

sbintree *sbintree_successor(sbintree * const node)
{
    if (node->right != NULL)
    {
        return sbintree_leftmost(node->right);
    }

    sbintree *current = node;
    sbintree *successor = node->parent;
    while (successor != NULL && current == successor->right)
    {
        current = successor;
        successor = successor->parent;
    }
    return successor;
}

sbintree *sbintree_predecessor(sbintree * const node)
{
    if (node->left != NULL)
    {
        return sbintree_rightmost(node->left);
    }

    sbintree *current = node;
    sbintree *predecessor = node->parent;
    while (predecessor != NULL && current == predecessor->left)
    {
        current = predecessor;
        predecessor = predecessor->parent;
    }
    return predecessor;
}

static void sbintree__replace_nodes(sbintree ** const root, sbintree * const old_node, sbintree * const new_node)
{
    if (old_node->parent == NULL)
    {
        *root = new_node;
    }
    else if (old_node == old_node->parent->left)
    {
        old_node->parent->left = new_node;
    }
    else
    {
        old_node->parent->right = new_node;
    }

    if (new_node != NULL)
    {
        new_node->parent = old_node->parent;
    }
}

void sbintree_remove(sbintree ** const root, sbintree * const node, void (*free_func)(sbintree *))
{
    if (node->left == NULL)
    {
        sbintree__replace_nodes(root, node, node->right);
        free_func(node);
        return;
    }
    else if (node->right == NULL)
    {
        sbintree__replace_nodes(root, node, node->left);
        free_func(node);
        return;
    }

    sbintree *replacement = sbintree_successor(node);
    if (replacement->parent != node)
    {
        sbintree__replace_nodes(root, replacement, replacement->right);
        replacement->right = node->right;
        replacement->right->parent = replacement;
    }

    sbintree__replace_nodes(root, node, replacement);
    replacement->left = node->left;
    replacement->left->parent = replacement;

    free_func(node);
}

#endif /*SBINTREE_IMPLEMENTATION*/
