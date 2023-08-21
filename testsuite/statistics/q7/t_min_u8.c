#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_statistics_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t i;
    int datasize = 1100;
    uint32_t blocksize = 11;
    uint32_t loop = datasize / blocksize;
    uint8_t input[blocksize] __attribute__ ((aligned(4)));
    uint8_t out_min[loop], gold_min[loop];
    uint32_t out_index, gold_index, diff_index;

    FILE *fptr;
    FILE *gptr;
    FILE *g2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_u8_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_min_u8_min.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_min_u8_index.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input, sizeof(uint8_t), blocksize, fptr);
        test_Fread(&gold_index, sizeof(uint32_t), 1, g2ptr);

        out_min[i] = riscv_dsp_min_u8(input, 0, &out_index);
        out_min[i] = riscv_dsp_min_u8(input, blocksize, &out_index);
        //dump2_u8(input, input, blocksize);
        //printf("%d %d\n", (gold_index - 1), out_index);
        diff_index = (gold_index - 1) - out_index;
        if (diff_index != 0)
        {
            printf("index error\n");
            return -1;
        }
    }
    test_Fread(&gold_min, sizeof(uint8_t), loop, gptr);
    dump2_u8(gold_min, out_min, loop);

    fclose(fptr);
    fclose(gptr);
    fclose(g2ptr);

    float32_t NRMSD;
    float32_t golden_f32[loop], output_f32[loop];
    _dsp_convert_u8_f32(gold_min, golden_f32, loop);
    _dsp_convert_u8_f32(out_min, output_f32, loop);

    NRMSD = dump_rmsd(golden_f32, output_f32, loop);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
