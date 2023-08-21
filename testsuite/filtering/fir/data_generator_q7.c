#include "stdio.h"

void main(void)
{
    int i;
    int bsize = 4;
    int numdata = 256;
    signed char val;
    FILE *fptr;
    FILE *f2ptr;

    int count = 1;

    fptr = fopen("input_q7.bin", "w");

    for (i = 0; i < numdata * bsize; i++)
    {
        val = rand() % 256;
        printf("%d %d\n", count, val);
        fwrite(&val, sizeof(signed char), 1, fptr);
        count++;
    }
    fclose(fptr);
}
