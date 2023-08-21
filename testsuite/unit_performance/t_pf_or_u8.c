#include "riscv_dsp_basic_math.h"

u8_t srcu8_1[SIZE] FUNC_ATTR_ALIGN;
u8_t srcu8_2[SIZE] FUNC_ATTR_ALIGN;
u8_t outu8[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        srcu8_1[i] = (u8_t)(rand() >> 24);
        srcu8_2[i] = (u8_t)(rand() >> 24);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_or_u8(srcu8_1, srcu8_2, outu8, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
