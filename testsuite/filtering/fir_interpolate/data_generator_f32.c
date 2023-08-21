#include "stdio.h"

void main(void)
{
    int i;
    int bsize = 4;
    int numdata = 256;
    float val = 0.1;
    FILE *fptr;

    int count = 1;

    fptr = fopen("input_f32.bin", "w");

    for (i = 0; i < numdata * bsize; i++)
    {
        //val=rand()%100;
        //val=val/100;
        printf("%d %f\n", count, val);
        fwrite(&val, sizeof(float), 1, fptr);
        val += 0.1;
        count++;
    }
    fclose(fptr);
}
