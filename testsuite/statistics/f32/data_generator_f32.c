#include "stdio.h"

void main(void)
{
    int i;
    int datasize = 10000;
    float val;
    FILE *fptr;

    int count = 1;

    fptr = fopen("input_f32.bin", "w");

    for (i = 0; i < datasize; i++)
    {
        val = rand() % 1000000;
        val = val / 1000000;
        printf("%d %f\n", count, val);
        fwrite(&val, sizeof(float), 1, fptr);
        count++;
    }
    fclose(fptr);
}
