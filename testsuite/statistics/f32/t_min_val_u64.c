#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_statistics_math.h"

int main(void)
{
    uint32_t i;
    int datasize = 10000;
    uint32_t blocksize = 16;
    uint32_t loop = datasize / blocksize;
    u64_t input[blocksize];
    u64_t out_min[loop], gold_min[loop];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f64_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_min_u64_min.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input, sizeof(u64_t), blocksize, fptr);

        out_min[i] = riscv_dsp_min_val_u64(input, 0);
        out_min[i] = riscv_dsp_min_val_u64(input, blocksize);
    }
    test_Fread(&gold_min, sizeof(u64_t), loop, gptr);
    dump2_u64(gold_min, out_min, loop);

    fclose(fptr);
    fclose(gptr);

    float64_t NRMSD;
    float64_t golden_f64[loop], output_f64[loop];
    _dsp_convert_u64_f64(gold_min, golden_f64, loop);
    _dsp_convert_u64_f64(out_min, output_f64, loop);

    NRMSD = dump_rmsd64(golden_f64, output_f64, loop);

    if (NRMSD <= 0.00000003)
        return 0;
    else
        return -1;
}
