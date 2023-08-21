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
    u16_t out_max[loop], gold_max[loop];
    uint32_t out_index, gold_index, diff_index;

    FILE *fptr;
    FILE *gptr;
    FILE *g2ptr;
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

    sprintf (path_buffer, "%s/golden_max_u16_index.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input, sizeof(u16_t), blocksize, fptr);
        test_Fread(&gold_index, sizeof(uint32_t), 1, g2ptr);

        out_max[i] = riscv_dsp_max_u16(input, 0, &out_index);
        out_max[i] = riscv_dsp_max_u16(input, blocksize, &out_index);
        diff_index = (gold_index - 1) - out_index;
        if (diff_index != 0)
        {
            printf("index error\n");
            return -1;
        }
    }
    test_Fread(&gold_max, sizeof(u16_t), loop, gptr);
    dump2_u16(gold_max, out_max, loop);

    fclose(fptr);
    fclose(gptr);
    fclose(g2ptr);

    float32_t NRMSD;
    float32_t golden_f32[loop], output_f32[loop];
    _dsp_convert_u16_f32(gold_max, golden_f32, loop);
    _dsp_convert_u16_f32(out_max, output_f32, loop);

    NRMSD = dump_rmsd(golden_f32, output_f32, loop);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
