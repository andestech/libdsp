#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_basic_math.h"

double dec2float(q7_t in);
int main(void)
{
    uint32_t blocksize = 255;
    uint8_t inputAvec[blocksize] __attribute__ ((aligned(4)));
    uint8_t inputBvec[blocksize] __attribute__ ((aligned(4)));
    uint16_t outputvec[blocksize] __attribute__ ((aligned(4)));
    uint16_t goldenvec[blocksize];

    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_u8_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_u8_b.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mult_u8.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&inputAvec, sizeof(uint8_t), blocksize, fptr);
    test_Fread(&inputBvec, sizeof(uint8_t), blocksize, f2ptr);
    test_Fread(&goldenvec, sizeof(uint16_t), blocksize, gptr);

    riscv_dsp_mul_u8_u16(inputAvec, inputBvec, outputvec, blocksize);
    dump2_u16(goldenvec, outputvec, blocksize);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[blocksize], output_f32[blocksize];
    _dsp_convert_u16_f32(goldenvec, golden_f32, blocksize);
    _dsp_convert_u16_f32(outputvec, output_f32, blocksize);

    NRMSD = dump_rmsd(golden_f32, output_f32, blocksize);

    if (NRMSD <= 0.0007)
        return 0;
    else
        return -1;
}