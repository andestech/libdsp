#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_basic_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t blocksize = 255;
    int8_t shiftval = 5;
    uint8_t inputvec[blocksize] __attribute__ ((aligned(4)));
    uint8_t outputvec[blocksize] __attribute__ ((aligned(4)));
    q7_t scaleval = 0x8;
    uint8_t goldenvec[blocksize];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_u8_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;
    sprintf (path_buffer, "%s/golden_scale_u8.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&inputvec, sizeof(uint8_t), blocksize, fptr);
    test_Fread(&goldenvec, sizeof(uint8_t), blocksize, gptr);
    fclose(fptr);
    fclose(gptr);

    riscv_dsp_scale_u8(inputvec, scaleval, shiftval, outputvec, blocksize);
    dump2_u8(goldenvec, outputvec, blocksize);

    float32_t NRMSD = 0;
    float32_t golden_f32[blocksize], output_f32[blocksize];
    _dsp_convert_u8_f32(goldenvec, golden_f32, blocksize);
    _dsp_convert_u8_f32(outputvec, output_f32, blocksize);

    NRMSD = dump_rmsd(golden_f32, output_f32, blocksize);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
