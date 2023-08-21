#include "stdio.h"
#include "stdlib.h"

void main(void)
{
    int i;
    int numdata = 16384;
    short vala , valb, valc;
    FILE *fptr;
    FILE *f2ptr;

    int count = 1;

    fptr = fopen("input_real_q15_a.bin", "w");
    f2ptr = fopen("input_real_q15_b.bin", "w");

    for (i = 0; i < numdata; i++)
    {
        vala = rand() % (65536);
        valb = rand() % (65536);
        valc = rand() % (65536);
        printf("%d %x %x %x\n", count, vala, valb, valc);
        fwrite(&vala, sizeof(short), 1, fptr);
        fwrite(&valb, sizeof(short), 1, fptr);
        fwrite(&valc, sizeof(short), 1, f2ptr);
        count++;
    }
    fclose(fptr);
    fclose(f2ptr);
}
