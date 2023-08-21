#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_utils_math.h"
#include "riscv_dsp_test_accuracy.h"


#define interval1 (40000/4)
int main(void)
{
    int i;
    float inputy;
    float golden[8 * interval1 + 2], out[8 * interval1 + 2];

    //special 1
    for (i = 0; i < interval1; i++)
    {
        inputy = i * 4;
        golden[i] = atan2f(inputy , 1.0f);
        out[i] = riscv_dsp_atan2_f32(inputy, 1.0f);
    }
    for (i = 0; i < interval1; i++)
    {
        inputy = i * 4;
        golden[i+interval1] = atan2f(inputy , -1);
        out[i+interval1] = riscv_dsp_atan2_f32(inputy, -1);
    }
    for (i = 0; i < interval1; i++)
    {
        inputy = i * 4;
        golden[i+2*interval1] = atan2f(-inputy , 1);
        out[i+2*interval1] = riscv_dsp_atan2_f32(-inputy, 1);
    }
    for (i = 0; i < interval1; i++)
    {
        inputy = i * 4;
        if (i == 0)
            inputy = -inputy;
        golden[i+3*interval1] = atan2f(-inputy , -1);
        out[i+3*interval1] = riscv_dsp_atan2_f32(-inputy, -1);
    }

    // special 2
    for (i = 0; i < interval1; i++)
    {
        inputy = (float)0.1 / (4 * i + 1);
        golden[i+4*interval1] = atan2f(inputy , 0.1);
        out[i+4*interval1] = riscv_dsp_atan2_f32(inputy, 0.1);
    }
    for (i = 0; i < interval1; i++)
    {
        inputy = (float)0.1 / (4 * i + 1);
        golden[i+5*interval1] = atan2f(inputy , -0.1);
        out[i+5*interval1] = riscv_dsp_atan2_f32(inputy, -0.1);
    }
    for (i = 0; i < interval1; i++)
    {
        inputy = (float)0.1 / (4 * i + 1);
        golden[i+6*interval1] = atan2f(-inputy , 0.1);
        out[i+6*interval1] = riscv_dsp_atan2_f32(-inputy, 0.1);
    }
    for (i = 0; i < interval1; i++)
    {
        inputy = (float)0.1 / (4 * i + 1);
        golden[i+7*interval1] = atan2f(-inputy , -0.1);
        out[i+7*interval1] = riscv_dsp_atan2_f32(-inputy, -0.1);
    }

    golden[8*interval1] = atan2f(123 , 0);
    out[8*interval1] = riscv_dsp_atan2_f32(123, 0);
    golden[8*interval1+1] = atan2f(-123 , 0);
    out[8*interval1+1] = riscv_dsp_atan2_f32(-123, 0);

    dump2_f32(golden, out, 8*interval1 + 2);
    float32_t NRMSD;
    float32_t result[4] = {0};
    NRMSD = dump_rmsd2(golden, out, 8 * interval1 + 2, result);

    printf("MAE is %5.8f, RMSD is %5.8f, NRMSD is %5.8f, MAXDIFF is %5.8f, SNR is %5.8f\n", result[0], result[1], NRMSD, result[2], result[3]);
    if (result[0] <= 0.000001)
        return 0;
    else
        return -1;
}
