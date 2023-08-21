#include "riscv_dsp_basic_math.h"

uint8_t src[SIZE] FUNC_ATTR_ALIGN;
uint8_t out[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t blocksize = SIZE, i;
    q7_t offsetq7 = 30;

    for (i = 0; i < SIZE; i++)
    {
        src[i] = (rand() % 256);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_offset_u8(src, offsetq7, out, blocksize);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
