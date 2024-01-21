#include <stdio.h>

#define SSTR_IMPLEMENTATION
#include "../sstr.h"

int main()
{
    sstr s = sstr_new("Lorem ipsum");
    printf("%lu, %lu, %s\n", s.length, s.capacity, s.cstr);
    /* 11, 18, Lorem ipsum */

    sstr tmp_sstr = sstr_new(" dolor sit");
    sstr_add_sstr(&s, tmp_sstr);
    printf("%lu, %lu, %s\n", s.length, s.capacity, s.cstr);
    /* 21, 33, Lorem ipsum dolor sit */

    sstr_add_const(&s, " amet con");
    printf("%lu, %lu, %s\n", s.length, s.capacity, s.cstr);
    /* 30, 33, Lorem ipsum dolor sit amet con */

    sstr_add_char(&s, 's');
    printf("%lu, %lu, %s\n", s.length, s.capacity, s.cstr);
    /* 31, 33, Lorem ipsum dolor sit amet cons */

    sstr_add_char(&s, 'e');
    printf("%lu, %lu, %s\n", s.length, s.capacity, s.cstr);
    /* 32, 33, Lorem ipsum dolor sit amet conse */

    sstr s2 = sstr_clone(s);
    printf("%lu, %lu, %s\n", s2.length, s2.capacity, s2.cstr);
    /* 32, 33, Lorem ipsum dolor sit amet conse */

    sstr_add_const(&s2, "ctetur");
    printf("%lu, %lu, %s\n", s2.length, s2.capacity, s2.cstr);
    /* 38, 59, Lorem ipsum dolor sit amet consectetur */

    sstr_swap(&s, &s2);
    printf("%lu, %lu, %s\n", s.length, s.capacity, s.cstr);
    /* 38, 59, Lorem ipsum dolor sit amet consectetur */
    printf("%lu, %lu, %s\n", s2.length, s2.capacity, s2.cstr);
    /* 32, 33, Lorem ipsum dolor sit amet conse */

    sstr_set_capacity(&s2, 33);
    printf("%lu, %lu, %s\n", s2.length, s2.capacity, s2.cstr);
    /* 32, 33, Lorem ipsum dolor sit amet conse */

    sstr_add(&s2, s.cstr, 5);
    printf("%lu, %lu, %s\n", s2.length, s2.capacity, s2.cstr);
    /* 37, 57, Lorem ipsum dolor sit amet conseLorem */

    sstr s2_substr = sstr_substr(s2, 6, 5);
    printf("%lu, %lu, %s\n", s2_substr.length, s2_substr.capacity, s2_substr.cstr);
    /* 5, 9, ipsum */

    printf("%d\n", sstr_has_prefix(s2, "Lorem"));
    /* 1 */
    printf("%d\n", sstr_has_suffix(s2, "eLorem"));
    /* 1 */

    printf("%d\n", sstr_has_prefix_sstr(s2, s2));
    /* 1 */

    sstr s2_trimmed_left = sstr_trim_left(s2, "oLr");
    printf("%lu, %lu, %s\n", s2_trimmed_left.length, s2_trimmed_left.capacity, s2_trimmed_left.cstr);
    /* 34, 53, em ipsum dolor sit amet conseLorem */

    sstr s2_trimmed_right = sstr_trim_right(s2, "erm");
    printf("%lu, %lu, %s\n", s2_trimmed_right.length, s2_trimmed_right.capacity, s2_trimmed_right.cstr);
    /* 34, 53, Lorem ipsum dolor sit amet conseLo */

    size_t idx;
    if (sstr_index_of(s2, 'i', &idx))
        printf("%lu, %c\n", idx, 'i');
    /* 6, i */

    if (sstr_index_of_last(s2, 'o', &idx))
        printf("%lu, %c\n", idx, 'o');
    /* 33, o */

    sstr s2_repl = sstr_replace(s2, "Lorem", "foo");
    printf("%lu, %lu, %s\n", s2_repl.length, s2_repl.capacity, s2_repl.cstr);
    /* 33, 51, foo ipsum dolor sit amet consefoo */

    sstr s3 = sstr_new("foo bar");
    printf("%lu, %lu, %s\n", s3.length, s3.capacity, s3.cstr);
    /* 7, 12, foo bar */
    sstr s3_repl = sstr_replace(s3, "", "qux");
    printf("%lu, %lu, %s\n", s3_repl.length, s3_repl.capacity, s3_repl.cstr);
    /* 31, 48, quxfquxoquxoqux quxbquxaquxrqux */

    printf("%d\n", sstr_cmp(sstr_new("foo"), sstr_new("foo1")));
    /* -1 */

    /* cleanup */
    sstr_free(&s);
    sstr_free(&tmp_sstr);
    sstr_free(&s2);
    sstr_free(&s2_substr);
    sstr_free(&s2_trimmed_left);
    sstr_free(&s2_trimmed_right);
    sstr_free(&s2_repl);
    sstr_free(&s3);
    sstr_free(&s3_repl);

    return 0;
}
