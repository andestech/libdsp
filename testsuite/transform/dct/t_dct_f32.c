#include <stdlib.h>
#include <stdio.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_test_data.h"
#include "riscv_dsp_transform_math.h"

static float buf[TRANSFORM_SAMPLES];
static float buf2[TRANSFORM_SAMPLES];
static float dct_golden[TRANSFORM_SAMPLES];

int main(void)
{
    FILE *fptr;
    fptr = fopen("golden_dct_f32.bin", "w");
    FILE *gptr;
    gptr = fopen("golden_idct_f32.bin", "w");

    uint32_t i;
    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        buf[i] = buf2[i] = test_fft_Input[i];
        dct_golden[i] = dct_f32_golden[i];
    }
    dump_f32("randomly generated input array", buf, TRANSFORM_SAMPLES);

    riscv_dsp_dct_f32(buf, TRANSFORM_SAMPLES_LOG);
    float32_t NRMSD;
    printf("after DCT (Forward), ");
    dump2_f32(dct_golden, buf, TRANSFORM_SAMPLES);
    NRMSD = dump_rmsd(dct_golden, buf, TRANSFORM_SAMPLES);
    if (TRANSFORM_SAMPLES_LOG <= 7)
    {
        if (NRMSD > 0.000003)
        {
            printf("NRMSD = %2.8f > 0.000003, FAIL\n", NRMSD);
            return -1;
        }
    }
    else
    {
        if (NRMSD > 0.00004)
        {
            printf("NRMSD = %2.8f > 0.00004, FAIL\n", NRMSD);
            return -1;
        }
    }

    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        fwrite(&dct_golden[i], sizeof(float32_t), 1, fptr);
    }
    fclose(fptr);

    riscv_dsp_idct_f32(dct_golden, TRANSFORM_SAMPLES_LOG);

    printf("after IDCT (Backward), ");
    dump2_f32(buf2, dct_golden, TRANSFORM_SAMPLES);
    NRMSD = dump_rmsd(buf2, dct_golden, TRANSFORM_SAMPLES);

    if (TRANSFORM_SAMPLES_LOG <= 7)
    {
        if (NRMSD > 0.000003)
        {
            printf("NRMSD = %2.8f > 0.000003, FAIL\n", NRMSD);
            return -1;
        }
    }
    else
    {
        if (NRMSD > 0.00004)
        {
            printf("NRMSD = %2.8f > 0.00004, FAIL\n", NRMSD);
            return -1;
        }
    }

    dump_f32("after IDCT ", buf, TRANSFORM_SAMPLES);

    riscv_dsp_idct_f32(buf, TRANSFORM_SAMPLES_LOG);
    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        fwrite(&buf2[i], sizeof(float32_t), 1, gptr);
    }
    fclose(gptr);
    dump_f32("after IDCT(Forward + Backward)", buf, TRANSFORM_SAMPLES);

    printf("after IDCT (Forward + Backward), ");
    dump2_f32(buf2, buf, TRANSFORM_SAMPLES);
    NRMSD = dump_rmsd(buf2, buf, TRANSFORM_SAMPLES);

    if (TRANSFORM_SAMPLES_LOG <= 7)
    {
        if (NRMSD < 0.000003)
        {
            return 0;
        }
        else
        {
            printf("NRMSD = %2.8f > 0.000003, FAIL\n", NRMSD);
            return -1;
        }
    }
    else
    {
        if (NRMSD < 0.00004)
        {
            return 0;
        }
        else
        {
            printf("NRMSD = %2.8f > 0.00004, FAIL\n", NRMSD);
            return -1;
        }
    }
}
