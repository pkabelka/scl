#include <stdio.h>
#include <string.h>

#define SBINTREE_IMPLEMENTATION
#define SDLL_IMPLEMENTATION
#include "../sbintree.h"

int str_comp(void *s1, void *s2)
{
    return strcmp((const char*)s1, (const char*)s2);
}

void printf_callback(sbintree * const node)
{
    printf("%s: %s\n", (char*)node->key, (char*)node->data);
}

void sbintree_free_func(sbintree * const node)
{
    free(node);
}

int main()
{
    sbintree *root = NULL;

    sbintree_insert(&root, "a", str_comp, "a");
    sbintree_insert(&root, "c", str_comp, "c");
    sbintree_insert(&root, "b", str_comp, "b");
    sbintree_insert(&root, "e", str_comp, "e");
    sbintree_insert(&root, "d", str_comp, "d");

    printf("%s\n", (char*)(root->right->data));

    sbintree *node = sbintree_search(root, "d", str_comp);
    printf("%s\n", (char*)(node->data));

    sbintree_inorder(root, printf_callback);

    sbintree *leftmost = sbintree_leftmost(root);
    printf("%s\n", (char*)(leftmost->data));

    sbintree *rightmost = sbintree_rightmost(root);
    printf("%s\n", (char*)(rightmost->data));

    sbintree_free(&root, sbintree_free_func);

    return 0;
}
