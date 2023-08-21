#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_distance_math.h"

#define SIZE 100
#define order 2

int main(void)
{
    uint32_t i;
    int datasize = 10000;
    uint32_t blocksize = SIZE;
    uint32_t loop = datasize / blocksize;
    float32_t src1[blocksize];
    float32_t src2[blocksize];
    float32_t outputda[loop], golden[loop];

    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_f32_2.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (f2ptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_minkowski_dist_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&golden, sizeof(float32_t), loop, gptr);
    for (i = 0; i < loop; i++)
    {
        test_Fread(&src1, sizeof(float32_t), blocksize, fptr);
        test_Fread(&src2, sizeof(float32_t), blocksize, f2ptr);
        outputda[i] = riscv_dsp_dist_minkowski_f32(src1, src2, order, blocksize);
    }
    float32_t maxdiff = dump2_f32(golden, outputda, loop);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);

    dump_rmsd(golden, outputda, loop);

    if (maxdiff <= 0.000001)
        return 0;
    else
        return -1;
}
