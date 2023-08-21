#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_utils_math.h"

int main(void)
{
    int n;
    float tmp = 1.0f / RES_N;

    // test from -2 * PI to 2 * PI
    int size1 = RES_N;
    float L1f1[size1], L1f2[size1];
    float32_t Loop1[4] = {0.0f};
    int inx = 0;
    //initialize because the last value of array L1f1 and L1f2 won't be updated in the first loop.
    //on board, the initial value may not be zero.
    // L1f1[size1 - 1] = 0;
    // L1f2[size1 - 1] = 0;
    for (n = -RES_N; n < 0; n++)
    {
        L1f1[inx] = atanf(n);
        L1f2[inx] = riscv_dsp_atan_f32(n);
        inx++;
    }

    dump2_f32(L1f1, L1f2, size1);
    float32_t nrmsd1 = dump_rmsd2(L1f1, L1f2, size1, Loop1);

    int size2 = (RES_N << 1);
    float L2f1[size2], L2f2[size2];
    float32_t Loop2[4] = {0.0f};
    inx = 0;
    for (n = -RES_N; n < RES_N; n++)
    {
        L2f1[inx] = atanf(n * tmp);
        L2f2[inx] = riscv_dsp_atan_f32(n * tmp);
        inx++;
    }
    dump2_f32(L2f1, L2f2, size2);
    float32_t nrmsd2 = dump_rmsd2(L2f1, L2f2, size2, Loop2);

    int size3 = RES_N;
    float L3f1[size3], L3f2[size3];
    float32_t Loop3[4] = {0.0f};
    inx = 0;
    for (n = 1; n <= RES_N; n++)
    {
        L3f1[inx] = atanf(n);
        L3f2[inx] = riscv_dsp_atan_f32(n);
        inx++;
    }
    dump2_f32(L3f1, L3f2, size3);
    float32_t nrmsd3 = dump_rmsd2(L3f1, L3f2, size3, Loop3);

    Loop1[0] = (Loop1[0] + Loop2[0] + Loop3[0]) / 3;
    Loop1[1] = (Loop1[1] + Loop2[1] + Loop3[1]) / 3;
    Loop1[2] = (Loop1[2] + Loop2[2] + Loop3[2]) / 3;
    Loop1[3] = (Loop1[3] + Loop2[3] + Loop3[3]) / 3;
    nrmsd1 = (nrmsd1 + nrmsd2 + nrmsd3) / 3;

    printf("--------------------\n");
    printf("MAE is %5.8f, RMSD is %5.8f, NRMSD is %5.8f, MAXDIFF is %5.8f, SNR is %5.8f\n", Loop1[0], Loop1[1], nrmsd1, Loop1[2], Loop1[3]);

    if (nrmsd1 <= 0.00000001)
        return 0;
    else
        return -1;
}
