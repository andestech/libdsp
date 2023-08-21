#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRIx32
#include "riscv_dsp_basic_math.h"
#include "riscv_dsp_test_accuracy.h"

#define BLOCKSIZE 16383
#define TOTALOUTPUT (2 * BLOCKSIZE)

int main(void)
{
    uint32_t i;
    uint32_t blocksize = BLOCKSIZE;
    q31_t inputvec[blocksize], goldenvec[TOTALOUTPUT];
    q31_t output1[blocksize], output2[blocksize];
    int8_t shift1 = -7;
    int8_t shift2 = 7;

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q31.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_shift_q31.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(inputvec, sizeof(q31_t), blocksize, fptr);
    dump_q31("randomly generated input array", inputvec, blocksize);
    test_Fread(goldenvec, sizeof(q31_t), TOTALOUTPUT, gptr);

    // test1
    riscv_dsp_shift_q31(inputvec, shift1, output1, blocksize);

    // test2
    riscv_dsp_shift_q31(inputvec, shift2, output2, blocksize);

    q31_t tmpoutput[TOTALOUTPUT];
    for (i = 0; i < blocksize; i++)
    {
        tmpoutput[i] = output1[i];
        tmpoutput[i + blocksize] = output2[i];
    }

    dump2_q31(goldenvec, tmpoutput, TOTALOUTPUT);

    fclose(fptr);
    fclose(gptr);
    float32_t NRMSD;
    float32_t golden_f32[TOTALOUTPUT], output_f32[TOTALOUTPUT];
    riscv_dsp_convert_q31_f32(goldenvec, golden_f32, TOTALOUTPUT);
    riscv_dsp_convert_q31_f32(tmpoutput, output_f32, TOTALOUTPUT);

    NRMSD = dump_rmsd(golden_f32, output_f32, TOTALOUTPUT);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
