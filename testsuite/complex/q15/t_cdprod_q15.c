#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_complex_math.h"

double dec2float(q15_t in);
int main(void)
{
    uint32_t numsample = 29999;
    short inputAvec[2*numsample] __attribute__ ((aligned(4)));
    short inputBvec[2*numsample] __attribute__ ((aligned(4)));
    short outputvec[2*numsample] __attribute__ ((aligned(4)));
    short goldenvec[2*numsample];

    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_cmc_q15_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_cmc_q15_b.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_cmplx_dot_q15.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&inputAvec, sizeof(q15_t), 2*numsample, fptr);
    test_Fread(&inputBvec, sizeof(q15_t), 2*numsample, f2ptr);
    test_Fread(&goldenvec, sizeof(q15_t), 2*numsample, gptr);

    riscv_dsp_cdprod_q15(inputAvec, inputBvec, numsample, outputvec);
    dump2_q15(goldenvec, outputvec, 2*numsample);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[2 * numsample], output_f32[2 * numsample];
    riscv_dsp_convert_q15_f32(goldenvec, golden_f32, 2 * numsample);
    riscv_dsp_convert_q15_f32(outputvec, output_f32, 2 * numsample);

    NRMSD = dump_rmsd(golden_f32, output_f32, 2 * numsample);

    if (NRMSD <= 0.0003)
        return 0;
    else
        return -1;
}