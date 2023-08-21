#include "stdio.h"

void main(void)
{
    int i;
    int loop = 100;
    int alen = 5, blen = 4;
    int numdata = 20;
    float val;
    FILE *fptr;
    FILE *gptr;

    int count = 1;

    fptr = fopen("input_f32_a-1.bin", "w");
    gptr = fopen("input_f32_b-1.bin", "w");

    for (i = 0; i < loop * alen; i++)
    {
        val = rand() % 100;
        val = val / 100;
        printf("a :%d %f\n", count, val);
        fwrite(&val, sizeof(float), 1, fptr);
        count++;
    }
    for (i = 0; i < loop * blen; i++)
    {
        val = rand() % 100;
        val = val / 100;
        printf("b:%d %f\n", count, val);
        fwrite(&val, sizeof(float), 1, gptr);
        count++;
    }

    fclose(fptr);
    fclose(gptr);
}
