#include "stdio.h"
#include <stdlib.h>


int main(void)
{
    int i;
    int datasize = 30000;
    int valy, valx;
    FILE *fptr, *gptr;
    float zz;


    fptr = fopen("input_q31_y.bin", "w");
    gptr = fopen("input_q31_x.bin", "w");

    for (i = 0; i < (datasize); i++)
    {
        valy = rand() % (2147483648LL);
        valx = rand() % (2147483648LL);

        printf("%d %d %d\n", i, valy, valx);

        fwrite(&valy, sizeof(int), 1, fptr);
        fwrite(&valx, sizeof(int), 1, gptr);
    }

    fclose(fptr);
    fclose(gptr);

    return 0;
}
