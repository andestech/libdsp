#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_utils_math.h"
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    int i, loop = 32768;
    FILE *fptr;
    FILE *gptr;
    q31_t output[loop], input, golden[loop];

    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_sqrt_q31.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_sqrt_q31.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < 32768; i++)
    {
        test_Fread(&input, sizeof(q31_t), 1, fptr);
        output[i] = riscv_dsp_sqrt_q31(input);
    }
    test_Fread(&golden, sizeof(q31_t), loop, gptr);
    dump2_q31(golden, output, loop);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[loop], output_f32[loop];
    riscv_dsp_convert_q31_f32(golden, golden_f32, loop);
    riscv_dsp_convert_q31_f32(output, output_f32, loop);

    NRMSD = dump_rmsd(golden_f32, output_f32, loop);

    if (NRMSD <= 0.00000002)
        return 0;
    else
        return -1;
}
