#include "stdio.h"

#define N 20

void main(void)
{
    int i;
    int bsize = 4;
    int numdata = 256;
    double val;
    FILE *fptr;

    int count = 1;

    fptr = fopen("input_f64.bin", "w");

    for (i = 0; i < numdata * bsize; i++)
    {
        val = rand() % (1 << N);
        val = val / (1 << N);
        printf("%d %5.20f\n", count, val);
        fwrite(&val, sizeof(double), 1, fptr);
        count++;
    }
    fclose(fptr);
}
