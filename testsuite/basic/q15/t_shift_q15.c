#include <stdio.h>
#include <math.h>
#include "riscv_dsp_basic_math.h"
#include "riscv_dsp_test_accuracy.h"

#define BLOCKSIZE 32761
#define TOTALOUTPUT (2 * BLOCKSIZE)

int main(void)
{
    uint32_t i;
    const uint32_t blocksize = BLOCKSIZE;
    q15_t inputvec[blocksize] __attribute__ ((aligned(4)));
    q15_t output1[blocksize] __attribute__ ((aligned(4)));
    q15_t output2[blocksize] __attribute__ ((aligned(4)));
    q15_t goldenvec[TOTALOUTPUT];


    int8_t shift1 = -3;
    int8_t shift2 = 3;

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q15-1.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_shift_q15.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(inputvec, sizeof(q15_t), blocksize, fptr);
    test_Fread(goldenvec, sizeof(q15_t), TOTALOUTPUT, gptr);

    // test1
    riscv_dsp_shift_q15(inputvec, shift1, output1, blocksize);

    // test2
    riscv_dsp_shift_q15(inputvec, shift2, output2, blocksize);

    q15_t tmpoutput[TOTALOUTPUT];
    for (i = 0; i < blocksize; i++)
    {
        tmpoutput[i] = output1[i];
        tmpoutput[i + blocksize] = output2[i];
    }
    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[TOTALOUTPUT], output_f32[TOTALOUTPUT];
    riscv_dsp_convert_q15_f32(goldenvec, golden_f32, TOTALOUTPUT);
    riscv_dsp_convert_q15_f32(tmpoutput, output_f32, TOTALOUTPUT);

    NRMSD = dump_rmsd(golden_f32, output_f32, TOTALOUTPUT);

    if (NRMSD <= 0.00009)
        return 0;
    else
        return -1;
}
