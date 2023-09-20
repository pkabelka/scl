/**
 * This example show how to scan two arrays from STDIN. The numbers are
 * delimited by '\n' and the two arrays are delimited by "\n\n".
 *
 * The example program can be run like this:
 * { seq 0 4 && echo "" && seq 5 9 } | perl -pe 'chomp if eof' | ./sscannum_example_multiple
 *
 * The `perl` part is there to remove the trailing '\n' from `seq`.
 */

#include <stdio.h>
#define SSCANNUM_IMPLEMENTATION
#include "../sscannum.h"

static int get_char_stdin()
{
    int c = fgetc(stdin);
    if (c == '\n')
    {
        c = fgetc(stdin);
        if (c == '\n')
        {
            return EOF;
        }

        ungetc(c, stdin);
        return '\n';
    }
    return c;
}

int main()
{
    char const delimiter[] = {'\n'};

    long long int *numbers;
    size_t numbers_length;
    size_t numbers_capacity;

    sscannum_ll(&get_char_stdin, delimiter, 1, &numbers, &numbers_length, &numbers_capacity, 10);

    for (size_t i = 0; i < numbers_length; i++)
    {
        printf("%lld ", numbers[i]);
    }
    printf("\n");

    long long int *numbers2;
    size_t numbers2_length;
    size_t numbers2_capacity;

    sscannum_ll(&get_char_stdin, delimiter, 1, &numbers2, &numbers2_length, &numbers2_capacity, 10);

    for (size_t i = 0; i < numbers2_length; i++)
    {
        printf("%lld ", numbers2[i]);
    }
    printf("\n");

    free(numbers);
    free(numbers2);
    return 0;
}
