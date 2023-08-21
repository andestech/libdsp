#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_utils_math.h"

#define TEST_SIZE  1024

int main(void)
{
    uint32_t totalsize = TEST_SIZE;
    q31_t output_q31[totalsize];
    float32_t output[totalsize], golden[totalsize];
    float32_t NRMSD;

    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;
    char path_buffer[512];
    sprintf (path_buffer, "%s/golden_log_q31_in_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&golden, sizeof(float32_t), totalsize, gptr);
    q63_t count = 1;
    for (uint32_t i = 0; i < totalsize; i++)
    {
        q31_t out_q31 = riscv_dsp_log_q31(count);  // 5.26
        output_q31[i] = out_q31;
        count = count + 4194304;
    }
    riscv_dsp_convert_q31_f32(output_q31, output, totalsize);

    dump2_f32(golden, output, totalsize);

    fclose(gptr);

    NRMSD = dump_rmsd(golden, output, totalsize);

    if (NRMSD <= 0.00005)
        return 0;
    else
        return -1;
}
