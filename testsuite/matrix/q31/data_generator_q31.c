#include "stdio.h"

void main(void)
{
    int i;
    int bsize = 50;
    int numdata = 50;
    int vala;
    int valb;
    FILE *fptr;
    FILE *fptr2;

    int count = 1;

    fptr = fopen("input_q31_fast_a.bin", "w");
    fptr2 = fopen("input_q31_fast_b.bin", "w");

    for (i = 0; i < numdata * bsize; i++)
    {
        vala = rand() % (1073741824);
        vala = vala / 16;
        valb = rand() % (1073741824);
        valb = valb / 16;
        printf("%d %d %d\n", count, vala, valb);
        fwrite(&vala, sizeof(int), 1, fptr);
        fwrite(&valb, sizeof(int), 1, fptr2);
        count++;
    }
    fclose(fptr);
    fclose(fptr2);
}
