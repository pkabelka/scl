#define _GNU_SOURCE
#include <stdio.h>

#define SDLL_IMPLEMENTATION
#include "../sdll.h"

#define SSTR_IMPLEMENTATION
#include "../sstr.h"

void sdll_sstr_free(void *data)
{
    sstr_free((sstr *) data);
}

int main()
{
    sdll *list = sdll_new();

    sstr str1 = sstr_new("dolor sit");
    sdll_insert_first(list, &str1);
    printf("%lu\n", list->length);

    sstr str2 = sstr_new("amet consectetur");
    sdll_insert_last(list, &str2);
    printf("%lu\n", list->length);

    sstr str3 = sstr_new("Lorem ipsum");
    sdll_insert_first(list, &str3);
    printf("%lu\n", list->length);

    /* remove example with sdll_unlink */
    sdll_node *curr = list->first;
    while (curr != NULL)
    {
        sdll_node *next = curr->next;

        printf("%s\n", ((sstr *) curr->data)->cstr);

        /* find the node to remove */
        if (sstr_cmp_const(*(sstr *) curr->data, "amet consectetur") == 0)
        {
            printf("%lu\n", list->length);

            /* remove the node from list */
            sdll_unlink(list, curr);
            /* free the node data */
            sstr_free((sstr *) curr->data);
            /* set next node pointer */
            next = curr->next;
            /* free the unlinked node */
            free(curr);

            printf("%lu\n", list->length);
        }
        curr = next;
    }

    /* print the list */
    for (sdll_node *curr = list->first; curr != NULL; curr = curr->next)
    {
        printf("%s\n", ((sstr *) curr->data)->cstr);
    }

    /* remove example with sdll_remove */
    curr = list->first;
    while (curr != NULL)
    {
        sdll_node *next = curr->next;

        /* find the node to remove */
        if (sstr_cmp_const(*(sstr *) curr->data, "dolor sit") == 0)
        {
            printf("%lu\n", list->length);

            next = curr->next;
            sdll_remove(list, &curr, sdll_sstr_free);

            printf("%lu\n", list->length);
        }
        curr = next;
    }

    /* print the list */
    for (sdll_node *curr = list->first; curr != NULL; curr = curr->next)
    {
        printf("%s\n", ((sstr *) curr->data)->cstr);
    }

    /* free the entire list */
    sdll_free(&list, sdll_sstr_free);

    return 0;
}
