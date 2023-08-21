#include "riscv_dsp_utils_math.h"

int main(void)
{
    q31_t ret, input;
    q63_t tmp;
    acc_result result = {0, 0};
    int32_t j;
    total_samples = SAMPLES;

    for (j = -SAMPLES; j < SAMPLES; j += 2)
    {
        tmp = j * (interval_q * 2147483648.0f);

        if (tmp >= 2147483648LL)
            input = (q31_t) 2147483647;
        else
            input = (q31_t) tmp;
#ifdef PF_COUNTER
        startPFM;
#endif
        ret = riscv_dsp_cos_q31(input);
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
