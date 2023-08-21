#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_complex_math.h"

int main(void)
{
    uint32_t datasize = 5000;
    uint32_t numSample = 125;
    uint32_t loop = datasize / numSample, i;
    float32_t inputA[2*numSample], inputB[2*numSample];
    float32_t output[2 * loop], golden[2 * loop];

    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_cmplx_f32_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_cmplx_f32_b.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_cdprod_typ2_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&golden, sizeof(float32_t), 2 * loop, gptr);

    for (i = 0; i < loop; i++)
    {
        test_Fread(&inputA, sizeof(float32_t), 2*numSample, fptr);
        test_Fread(&inputB, sizeof(float32_t), 2*numSample, f2ptr);
        riscv_dsp_cdprod_typ2_f32(inputA, inputB, numSample, &output[2 * i], &output[2 * i + 1]);
    }
    dump2_f32(golden, output, 2*loop);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);
    float32_t NRMSD;
    NRMSD = dump_rmsd(golden, output, 2 * loop);

    if (NRMSD <= 0.0000002)
        return 0;
    else
        return -1;
}
