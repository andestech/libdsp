#include "stdio.h"

void main(void)
{
    int i;
    int fftlen = 16384;
    int datasize = 2 * fftlen;
    float val;
    FILE *fptr;

    int count = 1;

    fptr = fopen("input_f32.bin", "w");

    for (i = 0; i < datasize; i++)
    {
        val = rand() % 100;
        val = val / 100;
        printf("%d %f\n", count, val);
        fwrite(&val, sizeof(float), 1, fptr);
        count++;
    }
    fclose(fptr);
}
