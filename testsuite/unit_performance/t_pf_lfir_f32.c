#include "riscv_dsp_filtering_math.h"

float32_t fsrcf32[BUF_SIZE_F32] FUNC_ATTR_ALIGN, foutf32[BUF_SIZE_F32] FUNC_ATTR_ALIGN;
float32_t lstf32[LFIR_STATE_LEN] FUNC_ATTR_ALIGN = {0};
float32_t lcof32[LFIR_STATE_LEN] FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples = SIZE_F32;
    uint32_t i, j;
    acc_result result = {0, 0};

    for (i = 0; i < LFIR_STATE_LEN; i++)
    {
        lcof32[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }
    riscv_dsp_lfir_f32_t lf32 = {LFIR_STATE_LEN, lstf32, lcof32};

    for (j = 0; j < TEST_LOOP_F32; j++)
    {
        for (i = 0; i < BUF_SIZE_F32; i++)
        {
            fsrcf32[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
        }
#ifdef PF_COUNTER
        startPFM;
#endif
        riscv_dsp_lfir_f32(&lf32, fsrcf32, foutf32, BUF_SIZE_F32);
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