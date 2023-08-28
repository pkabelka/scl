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
    printf("%lu, %lu, %s\n", s.length, s.capacity, s.cstr);

    sstr_free(&s);

    return 0;
}

void buffered()
{
    sstr s = sstr_new("");
    char buffer[4096];
    while (fgets(buffer, 4096, stdin))
    {
        sstr_add_from(&s, buffer, strlen(buffer));
    }
    printf("%lu, %lu, %s\n", s.length, s.capacity, s.cstr);

    sstr_free(&s);
}
