#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_test_data.h"
#include "riscv_dsp_transform_math.h"

int main(void)
{
    uint32_t i;
    FILE *fptr;
    fptr = fopen("golden_dct4_f32.bin", "r");
    FILE *gptr;
    gptr = fopen("golden_idct4_f32.bin", "r");
    float32_t NRMSD;

    q15_t buf[TRANSFORM_SAMPLES] __attribute__ ((aligned(4)));
    q15_t buf2[TRANSFORM_SAMPLES] __attribute__ ((aligned(4)));
    float32_t golden_dct[TRANSFORM_SAMPLES];
    float32_t golden_idct[TRANSFORM_SAMPLES];

    test_Fread(golden_dct, sizeof(float32_t), TRANSFORM_SAMPLES, fptr);
    test_Fread(golden_idct, sizeof(float32_t), TRANSFORM_SAMPLES, gptr);

    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        buf[i] = (convert_float_to_q15(test_fft_Input[i])) >> 1;
    }
    dump_q15("randomly generated input array", buf, TRANSFORM_SAMPLES);

    riscv_dsp_dct4_q15(buf, TRANSFORM_SAMPLES_LOG);
    dump_q15("after DCT-IV (Forward)", buf, TRANSFORM_SAMPLES);

    /* check dct with golden */
    printf("--------------\n");
    NRMSD = dump2_goldendiff_q15("after DCT (Forward)", golden_dct, buf, TRANSFORM_SAMPLES, 0);
    printf("DCT4 out scaleup by %d\n", (4 << TRANSFORM_SAMPLES_LOG));
    LOG("--------------\n");

    if (RES_LOGN <= 10)
    {
        if (TRANSFORM_SAMPLES_LOG < 8)
        {
            if (NRMSD > 0.005)
            {
                printf("NRMSD = %2.8f > 0.005, DCT4 FAIL\n", NRMSD);
                return -1;
            }

        }
    }
    else
    {
        printf("NRMSD = %2.8f > 0.005, DCT4 FAIL\n", NRMSD);
    }

    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        buf2[i] = buf[i];
    }

    /* IDCT function */
    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        buf[i] = (convert_float_to_q15(golden_dct[i] / (4 << TRANSFORM_SAMPLES_LOG)));
    }
    dump_q15("randomly generated input array", buf, TRANSFORM_SAMPLES);

    riscv_dsp_idct4_q15(buf, TRANSFORM_SAMPLES_LOG);
    dump_q15("after DCT-IV (Backward)", buf, TRANSFORM_SAMPLES);

    /* check idct with golden */
    NRMSD = dump2_inv_goldendiff_q15("after IDCT (Backward)", golden_idct, buf, TRANSFORM_SAMPLES);
    printf("IDCT4 out scaleup by %d\n", 2);
    LOG("--------------\n");

    if (RES_LOGN <= 10)
    {
        if (TRANSFORM_SAMPLES_LOG < 8)
        {
            if (NRMSD > 0.008)
            {
                printf("NRMSD = %2.8f > 0.008, IDCT4 FAIL\n", NRMSD);
                return -1;
            }
        }
    }
    else
    {
        printf("NRMSD = %2.8f > 0.008, IDCT4\n", NRMSD);
    }

    riscv_dsp_idct4_q15(buf2, TRANSFORM_SAMPLES_LOG);
    dump_q15("after DCT-IV (Forward + Backward)", buf2, TRANSFORM_SAMPLES);

    /* check idct with golden */
    NRMSD = dump2_inv_goldendiff_q15("after IDCT (Forward + Backward)", golden_idct, buf2, TRANSFORM_SAMPLES);
    printf("IDCT4 out scaleup by %d\n", 2);
    LOG("--------------\n");

    fclose(fptr);
    fclose(gptr);

    if (RES_LOGN <= 10)
    {
        if (TRANSFORM_SAMPLES_LOG < 8)
        {
            if (NRMSD < 0.02)
            {
                return 0;
            }
            else
            {
                printf("NRMSD = %2.8f > 0.02, IDCT4 FAIL\n", NRMSD);
                return -1;
            }
        }
        else
            return 0;
    }
    else
    {
        printf("NRMSD = %2.8f > 0.02, IDCT4\n", NRMSD);
        return 0;
    }
}