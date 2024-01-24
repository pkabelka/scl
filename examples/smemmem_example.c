#include <stdio.h>

#define SMEMMEM_IMPLEMENTATION
#include "../smemmem.h"

int main()
{
    char *haystack = "foobarbarfoobar";
    void *found = smemmem_naive(haystack, strlen(haystack), "bar", 3);
    if (found != NULL)
    {
        printf("%zu\n", (size_t) ((char *) found - haystack));
        /* 3 */
    }

    found = smemmem_bmh(haystack, strlen(haystack), "bar", 3, NULL);
    if (found != NULL)
    {
        printf("%zu\n", (size_t) ((char *) found - haystack));
        /* 3 */
    }

    found = smemmem_kmp(haystack, strlen(haystack), "bar", 3);
    if (found != NULL)
    {
        printf("%zu\n", (size_t) ((char *) found - haystack));
        /* 3 */
    }

    size_t *indices = NULL;
    size_t const n_found = smemmem_kmp_all(haystack, strlen(haystack), "bar", 3, &indices);
    for (size_t i = 0; i < n_found; i++)
    {
        printf("%zu ", indices[i]);
    }
    printf("\n");
    /* 3 6 12  */
    free(indices);

    return 0;
}
