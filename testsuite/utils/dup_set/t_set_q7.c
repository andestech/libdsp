#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_utils_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    int i;
    int datasize = 1000;
    uint32_t blocksize = 4;

    FILE *fptr, *gptr;
    q7_t input;
    q7_t golden[blocksize * datasize], out[blocksize * datasize];
    q7_t *tmpout = out;

    const char *srcdir = GETENV_SRCDIR;
    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q7_1.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    sprintf (path_buffer, "%s/golden_set_q7.bin", srcdir);
    gptr = fopen(path_buffer, "r");

    for (i = 0; i < datasize; i++)
    {
        test_Fread(&input, sizeof(q7_t), 1, fptr);
        riscv_dsp_set_q7(input, tmpout, blocksize);
        tmpout += blocksize;
    }
    test_Fread(&golden, sizeof(q7_t), blocksize * datasize, gptr);
    dump2_q7(golden, out, blocksize * datasize);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[blocksize * datasize], output_f32[blocksize * datasize];
    riscv_dsp_convert_q7_f32(golden, golden_f32, blocksize * datasize);
    riscv_dsp_convert_q7_f32(out, output_f32, blocksize * datasize);

    NRMSD = dump_rmsd(golden_f32, output_f32, blocksize * datasize);

    if (NRMSD <= 0.003)
        return 0;
    else
        return -1;
}
