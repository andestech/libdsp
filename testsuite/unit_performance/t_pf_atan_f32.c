#include "riscv_dsp_utils_math.h"

float32_t srcf32[BUF_SIZE_F32] FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples = SIZE_F32;
    float32_t ret = 0.0f;
    unsigned long long count = 0;
    uint32_t i, j;
    acc_result result = {0, 0};

    for (j = 0 ; j < TEST_LOOP_F32; j++)
    {
        for (i = 0; i < BUF_SIZE_F32; i++)
        {
            srcf32[i] = count * 16384 / 32768.0f; // average sampling  (2 ^ 31) / 131072 = 16384
            count++;
        }
#ifdef PF_COUNTER
        startPFM;
#endif
        for (i = 0; i < BUF_SIZE_F32; i++)
        {
            ret =  riscv_dsp_atan_f32(srcf32[i]);
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
