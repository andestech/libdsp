#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>


int main(void)
{
    int i;
    int size = 1000;
    long long val;
    FILE *fptr;
    FILE *gptr;

    fptr = fopen("input_q63.bin", "w");
    gptr = fopen("input_q63_u32.bin", "w");

    for (i = 0; i < size; i++)
    {
        int a = rand();
        int b = rand();
        int c = rand();
        int d = rand();
        long e = (long)a * b;
        e = abs(e);
        long f = (long)c * d;
        f = abs(f);
        long long answer = (long long)e * f * e;
        if ((llabs(answer) >> 31) < e)
        {
            printf("%d %lli %d\n", i, answer, e);
            fwrite(&answer, sizeof(long long), 1, fptr);
            fwrite(&e, sizeof(unsigned int), 1, gptr);
        }
        else i--;
    }
    fclose(fptr);
    fclose(gptr);

    return 0;
}
