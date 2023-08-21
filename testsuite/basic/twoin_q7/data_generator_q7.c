#include "stdio.h"

void main(void)
{
    int i;
    int bsize = 5;
    int numdata = 10000;
    unsigned char vala, valb;
    FILE *fptr;
    FILE *f2ptr;

    int count = 1;

    fptr = fopen("input_u8_a.bin", "w");
    f2ptr = fopen("input_u8_b.bin", "w");

    for (i = 0; i < numdata * bsize; i++)
    {
        vala = rand() % 256;
        valb = rand() % 256;

        printf("%d %d %d\n", count, vala, valb);
        fwrite(&vala, sizeof(unsigned char), 1, fptr);
        fwrite(&valb, sizeof(unsigned char), 1, f2ptr);
        count++;
    }
    fclose(fptr);
    fclose(f2ptr);
}
