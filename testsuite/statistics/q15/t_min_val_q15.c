#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_statistics_math.h"

int main(void)
{
    uint32_t i;
    uint32_t blocksize = 255;
    uint32_t loop = 100;
    q15_t input[blocksize] __attribute__ ((aligned(4)));
    q15_t out_min[loop], gold_min[loop];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q15.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_min_q15_min.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input, sizeof(q15_t), blocksize, fptr);

        out_min[i] = riscv_dsp_min_val_q15(input, 0);
        out_min[i] = riscv_dsp_min_val_q15(input, blocksize);
    }
    test_Fread(&gold_min, sizeof(q15_t), loop, gptr);
    dump2_q15(gold_min, out_min, loop);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[loop], output_f32[loop];
    riscv_dsp_convert_q15_f32(gold_min, golden_f32, loop);
    riscv_dsp_convert_q15_f32(out_min, output_f32, loop);

    NRMSD = dump_rmsd(golden_f32, output_f32, loop);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
