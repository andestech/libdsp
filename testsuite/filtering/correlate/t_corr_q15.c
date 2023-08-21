#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_filtering_math.h"
#include <string.h>

typedef struct
{
    int len1;
    int len2;
    q15_t * buff;
} struCorr;

q15_t corrData_64A[] =
{
    0,     16,    682,   2894,   5973,   1496,   1512,   7982,
    -7754,   7445,   3820,  -2765,  -1119,  -7077,  -7273,    832,
    -3817,    309,   4134,   4885,  -2032,  -7675,  -1307,  -7940,
    -3106,   3087,  -1863,  -7036,   1257,  -3027,   -248,   1939,
    -1739,  -3532,   1653,   4150,    325,   6199,  -5262,  -5228,
    -4052,    102,  -6154,   2071,  -6267,  -3754,   3056,   8138,
    -8037,   5176,  -4888,   6016,  -2027,   6078,  -7779,   7009,
    -7492,  -5440,   2803,  -2773,  -2263,  -3782,    861,   -546
};

q15_t corrData_20B[] =
{
    1213,  1257,   7084,  -1470,   1829,   4242,  2766,  1581,
    2494,    90,   -912,   1120,  -5824,  -6787,  -150,  2584,
    2217,  -813,  -3039,  -5542
};

q15_t golden16_12[] =
{
    0,     0,      0,      0,     0,      0,    22,    79,
    126,   -54,    280,    888,   252,   1595,  1616,   287,
    1579,  2255,    581,   1093,  2227,  -2780,   252,  -603,
    -1391,  -187,  -1302,  -1922,  -361,   -237,    30
};

q15_t golden16_11[] =
{
    0,     0,      0,      0,     0,      0,     0,   -18,
    -77,  -105,    229,    615,   517,   1341,  1485,   381,
    1617,  2497,    829,   1065,  2227,  -2780,   252,  -603,
    -1391,  -187,  -1302,  -1922,  -361,   -237,    30
};

q15_t golden16_10[] =
{
    0,     0,      0,      0,     0,      0,     0,     0,
    3,    60,    270,    657,   739,   1125,  1693,   488,
    1540,  2465,    632,    862,  2250,  -2780,   252,  -603,
    -1391,  -187,  -1302,  -1922,  -361,   -237,    30,
};

q15_t golden16_3[] =
{
    0,     0,      0,      0,     0,      0,     0,    0,
    0,     0,      0,      0,     0,      0,     3,  148,
    652,  1427,    659,    605,  1838,  -1314,  1607,  824,
    -175,  -206,  -1675,  -1885,  -361,   -237,    30
};

q15_t golden9_16[] =
{
    0,      1,   53,   221,   313,   -629,  -1616,    -668,
    -1100,  -1940,  468,  2266,   252,   1341,   1693,     467,
    963,   1823,   33,  -185,  2186,  -1314,     -1,    -287,
    0,      0,    0,     0,     0,      0,      0
};

struCorr CorrArray[] =
{
    {16, 12, golden16_12},
    {16, 3, golden16_3},
    {9, 16, golden9_16},
    {16, 11, golden16_11},
    {16, 10, golden16_10},
};


float32_t sub_corr(struCorr * instance, float32_t tmpresult[])
{
    int32_t i;
    int len1 = instance->len1;
    int len2 = instance->len2;
    q15_t inputa[len1] __attribute__ ((aligned(4)));
    q15_t inputb[len2] __attribute__ ((aligned(4)));
    int clen = 2 * len1 - 1;

    if (len2 > len1)
    {
        clen = 2 * len2 - 1;
    }
    q15_t output[clen] __attribute__ ((aligned(4)));

    //Initialize because the values on board must not be zero.
    //Not all of elements in output will be updated according to correlate algorithm.
    for (i = 0; i < clen; i++)
    {
        output[i] = 0;
    }
    for (i = 0; i < len1; i++)
    {
        inputa[i] = corrData_64A[i];
    }
    for (i = 0; i < len2; i++)
    {
        inputb[i] = corrData_20B[i];
    }

    riscv_dsp_corr_q15(inputa, len1, inputb, len2, output);

    float32_t NRMSD;
    float32_t golden_f32[clen];
    float32_t output_f32[clen];
    q15_t *golden = instance->buff;

    riscv_dsp_convert_q15_f32(golden, golden_f32, clen);
    riscv_dsp_convert_q15_f32(output, output_f32, clen);

    //dump2_q15(golden, output, clen);

    NRMSD = dump_rmsd2(golden_f32, output_f32, clen, tmpresult);
    return NRMSD;
}

int main(void)
{
    uint32_t i;
    float32_t tmpresult[4] = {0.0};
    float32_t result[4] = {0.0};
    float32_t tmpNRMSD, NRMSD = 0.0f;

    for (i = 0; i < sizeof(CorrArray) / sizeof(struCorr); i++)
    {
        tmpNRMSD = sub_corr(&CorrArray[i], tmpresult);
        if (tmpNRMSD >= NRMSD)
        {
            result[0] = tmpresult[0];
            result[1] = tmpresult[1];
            result[2] = tmpresult[2];
            result[3] = tmpresult[3];
            NRMSD = tmpNRMSD;
        }
    }
    printf("MAE is %5.8f, RMSD is %5.8f, NRMSD is %5.8f, MAXDIFF is %5.8f, SNR is %5.8f\n", result[0], result[1], NRMSD, result[2], result[3]);
    return 0;
}
