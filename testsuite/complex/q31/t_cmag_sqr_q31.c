#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_complex_math.h"
#include "riscv_dsp_test_accuracy.h"

double dec2float(q31_t in);
int main(void)
{
    uint32_t numsample = 8192;
    q31_t inputAvec[2 * numsample], outputvec[numsample], goldenvec[numsample];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_mag_q31.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mag_squared_q31.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&inputAvec, sizeof(q31_t), 2 * numsample, fptr);
    dump_q31("input complex array(even: re, odd: im)", inputAvec, 2*numsample);
    test_Fread(&goldenvec, sizeof(q31_t), numsample, gptr);

    riscv_dsp_cmag_sqr_q31(inputAvec, outputvec, numsample);
    dump2_q31(goldenvec, outputvec, numsample);

    fclose(fptr);
    fclose(gptr);
    float32_t NRMSD;
    float32_t golden_f32[numsample], output_f32[numsample];
    riscv_dsp_convert_q31_f32(goldenvec, golden_f32, numsample);
    riscv_dsp_convert_q31_f32(outputvec, output_f32, numsample);

    NRMSD = dump_rmsd(golden_f32, output_f32, numsample);

    if (NRMSD <= 0.00000001)
        return 0;
    else
        return -1;
}
