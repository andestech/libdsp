#include "riscv_dsp_utils_math.h"
#include "math.h"

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
        float16_t tmpin = (float16_t)fabsf(gen_frandom() + 0.00007);
        srcf16[i] = tmpin;
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    for (i = 0; i < BUF_SIZE_F32; i++)
    {
        ret = riscv_dsp_log_f16(srcf16[i]);
    }
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
