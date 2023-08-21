#include "riscv_dsp_utils_math.h"

float32_t srcy[BUF_SIZE_F32] FUNC_ATTR_ALIGN;
float32_t srcx[BUF_SIZE_F32] FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples = SIZE_F32;
    float32_t ret = 0.0f;
    uint32_t i, j;
    acc_result result = {0, 0};

    for (j = 0 ; j < TEST_LOOP_F32; j++)
    {
        for (i = 0; i < BUF_SIZE_F32; i++)
        {
            srcy[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
            srcx[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
        }
#ifdef PF_COUNTER
        startPFM;
#endif
        for (i = 0; i < BUF_SIZE_F32; i++)
        {
            ret =  riscv_dsp_atan2_f32(srcy[i], srcx[i]);
        }
#ifdef PF_COUNTER
        stopPFM;
        ACC_Result(&result);
#endif
    }

#ifdef PF_COUNTER
    Read_ACC_Result(&result);
#endif

    return ret;
}
