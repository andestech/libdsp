#include "stdio.h"

void main(void)
{
    int i;
    int bsize = 16;
    int numdata = 1;
    int vala = 0, valb;
    FILE *fptr;
    FILE *f2ptr;

    int count = 1;

    fptr = fopen("input_q31.bin", "w");
    f2ptr = fopen("input_ref_q31.bin", "w");

    for (i = 0; i < numdata * bsize; i++)
    {
        vala = rand() % (1073741824);
        valb = rand() % (1073741824);
        printf("%d %d %d\n", count, vala, valb);
        fwrite(&vala, sizeof(int), 1, fptr);
        fwrite(&valb, sizeof(int), 1, f2ptr);
        count++;
    }
    fclose(fptr);
    fclose(f2ptr);
}
