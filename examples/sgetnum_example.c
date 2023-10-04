#define SGETNUM_IMPLEMENTATION
#include "../sgetnum.h"

#define SDARRAY_IMPLEMENTATION
#include "../sdarray.h"

static int get_char_stdin()
{
    return fgetc(stdin);
}

int main()
{
    sdarray numbers = sdarray_new(sizeof(long long int), 0);

    long long int number[1];

    bool cont;
    do
    {
        cont = sgetnum_ll(&get_char_stdin, NULL, 0, &number[0], 10);
        sdarray_add_from(&numbers, (void *) number, 1);
    } while (cont);

    for (size_t i = 0; i < numbers.length; i++)
    {
        printf("%lld ", ((long long int *) numbers.data)[i]);
    }
    printf("\n");

    free(numbers.data);
    return 0;
}
