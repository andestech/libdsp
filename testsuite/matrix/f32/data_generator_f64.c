#include "stdio.h"

#define N 20

void main(void)
{
    int i;
    int datasize = 10000;
    double val;
    double val2;
    FILE *fptr;
    FILE *f2ptr;

    int count = 1;

    fptr = fopen("input_f64_a.bin", "w");
    f2ptr = fopen("input_f64_b.bin", "w");

    for (i = 0; i < datasize; i++)
    {
        val = rand() % (1 << N);
        val = val / (1 << N);
        val2 = rand() % (1 << N);
        val2 = val2 / (1 << N);
        printf("%d %5.20f\n", count, val, val2);
        fwrite(&val, sizeof(double), 1, fptr);
        fwrite(&val2, sizeof(double), 1, f2ptr);
        count++;
    }
    fclose(fptr);
}
