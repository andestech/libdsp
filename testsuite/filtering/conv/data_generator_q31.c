#include "stdio.h"


int main(void)
{
    int i;
    int numdata = 1024 * 1000;
    int numdata2 = 16 * 1000;
    int val;
    FILE *fptr;

    int count = 1;

    fptr = fopen("input_q31_a.bin", "w");
    for (i = 0; i < numdata; i++)
    {
        val = mrand48();
        val = val >> 2;
        printf("%d %d\n", count, val);
        fwrite(&val, sizeof(int), 1, fptr);
        count++;
    }
    fclose(fptr);

    fptr = fopen("input_q31_b.bin", "w");
    for (i = 0; i < numdata2; i++)
    {
        val = mrand48();
        val = val >> 2;
        printf("%d %d\n", count, val);
        fwrite(&val, sizeof(int), 1, fptr);
        count++;
    }
    fclose(fptr);

    return 0;
    return 0;
}
