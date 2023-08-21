#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_complex_math.h"

int main(void)
{
    uint32_t numSample = 2500;
    float32_t inputA[2*numSample], inputB[2*numSample], outputvec[2*numSample], goldenvec[2*numSample];

    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_real_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_real_b.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mult_real_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&inputA, sizeof(float32_t), 2*numSample, fptr);
    test_Fread(&inputB, sizeof(float32_t), numSample, f2ptr);
    test_Fread(&goldenvec, sizeof(float32_t), 2*numSample, gptr);

    riscv_dsp_cmul_real_f32(inputA, inputB, outputvec, numSample);
    dump2_f32(goldenvec, outputvec, 2*numSample);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(goldenvec, outputvec, 2 * numSample);

    if (NRMSD <= 0.0000001)
        return 0;
    else
        return -1;
}
