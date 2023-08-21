#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_statistics_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t i;
    uint32_t blocksize = 255;
    uint32_t loop = 100;
    q31_t input[blocksize];
    q31_t out_absmax[loop], gold_absmax[loop];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_with_corner_q31.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_absmax_q31_absmax.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input, sizeof(q31_t), blocksize, fptr);

        out_absmax[i] = riscv_dsp_absmax_val_q31(input, 0);
        out_absmax[i] = riscv_dsp_absmax_val_q31(input, blocksize);
    }
    test_Fread(&gold_absmax, sizeof(q31_t), loop, gptr);
    dump2_q31(gold_absmax, out_absmax, loop);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[loop], output_f32[loop];
    riscv_dsp_convert_q31_f32(gold_absmax, golden_f32, loop);
    riscv_dsp_convert_q31_f32(out_absmax, output_f32, loop);

    NRMSD = dump_rmsd(golden_f32, output_f32, loop);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
