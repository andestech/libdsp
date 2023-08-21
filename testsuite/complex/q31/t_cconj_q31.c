#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_complex_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t sample = 16384;
    q31_t inputA[2 * sample];
    q31_t output[2 * sample], golden[2 * sample];
    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_conj_q31.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_conj_q31.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&inputA, sizeof(q31_t), 2 * sample, fptr);
    dump_q31("input complex array(even: re, odd: im)", inputA, 2*sample);
    test_Fread(&golden, sizeof(q31_t), 2 * sample, gptr);

    riscv_dsp_cconj_q31(inputA, output, sample);
    dump2_q31(golden, output, 2*sample);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[2 * sample], output_f32[2 * sample];
    riscv_dsp_convert_q31_f32(golden, golden_f32, 2 * sample);
    riscv_dsp_convert_q31_f32(output, output_f32, 2 * sample);

    NRMSD = dump_rmsd(golden_f32, output_f32, 2 * sample);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
