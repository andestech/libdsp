#include "stdio.h"

void main(void)
{
    int i;
    int fftlen = 16384;
    int datasize = 2 * fftlen;
    short val;
    FILE *fptr;

    int count = 1;

    fptr = fopen("input_q15.bin", "w");

    for (i = 0; i < datasize; i++)
    {
        val = rand() % (65536);
        printf("%d %d\n", count, val);
        fwrite(&val, sizeof(short), 1, fptr);
        count++;
    }
    fclose(fptr);
}
