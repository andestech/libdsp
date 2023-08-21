#include "stdio.h"

void main(void)
{
    int i;
    int totalsize = 32000;
    float val;
    float val2;
    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;

    int count = 1;

    gptr = fopen("far_end_signal.txt", "r");

    fptr = fopen("input_f32.bin", "w");
    f2ptr = fopen("input_ref_f32.bin", "w");

    float maxval = 0.0f;
    float minval = 0.0f;
    for (i = 0; i < totalsize; i++)
    {
#if 1
        short vd;
        float vf;
        fscanf(gptr, "%hd ", &vd);
        vf = (float)vd / 32768;
        printf("%d %.6f\n", vd, vf);
        val = vf;
        val2 = vf;
#else
        short vd;
        float vf;
        fscanf(gptr, "%hd ", &vd);
        vf = (float)vd / 32768;
        printf("%d %.6f\n", vd, vf);
        val = vf / 32;
        val2 = vf / 32;
        //val = rand() % 31250 - 15625;
        //val2 = rand() % 31250 - 15625;
        //val = val / 1000000;
        //val2 = val2 / 1000000;
#endif
        //printf("%d %.6f %.6f\n", count, val, val2);
        fwrite(&val, sizeof(float), 1, fptr);
        fwrite(&val2, sizeof(float), 1, f2ptr);
        maxval = (maxval < val) ? val : maxval;
        maxval = (maxval < val2) ? val2 : maxval;
        minval = (minval > val) ? val : minval;
        minval = (minval > val2) ? val2 : minval;
        count++;
    }
    printf("maxval is %.6f\n", maxval);
    printf("minval is %.6f\n", minval);
    fclose(fptr);
    fclose(f2ptr);
}
