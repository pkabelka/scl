#include <stdio.h>

#define SSTR_IMPLEMENTATION
#include "../sstr.h"

int main()
{
    sstr s = sstr_new("Lorem ipsum");
    printf("%lu, %lu, %s\n", s.length, s.capacity, s.cstr);
    /* 11, 32, Lorem ipsum */

    sstr tmp_sstr = sstr_new(" dolor sit");
    sstr_add_sstr(&s, tmp_sstr);
    printf("%lu, %lu, %s\n", s.length, s.capacity, s.cstr);
    /* 21, 32, Lorem ipsum dolor sit */

    sstr_add_const(&s, " amet con");
    printf("%lu, %lu, %s\n", s.length, s.capacity, s.cstr);
    /* 30, 32, Lorem ipsum dolor sit amet con */

    sstr_add_char(&s, 's');
    printf("%lu, %lu, %s\n", s.length, s.capacity, s.cstr);
    /* 31, 32, Lorem ipsum dolor sit amet cons */

    sstr_add_char(&s, 'e');
    printf("%lu, %lu, %s\n", s.length, s.capacity, s.cstr);
    /* 32, 65, Lorem ipsum dolor sit amet conse */

    sstr s2 = sstr_clone(s);
    printf("%lu, %lu, %s\n", s2.length, s2.capacity, s2.cstr);
    /* 32, 65, Lorem ipsum dolor sit amet conse */

    sstr_add_const(&s2, "ctetur");
    printf("%lu, %lu, %s\n", s2.length, s2.capacity, s2.cstr);
    /* 38, 65, Lorem ipsum dolor sit amet consectetur */

    sstr_swap(&s, &s2);
    printf("%lu, %lu, %s\n", s.length, s.capacity, s.cstr);
    /* 38, 65, Lorem ipsum dolor sit amet consectetur */
    printf("%lu, %lu, %s\n", s2.length, s2.capacity, s2.cstr);
    /* 32, 65, Lorem ipsum dolor sit amet conse */

    sstr_set_capacity(&s2, 33);
    printf("%lu, %lu, %s\n", s2.length, s2.capacity, s2.cstr);
    /* 32, 33, Lorem ipsum dolor sit amet conse */

    sstr_add_from(&s2, s.cstr, 5);
    printf("%lu, %lu, %s\n", s2.length, s2.capacity, s2.cstr);
    /* 37, 70, Lorem ipsum dolor sit amet conseLorem */

    sstr s2_substr = sstr_substr(&s2, 6, 5);
    printf("%lu, %lu, %s\n", s2_substr.length, s2_substr.capacity, s2_substr.cstr);
    /* 5, 32, ipsum */

    /* cleanup */
    sstr_free(&s);
    sstr_free(&tmp_sstr);
    sstr_free(&s2);
    sstr_free(&s2_substr);

    return 0;
}
