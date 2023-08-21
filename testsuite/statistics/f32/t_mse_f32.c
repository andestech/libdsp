#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_statistics_math.h"

int main(void)
{
    uint32_t i;
    int datasize = 10000;
    uint32_t blocksize = 1000;
    uint32_t loop = datasize / blocksize;
    float32_t input1[blocksize], input2[blocksize];
    float32_t result;
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

    sprintf (path_buffer, "%s/golden_mse_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input1, sizeof(float32_t), blocksize, fptr);
        test_Fread(&input2, sizeof(float32_t), blocksize, f2ptr);
        riscv_dsp_mse_f32(input1, input2, blocksize, &result);
        outputda[i] = result;
    }
    test_Fread(&golden, sizeof(float32_t), loop, gptr);
    dump2_f32(golden, outputda, loop);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden, outputda, loop);

    if (NRMSD <= 0.000001)
        return 0;
    else
        return -1;
}
