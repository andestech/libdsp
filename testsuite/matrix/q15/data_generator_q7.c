#include "stdio.h"
#include "stdlib.h"

void main(void)
{
    int i;
    int bsize = 5;
    int numdata = 10000;
    char vala, valb;
    FILE *fptr;
    FILE *f2ptr;

    int count = 1;

    fptr = fopen("input_q7_little_a.bin", "w");
    f2ptr = fopen("input_q7_little_b.bin", "w");

    for (i = 0; i < numdata * bsize; i++)
    {
        vala = rand() % 256;
        valb = rand() % 256;
        vala = vala - 128;
        valb = valb - 128;
        vala = vala >> 2;
        valb = valb >> 2;

        printf("%d %d %d\n", count, vala, valb);
        fwrite(&vala, sizeof(char), 1, fptr);
        fwrite(&valb, sizeof(char), 1, f2ptr);
        count++;
    }
    fclose(fptr);
    fclose(f2ptr);
}
