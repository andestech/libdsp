#include "stdio.h"

void main(void)
{
    int i;
    int bsize = 50;
    int numdata = 50;
    short val;
    short valb;
    FILE *fptr;
    FILE *f2ptr;

    int count = 1;

    fptr = fopen("input_q15_fast_a.bin", "w");
    f2ptr = fopen("input_q15_fast_b.bin", "w");

    for (i = 0; i < numdata * bsize; i++)
    {
        val = rand() % (65536);
        val = val / 4;
        valb = rand() % (65536);
        valb = valb / 4;
        printf("%d %d %d\n", count, val, valb);
        fwrite(&val, sizeof(short), 1, fptr);
        fwrite(&valb, sizeof(short), 1, f2ptr);
        count++;
    }
    fclose(fptr);
    fclose(f2ptr);
}
