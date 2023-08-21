#include <stdio.h>
#include <math.h>
#include "riscv_dsp_basic_math.h"
#include "riscv_dsp_test_accuracy.h"

#define BLOCKSIZE 255
#define TOTALOUTPUT (2 * BLOCKSIZE)

int main(void)
{
    uint32_t i;
    uint32_t blocksize = BLOCKSIZE;
    uint8_t inputvec[blocksize] __attribute__ ((aligned(4)));
    uint8_t output1[blocksize] __attribute__ ((aligned(4)));
    uint8_t output2[blocksize] __attribute__ ((aligned(4)));
    uint8_t goldenvec[TOTALOUTPUT];
    int8_t shift1 = -3;
    int8_t shift2 = 3;

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_u8_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_shift_u8.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&inputvec, sizeof(uint8_t), blocksize, fptr);
    test_Fread(&goldenvec, sizeof(uint8_t), TOTALOUTPUT, gptr);
    fclose(fptr);
    fclose(gptr);

    riscv_dsp_shift_u8(inputvec, shift1, output1, blocksize);
    riscv_dsp_shift_u8(inputvec, shift2, output2, blocksize);

    uint8_t tmpoutput[TOTALOUTPUT];
    for (i = 0; i < blocksize; i++)
    {
        tmpoutput[i] = output1[i];
        tmpoutput[i + blocksize] = output2[i];
    }

    dump2_u8(goldenvec, tmpoutput, TOTALOUTPUT);

    float32_t NRMSD;
    float32_t golden_f32[TOTALOUTPUT], output_f32[TOTALOUTPUT];
    _dsp_convert_u8_f32(goldenvec, golden_f32, TOTALOUTPUT);
    _dsp_convert_u8_f32(tmpoutput, output_f32, TOTALOUTPUT);

    NRMSD = dump_rmsd(golden_f32, output_f32, TOTALOUTPUT);

    if (NRMSD <= 0)
        return 0;
    else
        return -1;
}
