#include "stdio.h"

void main(void)
{
    int i;
    int bsize = 4;
    int numdata = 1000;
    short vala = 0, valb;
    FILE *fptr;
    FILE *f2ptr;

    int count = 1;

    fptr = fopen("input_q15.bin", "w");
    f2ptr = fopen("input_ref_q15.bin", "w");

    for (i = 0; i < numdata * bsize; i++)
    {
        vala = rand() % (65536 / 2);
        valb = rand() % (65536 / 2);
        printf("%d %d %d\n", count, vala, valb);
        fwrite(&vala, sizeof(short), 1, fptr);
        fwrite(&valb, sizeof(short), 1, f2ptr);
        count++;
    }
    fclose(fptr);
    fclose(f2ptr);
}
