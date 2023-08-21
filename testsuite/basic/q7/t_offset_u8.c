#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_basic_math.h"

int main(void)
{
    uint32_t blocksize = 258;
    uint32_t i;
    q7_t offsetval = -20;
    q7_t offsetval2 = 30;
    q7_t offsetval3 = -128;
    uint8_t inputvec[blocksize] __attribute__ ((aligned(4)));
    uint8_t outputvec[blocksize] __attribute__ ((aligned(4)));
    uint8_t outputvec2[blocksize] __attribute__ ((aligned(4)));
    uint8_t outputvec3[blocksize] __attribute__ ((aligned(4)));
    uint8_t goldenvec[blocksize];
    uint8_t goldenvec2[blocksize];
    uint8_t goldenvec3[blocksize];



    FILE *fptr;
    FILE *gptr;
    FILE *g2ptr;
    FILE *g3ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_u8_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_offset_u8.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_offset_u8_2.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_offset_u8_3.bin", srcdir);
    g3ptr = fopen(path_buffer, "r");
    if (g3ptr == NULL)
        return -1;

    test_Fread(&inputvec, sizeof(uint8_t), blocksize, fptr);
    test_Fread(&goldenvec, sizeof(uint8_t), blocksize, gptr);
    test_Fread(&goldenvec2, sizeof(uint8_t), blocksize, g2ptr);
    test_Fread(&goldenvec3, sizeof(uint8_t), blocksize, g3ptr);

    riscv_dsp_offset_u8(inputvec, offsetval, outputvec, blocksize);
    riscv_dsp_offset_u8(inputvec, offsetval2, outputvec2, blocksize);
    riscv_dsp_offset_u8(inputvec, offsetval3, outputvec3, blocksize);

    uint8_t tmpout[3 * blocksize];
    uint8_t tmpgold[3 * blocksize];
    for (i = 0; i < blocksize; i++)
    {
        tmpout[i] = outputvec[i];
        tmpout[i + blocksize] = outputvec2[i];
        tmpout[i + 2 * blocksize] = outputvec3[i];
        tmpgold[i] = goldenvec[i];
        tmpgold[i + blocksize] = goldenvec2[i];
        tmpgold[i + 2 * blocksize] = goldenvec3[i];
    }
    //dump2_u8(goldenvec3, outputvec3, blocksize);
    //dump2_u8(goldenvec2, outputvec2, blocksize);
    dump2_u8(tmpgold, tmpout, 3 * blocksize);

    fclose(fptr);
    fclose(gptr);
    fclose(g2ptr);
    fclose(g3ptr);
    float32_t NRMSD;
    float32_t golden_f32[3 * blocksize], output_f32[3 * blocksize];
    _dsp_convert_u8_f32(tmpgold, golden_f32, 3 * blocksize);
    _dsp_convert_u8_f32(tmpout, output_f32, 3 * blocksize);

    NRMSD = dump_rmsd(golden_f32, output_f32, 3 * blocksize);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
