#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_utils_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t blocksize = 19999;

    FILE *fptr, *gptr;
    float input[blocksize];
    q15_t golden[blocksize], out[blocksize];

    const char *srcdir = GETENV_SRCDIR;
    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    sprintf (path_buffer, "%s/golden_f_to_q15.bin", srcdir);
    gptr = fopen(path_buffer, "r");

    test_Fread(&input, sizeof(float), blocksize, fptr);
    test_Fread(&golden, sizeof(q15_t), blocksize, gptr);
    riscv_dsp_convert_f32_q15(input, out, blocksize);
    dump2_q15(golden, out, blocksize);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[blocksize], output_f32[blocksize];
    riscv_dsp_convert_q15_f32(golden, golden_f32, blocksize);
    riscv_dsp_convert_q15_f32(out, output_f32, blocksize);

    NRMSD = dump_rmsd(golden_f32, output_f32, blocksize);

    // for coverage remainder loop
    float a[2] = { -0.5, 0.5};
    q15_t b[2];
    riscv_dsp_convert_f32_q15(a, b, 2);

    if (NRMSD <= 0.000011)
        return 0;
    else
        return -1;
}
