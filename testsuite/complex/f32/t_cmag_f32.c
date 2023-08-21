#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_complex_math.h"

int main(void)
{
    uint32_t numSample = 600;
    float32_t inputvec[2*numSample];
    float32_t goldenvec[numSample], outputvec[numSample];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_mag_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mag_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&inputvec, sizeof(float32_t), 2*numSample, fptr);
    dump_f32("input complex array(even: re, odd: im)", inputvec, 2*numSample);
    test_Fread(&goldenvec, sizeof(float32_t), numSample, gptr);

    riscv_dsp_cmag_f32(inputvec, outputvec, numSample);
    dump2_f32(goldenvec, outputvec, numSample);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(goldenvec, outputvec, numSample);

    if (NRMSD <= 0.0000001)
        return 0;
    else
        return -1;
}
