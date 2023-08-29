#include <stdio.h>

#define SSTR_IMPLEMENTATION
#include "../sstr.h"
#define SDARRAY_IMPLEMENTATION
#include "../sdarray.h"
#define SFFT_IMPLEMENTATION
#include "../sfft.h"

/* Input from command: seq 8 | sed '$!s/$/,/'
 * Output:
 * 1 2 3 4 5 6 7 8 
 * 36 0j    -4 9.65685j    -4 4j    -4 1.65685j    -4 0j    -4 -1.65685j    -4 -4j    -4 -9.65685j    
 */
int main()
{
    /* read all of STDIN into a string */
    sstr s = sstr_new("");
    char buffer[4096];
    while (fgets(buffer, 4096, stdin))
    {
        sstr_add(&s, buffer, strlen(buffer));
    }
    /* printf("%lu, %lu, %s\n", s.length, s.capacity, s.cstr); */


    /* convert numbers in string to long long int numbers */
    char digit_buffer[128];
    size_t digit_buffer_index = 0;
    sdarray numbers = sdarray_new(sizeof(long long int), 1);

    char const delimiter = ',';

    for (size_t i = 0; i <= s.length; i++)
    {
        if (digit_buffer_index < 128-1)
        {
            char const c = s.cstr[i];
            if (c == '\n' || c == '\r' || c == '\t' || c == ' ')
            {
                continue;
            }

            /* add non-delimiter chars to buffer */
            if (c != '\0' && c != delimiter)
            {
                digit_buffer[digit_buffer_index++] = c;
                continue;
            }
        }

        digit_buffer[digit_buffer_index] = '\0';
        digit_buffer_index = 0;

        long long int num_buffer[1] = { strtoll(digit_buffer, NULL, 10) };
        sdarray_add_from(&numbers, num_buffer, 1);
    }

    for (size_t i = 0; i < numbers.length; i++)
    {
        printf("%lld ", ((long long int *) numbers.data)[i]);
    }
    printf("\n");


    /* create arrays of "double" numbers for real and imaginary parts */
    sdarray real = sdarray_new(sizeof(double), numbers.length);
    sdarray imag = sdarray_new(sizeof(double), numbers.length);
    for (size_t i = 0; i < numbers.length; i++)
    {
        ((double *) real.data)[i] = (double) ((long long int *) numbers.data)[i];
        ((double *) imag.data)[i] = 0.0;
    }
    real.length = numbers.length;
    imag.length = numbers.length;


    /* run FFT of real numbers from STDIN */
    sfft fft;
    sfft_new(&fft, real.length);
    sfft_fft(&fft, real.data, imag.data);

    for (size_t i = 0; i < real.length; i++)
    {
        printf("%g %gj    ", ((double *) real.data)[i], ((double *) imag.data)[i]);
    }
    printf("\n");


    sstr_free(&s);
    free(numbers.data);
    free(real.data);
    free(imag.data);
    sfft_free(&fft);

    return 0;
}
