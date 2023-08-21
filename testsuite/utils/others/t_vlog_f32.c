#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_utils_math.h"

#define TEST_SIZE 1023

int main(void)
{
    uint32_t totalsize = TEST_SIZE;
    float32_t input[totalsize];
    float32_t output[totalsize], golden[totalsize];
    float32_t NRMSD;

    for(int i = 0; i < totalsize; i++)
    {
        float32_t tmpin = fabsf(gen_frandom() + 0.0000001);
        input[i] = tmpin;
        golden[i] = logf(tmpin);

    }
    dump_f32("input", input, totalsize);

    riscv_dsp_vlog_f32(input, output, totalsize);

    dump2_f32(golden, output, totalsize);
    NRMSD = dump_rmsd(golden, output, totalsize);

    if (NRMSD <= 0.0000001)
        return 0;
    else
        return -1;
}
