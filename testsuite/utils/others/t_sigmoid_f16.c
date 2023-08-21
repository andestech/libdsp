#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_utils_math.h"

#define TEST_SIZE 16383

int main(void)
{
    uint32_t totalsize = TEST_SIZE;
    float32_t input[totalsize];
    float16_t inf16[totalsize];
    float16_t outf16[totalsize];
    float32_t output[totalsize], golden[totalsize];
    float32_t NRMSD;
    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_sigmoid_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;
    sprintf (path_buffer, "%s/golden_sigmoid_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&input, sizeof(float32_t), totalsize, fptr);
    test_Fread(&golden, sizeof(float32_t), totalsize, gptr);

    for (uint32_t i = 0; i < totalsize; i++)
    {
        inf16[i] = (float16_t)input[i];
        outf16[i] = riscv_dsp_sigmoid_f16(inf16[i]);
        output[i] = (float32_t)outf16[i];
    }

    dump2_f32(golden, output, totalsize);

    fclose(fptr);
    fclose(gptr);

    NRMSD = dump_rmsd(golden, output, totalsize);

#ifdef ENA_HIGH_ACCURACY_F16
    if (NRMSD <= 0.00011)
#else
    if (NRMSD <= 0.00014)
#endif
        return 0;
    else
        return -1;
}
