#include <stdio.h>
#include <stdlib.h>
#include <time.h>


static inline float gen_frandom(void)
{
    short a = rand() & 0xff;
    short signbit = a & 0x1;
    float out = ((float)a / 256);
    if (signbit == 0x1)
        return out;
    else
        return -out;
}

void main(void)
{
    int i;
    int numdata = 131072;
    float vala;
    float valb;
    FILE *fptr;
    FILE *fptr2;

    fptr = fopen("input_f32_2_rand_a.bin", "w");
    fptr2 = fopen("input_f32_2_rand_b.bin", "w");

    for (i = 0; i < numdata; i++)
    {
        do {
            vala =  gen_frandom();
        } while(vala == 0);
        do {
            valb =  gen_frandom();
        }while(valb == 0);
        printf("%d %f %f\n", i, vala, valb);
        fwrite(&vala, sizeof(float), 1, fptr);
        fwrite(&valb, sizeof(float), 1, fptr2);
    }
    fclose(fptr);
    fclose(fptr2);
}
