#include <stdio.h>

#define SDARRAY_IMPLEMENTATION
#include "../sdarray.h"

int main()
{
    sdarray arr = sdarray_new(sizeof(char), 0);
    printf("%lu, %lu, %s\n", arr.length, arr.capacity, (char *) arr.data);
    /* 0, 0, (null) */

    char str[] = {'f', 'o', 'o', '\0'};
    sdarray tmp_sdarray = sdarray_wrap_ptr(str, sizeof(char), 4);
    printf("%lu, %lu, %s\n", tmp_sdarray.length, tmp_sdarray.capacity, (char *) tmp_sdarray.data);
    /* 4, 4, foo */

    sdarray_add_sdarray(&arr, tmp_sdarray);
    printf("%lu, %lu, %s\n", arr.length, arr.capacity, (char *) arr.data);
    /* 4, 6, foo */

    sdarray arr2 = sdarray_new(sizeof(char), 0);
    sdarray_add_from(&arr2, str, 4);
    /* overwrite the string terminator when adding "bar" */
    arr2.length--;
    sdarray_add_from(&arr2, "bar", 4);
    printf("%lu, %lu, %s\n", arr2.length, arr2.capacity, (char *) arr2.data);
    /* 7, 11, foobar */

    sdarray arr3 = sdarray_clone(arr2);
    printf("%lu, %lu, %s\n", arr3.length, arr3.capacity, (char *) arr3.data);
    /* 7, 11, foobar */

    sdarray_remove(&arr3, 5);
    printf("%lu, %lu, %s\n", arr3.length, arr3.capacity, (char *) arr3.data);
    /* 6, 11, fooba */

    sdarray_remove(&arr3, 4);
    printf("%lu, %lu, %s\n", arr3.length, arr3.capacity, (char *) arr3.data);
    /* 5, 5, foob */

    arr3.length--;
    sdarray_add_from(&arr3, "az", 3);
    printf("%lu, %lu, %s\n", arr3.length, arr3.capacity, (char *) arr3.data);
    /* 7, 11, foobaz */

    printf("%c\n", *(char *) sdarray_at(&arr3, 3));
    /* b */

    free(arr.data);
    free(arr2.data);
    free(arr3.data);

    return 0;
}
