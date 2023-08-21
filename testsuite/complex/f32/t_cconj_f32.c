#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_complex_math.h"

int main(void)
{
    uint32_t numSample = 30000;
    float32_t inputvec[2 * numSample], outputvec[2 * numSample], goldenvec[2 * numSample];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_cmplx_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_conj_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(inputvec, sizeof(float32_t), 2 * numSample, fptr);
    dump_f32("input complex array(even: re, odd: im)", inputvec, 2 * numSample);
    test_Fread(goldenvec, sizeof(float32_t), 2 * numSample, gptr);

    riscv_dsp_cconj_f32(inputvec, outputvec, numSample);
    dump2_f32(goldenvec, outputvec, 2 * numSample);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(goldenvec, outputvec, 2 * numSample);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
