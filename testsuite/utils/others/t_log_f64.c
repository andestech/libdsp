#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_utils_math.h"

#define TEST_SIZE 7

int main(void)
{
    uint32_t totalsize = TEST_SIZE;
    float64_t input[totalsize];
    float64_t output[totalsize], golden[totalsize];
    float64_t NRMSD;
    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f64_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;
    sprintf (path_buffer, "%s/golden_log_f64.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&input, sizeof(float64_t), totalsize, fptr);
    test_Fread(&golden, sizeof(float64_t), totalsize, gptr);

    for (uint32_t i = 0; i < totalsize; i++)
    {
        output[i] = riscv_dsp_log_f64(input[i]);
    }

    dump2_f64(golden, output, totalsize);

    fclose(fptr);
    fclose(gptr);

    NRMSD = dump_rmsd64(golden, output, totalsize);

    if (NRMSD <= 0.0000001)
        return 0;
    else
        return -1;
}
