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
    fptr = fopen("golden_dct4_f32.bin", "r");
    FILE *gptr;
    gptr = fopen("golden_idct4_f32.bin", "r");
    float32_t NRMSD = 0;

    float32_t golden_buf[TRANSFORM_SAMPLES];
    float32_t golden_ibuf[TRANSFORM_SAMPLES];
    test_Fread(&golden_buf, sizeof(float32_t), TRANSFORM_SAMPLES, fptr);
    test_Fread(&golden_ibuf, sizeof(float32_t), TRANSFORM_SAMPLES, gptr);

    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        buf[i] = buf2[i] = (convert_float_to_q31(test_fft_Input[i])) >> 1;
    }
    dump_q31("randomly generated input array", buf, TRANSFORM_SAMPLES);

    riscv_dsp_dct4_q31(buf, TRANSFORM_SAMPLES_LOG);
    dump_q31("after DCT-IV (Forward)", buf, TRANSFORM_SAMPLES);

    /* check dct with golden */
    printf("--------------\n");
    NRMSD = dump2_goldendiff_q31("after DCT (Forward)", golden_buf, buf, TRANSFORM_SAMPLES, 0);
    printf("DCT out scaleup by %d\n", (4 << TRANSFORM_SAMPLES_LOG));
    LOG("--------------\n");

    if (TRANSFORM_SAMPLES_LOG < 8)
    {
        if (NRMSD > 0.0000005)
        {
            printf("NRMSD = %2.8f > 0.0000005, DCT4 FAIL\n", NRMSD);
            return -1;
        }
    }

    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        buf3[i] = buf[i];
    }

    /* IDCT function */
    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        buf[i] = (convert_float_to_q31(golden_buf[i] / (4 << TRANSFORM_SAMPLES_LOG)));
    }
    dump_q31("randomly generated input array", buf, TRANSFORM_SAMPLES);

    riscv_dsp_idct4_q31(buf, TRANSFORM_SAMPLES_LOG);
    dump_q31("after IDCT-IV (Backward)", buf, TRANSFORM_SAMPLES);

    /* check idct with golden */
    NRMSD = dump2_inv_goldendiff_q31("after IDCT (Backward)", golden_ibuf, buf, TRANSFORM_SAMPLES);
    printf("IDCT out scaleup by %d\n", 2);
    LOG("--------------\n");

    if (TRANSFORM_SAMPLES_LOG < 8)
    {
        if (NRMSD > 0.000001)
        {
            printf("NRMSD = %2.8f > 0.000001, IDCT4 FAIL\n", NRMSD);
            return -1;
        }
    }

    riscv_dsp_idct4_q31(buf3, TRANSFORM_SAMPLES_LOG);
    dump_q31("after IDCT-IV (Forward + Backward)", buf3, TRANSFORM_SAMPLES);

    /* check idct with golden */
    NRMSD = dump2_inv_goldendiff_q31("after IDCT (Forward + Backward)", golden_ibuf, buf3, TRANSFORM_SAMPLES);
    printf("IDCT out scaleup by %d\n", 2);
    LOG("--------------\n");

    fclose(fptr);
    fclose(gptr);

    if (RES_LOGN <= 10)
    {
        if (TRANSFORM_SAMPLES_LOG < 8)
        {
            if (NRMSD < 0.000001)
            {
                return 0;
            }
            else
            {
                printf("NRMSD = %2.8f > 0.000001, IDCT4 FAIL\n", NRMSD);
                return -1;
            }
        }
        else
            return 0;
    }
    else
    {
        printf("NRMSD = %2.8f > 0.000001, IDCT4\n", NRMSD);
        return 0;
    }
}
