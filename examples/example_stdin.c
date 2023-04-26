#include <stdio.h>

#define SSTR_IMPLEMENTATION
#include "../sstr.h"

int main()
{
    sstr s = sstr_new("");
    int c;
    while ((c = getchar()) != EOF)
    {
        sstr_add_char(&s, c);
    }
    printf("%ld, %ld, %s\n", s.length, s.capacity, s.cstr);

    return 0;
}
