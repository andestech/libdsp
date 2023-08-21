#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_utils_math.h"

#define TEST_SIZE 8192

int main(void)
{
    uint32_t totalsize = TEST_SIZE;
    q15_t output_q15[totalsize];
    float32_t output[totalsize], golden[totalsize];
    float32_t NRMSD;

    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;
    char path_buffer[512];
    sprintf (path_buffer, "%s/golden_log_q15_in_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&golden, sizeof(float32_t), totalsize, gptr);
    q31_t count = 1;
    for (uint32_t i = 0; i < totalsize; i++)
    {
        q15_t out_q15 = riscv_dsp_log_q15(count);  // 4.11
        output_q15[i] = out_q15;
        count = count + 8;
    }
    riscv_dsp_convert_q15_f32(output_q15, output, totalsize);

    dump2_f32(golden, output, totalsize);

    fclose(gptr);

    NRMSD = dump_rmsd(golden, output, totalsize);

    if (NRMSD <= 0.00005)
        return 0;
    else
        return -1;
}
