#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "riscv_dsp_utils_math.h"
#include "riscv_dsp_test_accuracy.h"

#define TEST_NDS32_SIN_F_USE_ARRAY
#include "riscv_dsp_test_data.h"

#define SIN_RESN 1024

int main(void)
{
    int n;
    float tmp = PI / (SIN_RESN << 2);
    int blocksize = (SIN_RESN << 4);
    float32_t NRMSD;
    float f1[2 * blocksize], f2[2 * blocksize];

    /* test from -4 * PI to 4 * PI */
    for (n = 0; n < (2 * blocksize); n++)
    {
        float input = (float)(n - blocksize) * tmp;
#ifdef TEST_NDS32_SIN_F_USE_ARRAY
        f1[n] = golden_sin_f32[n];
#else
        f1[n] = sinf(input);
#endif
        f2[n] = riscv_dsp_sin_f32(input);
    }

    dump2_f32(f1, f2, 2 * blocksize);
    NRMSD = dump_rmsd(f1, f2, 2 * blocksize);

    if (NRMSD <= 0.00000009)
        return 0;
    else
        return -1;
}
