#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_complex_math.h"
#include "riscv_dsp_test_accuracy.h"

double dec2float(q31_t in);
int main(void)
{
    uint32_t numsample = 7500;
    q31_t inputAvec[2 * numsample], inputBvec[2 * numsample], outputvec[2 * numsample], goldenvec[2 * numsample];

    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_clx_q31_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_clx_q31_b.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (f2ptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_cmplx_dot_q31.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&inputAvec, sizeof(q31_t), 2 * numsample, fptr);
    test_Fread(&inputBvec, sizeof(q31_t), 2 * numsample, f2ptr);
    test_Fread(&goldenvec, sizeof(q31_t), 2 * numsample, gptr);

    riscv_dsp_cdprod_q31(inputAvec, inputBvec, numsample, outputvec);
    q31_t max = dump2_q31(goldenvec, outputvec, 2 * numsample);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);

    float32_t golden_f32[2 * numsample], output_f32[2 * numsample];
    riscv_dsp_convert_q31_f32(goldenvec, golden_f32, 2 * numsample);
    riscv_dsp_convert_q31_f32(outputvec, output_f32, 2 * numsample);

    dump_rmsd(golden_f32, output_f32, 2 * numsample);

    if (max <= 0x0000001d)
        return 0;
    else
        return -1;
}
