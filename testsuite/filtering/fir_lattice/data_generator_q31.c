#include "stdio.h"

void main(void)
{
    int i;
    int bsize = 8;
    int numdata = 128;
    int val;
    FILE *fptr;

    int count = 1;

    fptr = fopen("input_q31.bin", "w");

    for (i = 0; i < numdata * bsize; i++)
    {
        val = rand() % (1073741824);
        printf("%d %d\n", count, val);
        fwrite(&val, sizeof(int), 1, fptr);
        count++;
    }
    fclose(fptr);
}
