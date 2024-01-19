#include <stdio.h>
#include <string.h>

#define SHASHTAB_IMPLEMENTATION
#define SBINTREE_IMPLEMENTATION
#define SDLL_IMPLEMENTATION
#include "../shashtab.h"

int key_func(void *key, void *node_key)
{
    return strcmp(key, node_key);
}

void sbintree_free_func(sbintree * const node)
{
    free(node);
}

int main()
{
    shashtab tab = shashtab_new(100);

    if (tab.capacity <= 0)
    {
        return 1;
    }

    char *str = "Hello world";

    if (!shashtab_set(tab, "foo", 4, key_func, str))
    {
        return 1;
    }

    char *res = shashtab_get(tab, "foo", 4, key_func);
    printf("foo: %s\n", res);

    if (!shashtab_get(tab, "bar", 4, key_func))
    {
        printf("key 'bar' not found\n");
    }

    shashtab_del(tab, "foo", 4, key_func, sbintree_free_func);
    if (!shashtab_get(tab, "foo", 4, key_func))
    {
        printf("key 'foo' not found\n");
    }

    /* cleanup */
    shashtab_free(tab, sbintree_free_func);

    return 0;
}
