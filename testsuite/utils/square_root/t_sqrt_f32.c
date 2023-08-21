#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_utils_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    int i, loop = 32768;
    FILE *fptr;
    FILE *gptr;
    float32_t output[loop], input, golden[loop];

    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_sqrt_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_sqrt_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < 32768; i++)
    {
        test_Fread(&input, sizeof(float32_t), 1, fptr);
        output[i] = riscv_dsp_sqrt_f32(input);
    }
    test_Fread(&golden, sizeof(float32_t), loop, gptr);
    dump2_f32(golden, output, loop);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden, output, loop);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
