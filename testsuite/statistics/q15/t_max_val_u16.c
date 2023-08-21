#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_statistics_math.h"

int main(void)
{
    uint32_t i;
    uint32_t blocksize = 255;
    uint32_t loop = 100;
    u16_t input[blocksize] __attribute__ ((aligned(4)));
    u16_t out_max_val[loop], gold_max_val[loop];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q15.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_max_u16_max.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input, sizeof(u16_t), blocksize, fptr);

        out_max_val[i] = riscv_dsp_max_val_u16(input, 0);
        out_max_val[i] = riscv_dsp_max_val_u16(input, blocksize);
    }
    test_Fread(&gold_max_val, sizeof(u16_t), loop, gptr);
    dump2_u16(gold_max_val, out_max_val, loop);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[loop], output_f32[loop];
    _dsp_convert_u16_f32(gold_max_val, golden_f32, loop);
    _dsp_convert_u16_f32(out_max_val, output_f32, loop);

    NRMSD = dump_rmsd(golden_f32, output_f32, loop);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
