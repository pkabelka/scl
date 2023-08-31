#define SSCANNUM_IMPLEMENTATION
#include "../sscannum.h"

static int get_char_stdin()
{
    return fgetc(stdin);
}

int main()
{
    long long int *numbers;
    size_t numbers_length;
    size_t numbers_capacity;

    /* char const delimiter[] = {'\n'}; */
    /* sscannum_ll(&get_char_stdin, delimiter, 1, &numbers, &numbers_length, &numbers_capacity, 10); */
    sscannum_ll(&get_char_stdin, NULL, 0, &numbers, &numbers_length, &numbers_capacity, 10);
    /* sscannum_d(&get_char_stdin, NULL, 0, &numbers, &numbers_length, &numbers_capacity); */

    for (size_t i = 0; i < numbers_length; i++)
    {
        printf("%lld ", numbers[i]);
    }
    printf("\n");

    free(numbers);
    return 0;
}
