#include "riscv_dsp_utils_math.h"

float16_t srcf16[BUF_SIZE_F32]  FUNC_ATTR_ALIGN;

int main(void)
{
    float16_t ret;
    total_samples = BUF_SIZE_F32;
    acc_result result = {0, 0};
    unsigned long long count = 0;
    uint32_t i;

    for (i = 0; i < BUF_SIZE_F32; i++)
    {
        srcf16[i] = gen_frandom();
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    for (i = 0; i < BUF_SIZE_F32; i++)
    {
        ret = riscv_dsp_exp_f16(srcf16[i]);
    }
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
