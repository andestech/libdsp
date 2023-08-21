#include "riscv_dsp_utils_math.h"

int main(void)
{
    total_samples = SAMPLES;
    q15_t inputy, inputx, ret = 0;
    acc_result result = {0, 0};
    uint32_t j;

    for(j = 0; j < SAMPLES; j++)
    {  
        inputy = (q15_t) rand();
        inputx = (q15_t) rand();
#ifdef PF_COUNTER
        startPFM;
#endif    
        ret = riscv_dsp_atan2_q15(inputy, inputx);
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

