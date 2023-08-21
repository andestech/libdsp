#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_basic_math.h"

int main(void)
{
    uint32_t blocksize = 255;
    uint8_t inputAvec[blocksize] __attribute__ ((aligned(4)));
    uint8_t inputBvec[blocksize] __attribute__ ((aligned(4)));
    uint16_t outputvec[blocksize] __attribute__ ((aligned(4)));
    uint16_t goldenvec[blocksize];

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

    sprintf (path_buffer, "%s/golden_add_u8.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&inputAvec, sizeof(uint8_t), blocksize, fptr);
    test_Fread(&inputBvec, sizeof(uint8_t), blocksize, f2ptr);
    test_Fread(&goldenvec, sizeof(uint16_t), blocksize, gptr);

    riscv_dsp_add_u8_u16(inputAvec, inputBvec, outputvec, blocksize);
    dump2_u16(goldenvec, outputvec, blocksize);
#if 0
    uint8_t a[3] = {0xff, 0x3f, 0x02};
    uint8_t b[3] = {0x2f, 0x11, 0x02};
    uint16_t c[3];
    uint16_t g[3] = {0x12e, 0x50, 0x04};
    riscv_dsp_add_u8_u16(a, b, c, 3);
    dump2_u16(g, c, 3);
#endif

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[blocksize], output_f32[blocksize];
    _dsp_convert_u16_f32(goldenvec, golden_f32, blocksize);
    _dsp_convert_u16_f32(outputvec, output_f32, blocksize);

    NRMSD = dump_rmsd(golden_f32, output_f32, blocksize);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
