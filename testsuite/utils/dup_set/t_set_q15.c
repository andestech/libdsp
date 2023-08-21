#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_utils_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    int i;
    int datasize = 100;
    uint32_t blocksize = 34;

    FILE *fptr, *gptr;
    q15_t input;
    q15_t golden[blocksize * datasize] __attribute__ ((aligned(8)));
    q15_t out[blocksize * datasize] __attribute__ ((aligned(8)));
    q15_t *tmpout = out;

    const char *srcdir = GETENV_SRCDIR;
    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q15_1.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    sprintf (path_buffer, "%s/golden_set_q15.bin", srcdir);
    gptr = fopen(path_buffer, "r");

    for (i = 0; i < datasize; i++)
    {
        test_Fread(&input, sizeof(q15_t), 1, fptr);
        riscv_dsp_set_q15(input, tmpout, blocksize);
        tmpout += blocksize;
    }
    test_Fread(&golden, sizeof(q15_t), blocksize * datasize, gptr);
    dump2_q15(golden, out, blocksize * datasize);
    q15_t a = 0x300, b;
    riscv_dsp_set_q15(a, &b, 1);
    if (b != 0x300) return -1;

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[blocksize * datasize], output_f32[blocksize * datasize];
    riscv_dsp_convert_q15_f32(golden, golden_f32, blocksize * datasize);
    riscv_dsp_convert_q15_f32(out, output_f32, blocksize * datasize);

    NRMSD = dump_rmsd(golden_f32, output_f32, blocksize * datasize);

    if (NRMSD <= 0.000011)
        return 0;
    else
        return -1;
}
