#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_utils_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t blocksize = 29999;

    FILE *fptr, *gptr;
    q31_t input[blocksize];
    q7_t golden[blocksize], out[blocksize];

    const char *srcdir = GETENV_SRCDIR;
    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q31.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    sprintf (path_buffer, "%s/golden_q31_to_q7.bin", srcdir);
    gptr = fopen(path_buffer, "r");

    test_Fread(&input, sizeof(q31_t), blocksize, fptr);
    test_Fread(&golden, sizeof(q7_t), blocksize, gptr);
    riscv_dsp_convert_q31_q7(input, out, blocksize);
    dump2_q7(golden, out, blocksize);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[blocksize], output_f32[blocksize];
    riscv_dsp_convert_q7_f32(golden, golden_f32, blocksize);
    riscv_dsp_convert_q7_f32(out, output_f32, blocksize);

    NRMSD = dump_rmsd(golden_f32, output_f32, blocksize);

    if (NRMSD <= 0.003)
        return 0;
    else
        return -1;
}
