#include "riscv_dsp_basic_math.h"

q31_t src1[SIZE]  FUNC_ATTR_ALIGN;
q31_t src2[SIZE]  FUNC_ATTR_ALIGN;

int main(void)
{
    uint32_t i;
    q31_t ret;
    int16_t shift;

    for (i = 0; i < SIZE; i++)
    {
        src1[i] = (q31_t)(rand());
        src2[i] = (q31_t)(rand());
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    for (i = 0; i < SIZE; i++)
    {
        riscv_dsp_divide_q31(src1[i], src2[i] , &ret, &shift);
    }
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    printf("sfift : %u\n", (unsigned int)shift);
    return ret;
}
