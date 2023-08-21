#include "riscv_dsp_statistics_math.h"

q31_t src1[SIZE] FUNC_ATTR_ALIGN;
q31_t src2[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t bsize = SIZE;
    uint32_t i;
    q31_t result;

    for(i = 0; i < bsize; i++)
    {
        src1[i] = (q31_t)rand();
        src2[i] = (q31_t)rand();
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_mse_q31(src1, src2, bsize, &result);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return (int) result;
}
