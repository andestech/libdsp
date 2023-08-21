#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_utils_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    int i;
    int datasize = 10;
    uint32_t blocksize = 35;

    FILE *fptr, *gptr;
    q31_t input;
    q31_t golden[blocksize * datasize], out[blocksize * datasize];
    q31_t *tmpout = out;

    const char *srcdir = GETENV_SRCDIR;
    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q31_1.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    sprintf (path_buffer, "%s/golden_set_q31.bin", srcdir);
    gptr = fopen(path_buffer, "r");

    for (i = 0; i < datasize; i++)
    {
        test_Fread(&input, sizeof(q31_t), 1, fptr);
        riscv_dsp_set_q31(input, tmpout, blocksize);
        tmpout += blocksize;
    }
    test_Fread(&golden, sizeof(q31_t), blocksize * datasize, gptr);
    dump2_q31(golden, out, blocksize * datasize);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[blocksize * datasize], output_f32[blocksize * datasize];
    riscv_dsp_convert_q31_f32(golden, golden_f32, blocksize * datasize);
    riscv_dsp_convert_q31_f32(out, output_f32, blocksize * datasize);

    NRMSD = dump_rmsd(golden_f32, output_f32, blocksize * datasize);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
