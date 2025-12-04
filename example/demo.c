#include <stdlib.h>
#include "riscv_dsp_basic_math.h"

#define SIZE 1024
#define FUNC_ATTR_ALIGN  __attribute__ ((aligned(8)))

float32_t src1[SIZE] FUNC_ATTR_ALIGN;
float32_t src2[SIZE] FUNC_ATTR_ALIGN;
float32_t out[SIZE] FUNC_ATTR_ALIGN;

int main(void)
{
    for (int i = 0; i < SIZE; i++)
    {
        src1[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
        src2[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }
    riscv_dsp_add_f32(src1, src2, out, SIZE);
    return 0;
}
