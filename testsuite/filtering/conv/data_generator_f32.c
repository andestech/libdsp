#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void main(void)
{
    int i;
    int numdata = 1024 * 1000;
    int numdata2 = 16 * 1000;
    float val;
    FILE *fptr;

    int count = 1;

    fptr = fopen("input_f32_a.bin", "w");

    for (i = 0; i < numdata; i++)
    {
        val = (double)rand()*2 / RAND_MAX + (-1) ;
        //val = rand() % 1000;
        //val = val / 10000;
        printf("%d %f\n", count, val);
        fwrite(&val, sizeof(float), 1, fptr);
        count++;
    }
    fclose(fptr);

    fptr = fopen("input_f32_b.bin", "w");

    for (i = 0; i < numdata2; i++)
    {
        val = (double)rand()*2 / RAND_MAX + (-1) ;
        //val = rand() % 1000;
        //val = val / 10000;
        printf("%d %f\n", count, val);
        fwrite(&val, sizeof(float), 1, fptr);
        count++;
    }
    fclose(fptr);
}
