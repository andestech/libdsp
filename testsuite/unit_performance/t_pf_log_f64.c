#include <math.h>
#include "riscv_dsp_utils_math.h"

float64_t srcf64[BUF_SIZE_F32]  FUNC_ATTR_ALIGN;

int main(void)
{
    float64_t ret;
    total_samples = BUF_SIZE_F32;
    unsigned long long count = 0;
    uint32_t i;

    for (i = 0; i < BUF_SIZE_F32; i++)
    {
        float64_t tmpin = fabs(gen_frandom() + 0.0000001);
        srcf64[i] = tmpin;
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    for (i = 0; i < BUF_SIZE_F32; i++)
    {
        ret = riscv_dsp_log_f64(srcf64[i]);
    }
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
