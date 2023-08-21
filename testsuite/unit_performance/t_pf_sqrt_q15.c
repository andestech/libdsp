#include "riscv_dsp_utils_math.h"

int main(void)
{
    q15_t ret;
    acc_result result = {0, 0};
    uint32_t j;
    total_samples = SAMPLES;

    for(j = 0; j < SAMPLES; j++)
    {  
#ifdef PF_COUNTER
        startPFM;
#endif
        ret = riscv_dsp_sqrt_q15(j);
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

