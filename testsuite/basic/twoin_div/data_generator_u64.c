#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>


int main(void)
{
    int i;
    int size = 1000;
    FILE *fptr;
    FILE *gptr;

    fptr = fopen("input_u64.bin", "w");
    gptr = fopen("input_u64_u32.bin", "w");

    for (i = 0; i < size; i++)
    {
        int a = rand();
        int b = rand();
        int c = rand();
        int d = rand();
        uint32_t e = (uint32_t)a * b;
        uint32_t f = (uint32_t)c * d;
        uint64_t answer = ((uint64_t)e * f * e);
        if ((answer >> 31) < e)
        {
            printf("%d %llu %u\n", i, answer, e);
            fwrite(&answer, sizeof(uint64_t), 1, fptr);
            fwrite(&e, sizeof(uint32_t), 1, gptr);
        }
        else i--;

    }
    fclose(fptr);
    fclose(gptr);

    return 0;
}
