#include "riscv_dsp_complex_math.h"

float32_t srcf32[BUF_SIZE_F32] FUNC_ATTR_ALIGN;
float32_t clxoutf32[BUF_SIZE_F32 / 2] FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples = SIZE_F32;
    uint32_t i, j;
    acc_result result = {0, 0};

    for (j = 0; j < TEST_LOOP_F32; j++)
    {
        for (i = 0; i < BUF_SIZE_F32; i++)
        {
            srcf32[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
        }
#ifdef PF_COUNTER
        startPFM;
#endif
        riscv_dsp_cmag_sqr_f32(srcf32, clxoutf32, BUF_SIZE_F32 / 2);
#ifdef PF_COUNTER
        stopPFM;
        ACC_Result(&result);
#endif
    }

#ifdef PF_COUNTER
    Read_ACC_Result(&result);
#endif

    return 0;
}