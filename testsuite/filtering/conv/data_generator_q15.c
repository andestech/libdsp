#include "stdio.h"

void main(void)
{
    int i;
    int numdata = 1024 * 1000;
    int numdata2 = 16 * 1000;
    short val;
    FILE *fptr;
    FILE *f2ptr;

    int count = 1;

    fptr = fopen("input_q15_a.bin", "w");

    for (i = 0; i < numdata; i++)
    {
        val = rand() % (65536);
        printf("%d %d\n", count, val);
        fwrite(&val, sizeof(short), 1, fptr);
        count++;
    }
    fclose(fptr);
    fptr = fopen("input_q15_b.bin", "w");

    for (i = 0; i < numdata2; i++)
    {
        val = rand() % (65536);
        printf("%d %d\n", count, val);
        fwrite(&val, sizeof(short), 1, fptr);
        count++;
    }
    fclose(fptr);
}
