#include "stdio.h"


int main(void)
{
    int i;
    int bsize = 4;
    int numdata = 256;
    int val;
    FILE *fptr;

    int count = 1;

    fptr = fopen("input_q31.bin", "w");

    for (i = 0; i < numdata * bsize; i++)
    {
        val = mrand48();
        printf("%d %d\n", count, val);
        fwrite(&val, sizeof(int), 1, fptr);
        count++;
    }
    fclose(fptr);

    return 0;
}