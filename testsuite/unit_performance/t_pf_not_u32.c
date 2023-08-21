#include "riscv_dsp_basic_math.h"

u32_t srcu32_1[SIZE] FUNC_ATTR_ALIGN;
u32_t outu32[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        srcu32_1[i] = (u32_t)rand();
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_not_u32(srcu32_1, outu32, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
