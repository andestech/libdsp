#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_statistics_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t i;
    int datasize = 500;
    uint32_t blocksize = 10;
    uint32_t loop = datasize / blocksize;
    q7_t input[blocksize] __attribute__ ((aligned(4)));
    q7_t out_min[loop], gold_min[loop];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q7.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_min_q7_min.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input, sizeof(q7_t), blocksize, fptr);

        out_min[i] = riscv_dsp_min_val_q7(input, 0);
        out_min[i] = riscv_dsp_min_val_q7(input, blocksize);
    }
    test_Fread(&gold_min, sizeof(q7_t), loop, gptr);
    dump2_q7(gold_min, out_min, loop);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[loop], output_f32[loop];
    riscv_dsp_convert_q7_f32(gold_min, golden_f32, loop);
    riscv_dsp_convert_q7_f32(out_min, output_f32, loop);

    NRMSD = dump_rmsd(golden_f32, output_f32, loop);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
