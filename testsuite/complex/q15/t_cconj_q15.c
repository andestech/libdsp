#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_complex_math.h"

int main(void)
{
    uint32_t sample = 16383;
    q15_t inputA[2*sample] __attribute__ ((aligned(4)));
    q15_t output[2*sample] __attribute__ ((aligned(4)));
    q15_t golden[2*sample];
    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_conj_q15.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_conj_q15.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&inputA, sizeof(q15_t), 2*sample, fptr);
    dump_q15("input complex array(even: re, odd: im)", inputA, 2*sample);
    test_Fread(&golden, sizeof(q15_t), 2*sample, gptr);

    riscv_dsp_cconj_q15(inputA, output, sample);
    dump2_q15(golden, output, 2*sample);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[2 * sample], output_f32[2 * sample];
    riscv_dsp_convert_q15_f32(golden, golden_f32, 2 * sample);
    riscv_dsp_convert_q15_f32(output, output_f32, 2 * sample);

    NRMSD = dump_rmsd(golden_f32, output_f32, 2 * sample);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
