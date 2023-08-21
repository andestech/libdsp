#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_statistics_math.h"

int main(void)
{
    uint32_t i;
    int datasize = 10000;
    uint32_t blocksize = 8;
    uint32_t loop = datasize / blocksize;
    float32_t input[blocksize];
    float32_t out_min[loop], gold_min[loop];
    uint32_t out_index, gold_index, diff_index;

    FILE *fptr;
    FILE *gptr;
    FILE *g2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_abs_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_absmin_f32_absmin.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_absmin_f32_index.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input, sizeof(float32_t), blocksize, fptr);
        test_Fread(&gold_index, sizeof(uint32_t), 1, g2ptr);
        out_min[i] = riscv_dsp_absmin_f32(input, 0, &out_index);
        out_min[i] = riscv_dsp_absmin_f32(input, blocksize, &out_index);
        diff_index = (gold_index - 1) - out_index;
        if (diff_index != 0) 
        {
            printf("index error\n");
            return -1;
        }
    }
    test_Fread(&gold_min, sizeof(float32_t), loop, gptr);
    dump2_f32(gold_min, out_min, loop);

    fclose(fptr);
    fclose(gptr);
    fclose(g2ptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(gold_min, out_min, loop);

    if (NRMSD <= 0.00000003)
        return 0;
    else
        return -1;
}
