#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_statistics_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t i;
    int datasize = 1100;
    uint32_t blocksize = 11;
    uint32_t loop = datasize / blocksize;
    uint8_t input[blocksize] __attribute__ ((aligned(4)));
    uint8_t out_min[loop], gold_min[loop];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_u8_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_min_u8_min.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input, sizeof(uint8_t), blocksize, fptr);
        out_min[i] = riscv_dsp_min_val_u8(input, 0);
        out_min[i] = riscv_dsp_min_val_u8(input, blocksize);
    }
    test_Fread(&gold_min, sizeof(uint8_t), loop, gptr);
    dump2_u8(gold_min, out_min, loop);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[loop], output_f32[loop];
    _dsp_convert_u8_f32(gold_min, golden_f32, loop);
    _dsp_convert_u8_f32(out_min, output_f32, loop);

    NRMSD = dump_rmsd(golden_f32, output_f32, loop);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
