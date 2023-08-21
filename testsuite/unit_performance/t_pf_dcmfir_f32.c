#include "riscv_dsp_filtering_math.h"

float32_t fsrcf32[BUF_SIZE_F32]  FUNC_ATTR_ALIGN;
float32_t doutf32[BUF_SIZE_F32 / DCM_FACTOR]  FUNC_ATTR_ALIGN;
float32_t dsf32[FIR_STATE_LEN]  FUNC_ATTR_ALIGN = {0};
float32_t dcof32[FIR_NUM_TAP]  FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples = SIZE_F32;
    uint32_t i, j;
    acc_result result = {0, 0};

    //generate coefficient
    for (i = 0; i < FIR_NUM_TAP; i++)
    {
        dcof32[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }

    riscv_dsp_dcmfir_f32_t df32 = {DCM_FACTOR, FIR_NUM_TAP, dcof32, dsf32};

    for (j = 0; j < TEST_LOOP_F32; j++)
    {
        for (i = 0; i < BUF_SIZE_F32; i++)
        {
            fsrcf32[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
        }
#ifdef PF_COUNTER
        startPFM;
#endif
        riscv_dsp_dcmfir_f32(&df32, fsrcf32, doutf32, BUF_SIZE_F32);
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
