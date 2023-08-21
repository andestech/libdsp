#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_basic_math.h"

int main(void)
{
    int32_t blocksize = 8;
    uint32_t i;
    uint32_t loop = 800 / blocksize;
    float32_t output[loop], golden[loop];
    float32_t inputAvec[blocksize], inputBvec[blocksize];

    FILE *fptr;
    FILE *gptr;
    FILE *f2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_dot_f32_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_dot_f32_b.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_dprod_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&inputAvec, sizeof(float32_t), blocksize, fptr);
        test_Fread(&inputBvec, sizeof(float32_t), blocksize, f2ptr);
        output[i] = riscv_dsp_dprod_f32(inputAvec, inputBvec, blocksize);
    }
    test_Fread(&golden, sizeof(float32_t), loop, gptr);
    dump2_f32(golden, output, loop);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden, output, loop);

    if (NRMSD <= 0.00000003)
        return 0;
    else
        return -1;
}
