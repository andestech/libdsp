#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_utils_math.h"

#define TEST_SIZE 16383

int main(void)
{
    uint32_t totalsize = TEST_SIZE;
    float32_t input[totalsize];
    float32_t output[totalsize], golden[totalsize];
    float32_t NRMSD;
    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_exp_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;
    sprintf (path_buffer, "%s/golden_exp_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&input, sizeof(float32_t), totalsize, fptr);
    test_Fread(&golden, sizeof(float32_t), totalsize, gptr);

    for (uint32_t i = 0; i < totalsize; i++)
    {
        output[i] = riscv_dsp_exp_f32(input[i]);
    }

    dump2_f32(golden, output, totalsize);

    fclose(fptr);
    fclose(gptr);

    NRMSD = dump_rmsd(golden, output, totalsize);

    if (NRMSD <= 0.0000001)
        return 0;
    else
        return -1;
}
