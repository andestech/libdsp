#include "riscv_dsp_utils_math.h"

float32_t srcf32[BUF_SIZE_F32]  FUNC_ATTR_ALIGN;
q15_t outq15[BUF_SIZE_F32] FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples = SIZE_F32;
    float32_t *ptrf32;
    uint32_t i, j;
    acc_result result = {0, 0};

    for (j = 0; j < TEST_LOOP_F32; j++)
    {
        for (ptrf32 = &srcf32[0], i = 0; i < BUF_SIZE_F32; i++)
        {
            *ptrf32++ = ((q15_t)(rand() >> 16)) / 32768.0f;
        }
#ifdef PF_COUNTER
        startPFM;
#endif
        riscv_dsp_convert_f32_q15(srcf32, outq15, BUF_SIZE_F32);
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