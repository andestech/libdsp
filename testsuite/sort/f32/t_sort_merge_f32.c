#include "riscv_dsp_sort_math.h"
#include "riscv_dsp_test_accuracy.h"

#define SORT_FAIL -1
#define SORT_PASS 0
static inline int verify_sort_f32(riscv_dsp_sort_merge_f32_t * instance, float *outPtr, float *goldenPtr, uint32_t size)
{
    uint32_t i;
    float maxDiff = 0.0f;
#if 0
    float NRMDS = dump_rmsd(goldenPtr, outPtr, size);
    maxDiff = NRMDS;
#else
    for (i = 0; i < size; i++)
    {
        float diff = fabsf(goldenPtr[i] - outPtr[i]);
        maxDiff = (diff > maxDiff) ? diff : maxDiff;
#ifdef PRINT_DETAIL
        if (diff > 0.0000002)
            printf("out[%d] = %.8f, golden[%d] = %.8f\n", i, outPtr[i], i, goldenPtr[i]);
#endif
    }
#endif
    int order = instance->order;
    printf("sort_merge, ");
    if (order == 1)
        printf("ascending: ");
    else
        printf("descending: ");
    if (maxDiff > 0.0000002)
    {
        printf("FAIL\n");
        return SORT_FAIL;
    }
    else
    {
        printf("PASS\n");
        return SORT_PASS;
    }
}

int main(void)
{
    uint32_t size = 256;
    float src[size], out[size];
    float golden_ascend[size], golden_descend[size];

    FILE *fptr = NULL, *gptr1 = NULL, *gptr2 = NULL;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[1024];
    sprintf (path_buffer, "%s/input_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_sort_ascend_f32.bin", srcdir);
    gptr1 = fopen(path_buffer, "r");
    if (gptr1 == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_sort_descend_f32.bin", srcdir);
    gptr2 = fopen(path_buffer, "r");
    if (gptr2 == NULL)
        return -1;

    test_Fread(&src, sizeof(float32_t), size, fptr);
    //dump_f32("input: ", src, size);
    test_Fread(&golden_ascend, sizeof(float32_t), size, gptr1);
    test_Fread(&golden_descend, sizeof(float32_t), size, gptr2);

    riscv_dsp_sort_merge_f32_t instance;
    float tmpbuf[size];

    riscv_dsp_sort_merge_init_f32(&instance, RISCV_DSP_SORT_ASCENDING, tmpbuf);
    riscv_dsp_sort_merge_f32(&instance, src, out, size);
    int check1 = verify_sort_f32(&instance, out, golden_ascend, size);

    riscv_dsp_sort_merge_init_f32(&instance, RISCV_DSP_SORT_DESCENDING, tmpbuf);
    riscv_dsp_sort_merge_f32(&instance, src, out, size);
    int check2 = verify_sort_f32(&instance, out, golden_descend, size);

    fclose(fptr);
    fclose(gptr1);
    fclose(gptr2);

    if ((check1 == -1) || (check2 == -1))
        return -1;
    else
        return 0;
}
