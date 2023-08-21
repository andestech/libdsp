#include "riscv_dsp_basic_math.h"

q15_t src1[SIZE]  FUNC_ATTR_ALIGN;
q15_t src2[SIZE]  FUNC_ATTR_ALIGN;

int main(void)
{
    uint32_t i;
    q15_t ret;
    int16_t shift;

    for (i = 0; i < SIZE; i++)
    {
        src1[i] = (q15_t)(rand() >> 16);
        src2[i] = (q15_t)(rand() >> 16);
    }

#ifdef PF_COUNTER
        startPFM;
#endif
    for (i = 0; i < SIZE; i++)
    {
        riscv_dsp_divide_q15(src1[i], src2[i] , &ret, &shift);
    }
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    printf("sfift : %u\n", (unsigned int)shift);
    return ret;
}
