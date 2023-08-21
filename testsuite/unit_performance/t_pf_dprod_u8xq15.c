#include "riscv_dsp_basic_math.h"

uint8_t srcu8[SIZE] FUNC_ATTR_ALIGN;
q15_t srcq15[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    q31_t ret;

    for (i = 0; i < SIZE; i++)
    {
        srcu8[i] = (rand() % 256);
        srcq15[i] = (q15_t)(rand() >> 16);
        //    printf("%d %d\n", srcu8[i], srcq15[i]);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_dprod_u8xq15(srcu8, srcq15, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    PRINTF_INT32(ret);
    return ret;
}
