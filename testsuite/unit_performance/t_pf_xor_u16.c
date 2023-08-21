#include "riscv_dsp_basic_math.h"

u16_t srcu16_1[SIZE] FUNC_ATTR_ALIGN;
u16_t srcu16_2[SIZE] FUNC_ATTR_ALIGN;
u16_t outu16[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        srcu16_1[i] = (u16_t)(rand() >> 16);
        srcu16_2[i] = (u16_t)(rand() >> 16);
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_xor_u16(srcu16_1, srcu16_2, outu16, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
