#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_basic_math.h"

int main(void)
{
    uint32_t blocksize = 255;
    uint8_t inputAvec[blocksize] __attribute__ ((aligned(4)));
    uint8_t inputBvec[blocksize] __attribute__ ((aligned(4)));
    q7_t outputvec[blocksize] __attribute__ ((aligned(4)));
    q7_t goldenvec[blocksize];

    FILE *fptr;
    FILE *gptr;
    FILE *f2ptr;
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

    sprintf (path_buffer, "%s/golden_sub_u8.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&inputAvec, sizeof(uint8_t), blocksize, fptr);
    test_Fread(&inputBvec, sizeof(uint8_t), blocksize, f2ptr);
    test_Fread(&goldenvec, sizeof(q7_t), blocksize, gptr);

    riscv_dsp_sub_u8_q7(inputAvec, inputBvec, outputvec, blocksize);
    dump2_q7(goldenvec, outputvec, blocksize);

    uint8_t a[6] = {0xff, 0x10, 0x10, 0xff, 0x10, 0x10};
    uint8_t b[6] = {0x2f, 0x20, 0xff, 0x2f, 0x20, 0xff};
    q7_t c[6];
    q7_t g[6] = {0x7f, 0xf0, 0x80, 0x7f, 0xf0, 0x80};
    riscv_dsp_sub_u8_q7(a, b, c, 6);
    dump2_q7(g, c, 6);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[blocksize], output_f32[blocksize];
    riscv_dsp_convert_q7_f32(goldenvec, golden_f32, blocksize);
    riscv_dsp_convert_q7_f32(outputvec, output_f32, blocksize);

    NRMSD = dump_rmsd(golden_f32, output_f32, blocksize);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
