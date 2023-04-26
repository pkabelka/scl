#include <stdio.h>

#define SSTR_IMPLEMENTATION
#include "../sstr.h"

int main()
{
    sstr s = sstr_new("Lorem ipsum");
    printf("%ld, %ld, %s\n", s.length, s.capacity, s.cstr);
    /* 11, 32, Lorem ipsum */

    sstr_add_sstr(&s, sstr_new(" dolor sit"));
    printf("%ld, %ld, %s\n", s.length, s.capacity, s.cstr);
    /* 21, 32, Lorem ipsum dolor sit */

    sstr_add_const(&s, " amet consec");
    printf("%ld, %ld, %s\n", s.length, s.capacity, s.cstr);
    /* 33, 34, Lorem ipsum dolor sit amet consec */

    sstr_add_char(&s, 't');
    printf("%ld, %ld, %s\n", s.length, s.capacity, s.cstr);
    /* 34, 67, Lorem ipsum dolor sit amet consect */

    sstr_add_char(&s, 'e');
    printf("%ld, %ld, %s\n", s.length, s.capacity, s.cstr);
    /* 35, 67, Lorem ipsum dolor sit amet consecte */

    sstr s2 = sstr_clone(s);
    printf("%ld, %ld, %s\n", s2.length, s2.capacity, s2.cstr);
    /* 35, 67, Lorem ipsum dolor sit amet consecte */

    sstr_add_const(&s2, "tur");
    printf("%ld, %ld, %s\n", s2.length, s2.capacity, s2.cstr);
    /* 38, 67, Lorem ipsum dolor sit amet consectetur */

    sstr_swap(&s, &s2);
    printf("%ld, %ld, %s\n", s.length, s.capacity, s.cstr);
    /* 38, 67, Lorem ipsum dolor sit amet consectetur */
    printf("%ld, %ld, %s\n", s2.length, s2.capacity, s2.cstr);
    /* 35, 67, Lorem ipsum dolor sit amet consecte */

    sstr_set_capacity(&s2, 36);
    printf("%ld, %ld, %s\n", s2.length, s2.capacity, s2.cstr);
    /* 35, 36, Lorem ipsum dolor sit amet consecte */

    return 0;
}
