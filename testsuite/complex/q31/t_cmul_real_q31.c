#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_complex_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t numsample = 16384;
    q31_t inputAvec[2 * numsample], inputBvec[2 * numsample], outputvec[2 * numsample], goldenvec[2 * numsample];

    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_mult_q31_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_mult_q31_b.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mult_real_q31.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&inputAvec, sizeof(q31_t), 2 * numsample, fptr);
    test_Fread(&inputBvec, sizeof(q31_t), numsample, f2ptr);
    test_Fread(&goldenvec, sizeof(q31_t), 2 * numsample, gptr);

    riscv_dsp_cmul_real_q31(inputAvec, inputBvec, outputvec, numsample);
    dump2_q31(goldenvec, outputvec, 2*numsample);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[2 * numsample], output_f32[2 * numsample];
    riscv_dsp_convert_q31_f32(goldenvec, golden_f32, 2 * numsample);
    riscv_dsp_convert_q31_f32(outputvec, output_f32, 2 * numsample);

    NRMSD = dump_rmsd(golden_f32, output_f32, 2 * numsample);

    if (NRMSD <= 0.00000001)
        return 0;
    else
        return -1;
}
