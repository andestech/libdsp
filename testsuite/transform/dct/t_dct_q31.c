#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>       //for PRIx32
#include "riscv_dsp_transform_math.h"
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_test_data.h"

static q31_t buf[TRANSFORM_SAMPLES];
static q31_t buf2[TRANSFORM_SAMPLES];
static q31_t buf3[TRANSFORM_SAMPLES];

int main(void)
{
    uint32_t i;
    FILE *fptr;
    fptr = fopen("golden_dct_f32.bin", "r");
    FILE *gptr;
    gptr = fopen("golden_idct_f32.bin", "r");
    float32_t NRMSD;

    float32_t golden_dct[TRANSFORM_SAMPLES];
    float32_t golden_idct[TRANSFORM_SAMPLES];
    test_Fread(&golden_dct, sizeof(float32_t), TRANSFORM_SAMPLES, fptr);
    test_Fread(&golden_idct, sizeof(float32_t), TRANSFORM_SAMPLES, gptr);

    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        buf[i] = buf2[i] = (convert_float_to_q31(test_fft_Input[i])) >> 1;
    }
    dump_q31("randomly generated input array", buf, TRANSFORM_SAMPLES);

    riscv_dsp_dct_q31(buf, TRANSFORM_SAMPLES_LOG);
    dump_q31("after DCT", buf, TRANSFORM_SAMPLES);

    /* check dct with golden */
    printf("--------------\n");
    NRMSD = dump2_goldendiff_q31("after DCT (Forward)", golden_dct, buf, TRANSFORM_SAMPLES, 1);
    printf("DCT out scaleup by %d\n", (2 << TRANSFORM_SAMPLES_LOG));
    LOG("--------------\n");

    if (TRANSFORM_SAMPLES_LOG < 9)
    {
        if (NRMSD > 0.0000005)
        {
            printf("NRMSD = %2.8f > 0.0000005, DCT FAIL\n", NRMSD);
            return -1;
        }
    }

    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        buf3[i] =  buf[i];
    }

    /* IDCT function */
    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        buf[i] = (convert_float_to_q31(golden_dct[i] / (2 << TRANSFORM_SAMPLES_LOG)));
    }
    dump_q31("randomly generated input array", buf, TRANSFORM_SAMPLES);

    riscv_dsp_idct_q31(buf, TRANSFORM_SAMPLES_LOG);
    dump_q31("after iDCT", buf, TRANSFORM_SAMPLES);

    /* check idct with golden */
    NRMSD = dump2_inv_goldendiff_q31("after IDCT (Backward)", golden_idct, buf, TRANSFORM_SAMPLES);
    printf("IDCT out scaleup by %d\n", 2);
    LOG("--------------\n");


    if (TRANSFORM_SAMPLES_LOG < 9)
    {
        if (NRMSD > 0.000005)
        {
            printf("NRMSD = %2.8f > 0.000005, IDCT FAIL\n", NRMSD);
            return -1;
        }
    }

    riscv_dsp_idct_q31(buf3, TRANSFORM_SAMPLES_LOG);
    NRMSD = dump2_inv_goldendiff_q31("after IDCT (Forward + Backward)", golden_idct, buf3, TRANSFORM_SAMPLES);
    LOG("--------------\n");

    fclose(fptr);
    fclose(gptr);

    if (TRANSFORM_SAMPLES_LOG < 9)
    {
        if (NRMSD < 0.000005)
        {
            return 0;
        }
        else
        {
            printf("NRMSD = %2.8f > 0.000005, IDCT FAIL\n", NRMSD);
            return -1;
        }
    }
    else
        return 0;
}
