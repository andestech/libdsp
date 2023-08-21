#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_statistics_math.h"

int main(void)
{
    uint32_t i;
    int datasize = 350;
    uint32_t blocksize = 35;
    uint32_t loop = datasize / blocksize;
    uint8_t input[blocksize] __attribute__ ((aligned(4)));
    uint8_t outputda[loop], golden[loop];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_u8_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mean_u8.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input, sizeof(uint8_t), blocksize, fptr);
        outputda[i] = riscv_dsp_mean_u8(input, blocksize);
    }
    test_Fread(&golden, sizeof(uint8_t), loop, gptr);
    dump2_u8(golden, outputda, loop);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[loop], output_f32[loop];
    _dsp_convert_u8_f32(golden, golden_f32, loop);
    _dsp_convert_u8_f32(outputda, output_f32, loop);

    NRMSD = dump_rmsd(golden_f32, output_f32, loop);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
