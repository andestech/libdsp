#include "stdio.h"

void main(void)
{
    int i;
    int bsize = 16;
    int numdata = 1;
    float vala = 0, valb;
    FILE *fptr;
    FILE *f2ptr;

    int count = 1;

    fptr = fopen("input_f32.bin", "w");
    f2ptr = fopen("input_ref_f32.bin", "w");

    for (i = 0; i < numdata * bsize; i++)
    {
        vala = rand() % 100;
        valb = rand() % 100;
        vala = vala / 100;
        valb = valb / 100;
        printf("%d %f %f\n", count, vala, valb);
        fwrite(&vala, sizeof(float), 1, fptr);
        fwrite(&valb, sizeof(float), 1, f2ptr);
        count++;
    }
    fclose(fptr);
    fclose(f2ptr);
}
