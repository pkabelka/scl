#include <stdio.h>
#include <string.h>

#define STRIE_IMPLEMENTATION
#include "../strie.h"

void dummy_free(void *data){}

void print_keys(strie *node)
{
    for (size_t i = 0; i < 256; i++)
    {
        if (node->next[i] == NULL)
        {
            continue;
        }
        printf("%c: %p, ", (char)i, node->next[i]);
    }
    printf("\n");
}

int main()
{
    strie *root = NULL;

    strie_insert(&root, "foo", 3, "bar");
    strie_insert(&root, "faa", 3, "baz");
    strie_insert(&root, "fob", 3, "qux");

    printf("%s\n", (char*)strie_search(root, "foo", 3)->data);
    printf("%s\n", (char*)strie_search(root, "faa", 3)->data);

    print_keys(root);
    print_keys(root->next[(int)'f']);
    print_keys(root->next[(int)'f']->next[(int)'a']);
    print_keys(root->next[(int)'f']->next[(int)'o']);

    strie_remove(root, "faa", 3, dummy_free);

    print_keys(root);
    print_keys(root->next[(int)'f']);
    print_keys(root->next[(int)'f']->next[(int)'o']);

    strie_remove(root, "foo", 3, dummy_free);
    print_keys(root);
    print_keys(root->next[(int)'f']);
    print_keys(root->next[(int)'f']->next[(int)'o']);

    strie_remove(root, "fob", 3, dummy_free);

    free(root->next);
    free(root);
    root = NULL;

    return 0;
}
