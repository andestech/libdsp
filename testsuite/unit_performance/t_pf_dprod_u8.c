#include "riscv_dsp_basic_math.h"

uint8_t src_1[SIZE] FUNC_ATTR_ALIGN;
uint8_t src_2[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    uint32_t ret;

    for (i = 0; i < SIZE; i++)
    {
        src_1[i] = (rand() % 256);
        src_2[i] = (rand() % 256);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_dprod_u8(src_1, src_2, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
