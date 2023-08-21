#include "riscv_dsp_complex_math.h"

float32_t srcf32[BUF_SIZE_F32] FUNC_ATTR_ALIGN;
float32_t srcf32_2[BUF_SIZE_F32] FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples = SIZE_F32;
    acc_result result = {0, 0};
    uint32_t i, j;
    float32_t a1, b1;

    for (j = 0 ; j < TEST_LOOP_F32; j++)
    {
        for (i = 0; i < BUF_SIZE_F32; i++)
        {
            srcf32[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
            srcf32_2[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
        }

#ifdef PF_COUNTER
        startPFM;
#endif
        riscv_dsp_cdprod_typ2_f32(srcf32, srcf32_2, BUF_SIZE_F32 / 2, &a1, &b1);
#ifdef PF_COUNTER
        stopPFM;
        ACC_Result(&result);
#endif
    }
#ifdef PF_COUNTER
    Read_ACC_Result(&result);
#endif

    return (a1 + b1);
}
