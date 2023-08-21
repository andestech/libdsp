#include "riscv_dsp_statistics_math.h"

q7_t src1[SIZE] FUNC_ATTR_ALIGN;
q7_t src2[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t bsize = SIZE;
    uint32_t i;
    q7_t result;

    for(i = 0; i < bsize; i++)
    {
        src1[i] = (q7_t)(rand() >> 24);
        src2[i] = (q7_t)(rand() >> 24);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_mse_q7(src1, src2, bsize, &result);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return (int) result;
}
