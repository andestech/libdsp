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
    float64_t input[blocksize];
    float64_t out_min[loop], gold_min[loop];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f64_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_min_f64_min.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input, sizeof(float64_t), blocksize, fptr);

        out_min[i] = riscv_dsp_min_val_f64(input, 0);
        out_min[i] = riscv_dsp_min_val_f64(input, blocksize);
    }
    test_Fread(&gold_min, sizeof(float64_t), loop, gptr);
    dump2_f64(gold_min, out_min, loop);

    fclose(fptr);
    fclose(gptr);

    float64_t NRMSD;
    NRMSD = dump_rmsd64(gold_min, out_min, loop);

    if (NRMSD <= 0.00000003)
        return 0;
    else
        return -1;
}
