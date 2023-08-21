#include "riscv_dsp_utils_math.h"

float32_t srcf32[BUF_SIZE_F32]  FUNC_ATTR_ALIGN;

int main(void)
{
    float32_t ret;
    total_samples = BUF_SIZE_F32;
    acc_result result = {0, 0};
    unsigned long long count = 0;
    uint32_t i;

    for (i = 0; i < BUF_SIZE_F32; i++)
    {
        srcf32[i] = gen_frandom();
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    for (i = 0; i < BUF_SIZE_F32; i++)
    {
        ret = riscv_dsp_exp_f32(srcf32[i]);
    }
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
