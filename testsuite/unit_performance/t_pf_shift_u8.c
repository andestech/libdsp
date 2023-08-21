#include "riscv_dsp_basic_math.h"

uint8_t src_1[SIZE] FUNC_ATTR_ALIGN;
uint8_t out[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t blocksize = SIZE, i;

    for (i = 0; i < SIZE; i++)
    {
        src_1[i] = (rand() % 256);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_shift_u8(src_1, SHIFT_BITS, out, blocksize);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
