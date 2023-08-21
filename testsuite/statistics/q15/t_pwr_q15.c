#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_statistics_math.h"

int main(void)
{
    uint32_t i;
    int datasize = 30000;
    uint32_t blocksize = 15;
    uint32_t loop = datasize / blocksize;
    q15_t input[blocksize] __attribute__ ((aligned(4)));
    q63_t outputda[loop], golden[loop];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_sat_q15.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_power_q15.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input, sizeof(q15_t), blocksize, fptr);
        outputda[i] = riscv_dsp_pwr_q15(input, blocksize);
    }
    test_Fread(&golden, sizeof(q63_t), loop, gptr);
    dump2_q63(golden, outputda, loop);

    fclose(fptr);
    fclose(gptr);


    float32_t NRMSD;
    double golden_f32[loop], output_f32[loop];
    _dsp_convert_q63_f64(golden, golden_f32, loop);
    _dsp_convert_q63_f64(outputda, output_f32, loop);

    NRMSD = dump_rmsd64(golden_f32, output_f32, loop);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
