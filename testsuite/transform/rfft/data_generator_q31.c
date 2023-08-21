#include "stdio.h"

void main(void)
{
    int i;
    int fftlen = 1024;
    int datasize = 2 * fftlen;
    int val;
    FILE *fptr;

    int count = 1;

    fptr = fopen("input_q31.bin", "w");

    for (i = 0; i < datasize; i++)
    {
        val = mrand48();
        printf("%d %d\n", count, val);
        fwrite(&val, sizeof(int), 1, fptr);
        count++;
    }
    fclose(fptr);
}
