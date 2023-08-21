#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_utils_math.h"
#include "riscv_dsp_complex_math.h"

int main(void)
{
    int32_t numSample = 599;
    float32_t inputvec[2 * numSample];
    float32_t goldenvec[numSample], outputvec[numSample];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_mag_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mag_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&inputvec, sizeof(float32_t), 2 * numSample, fptr);
    test_Fread(&goldenvec, sizeof(float32_t), numSample, gptr);

    q15_t q15_in[2 * numSample];
    q15_t q15_out[numSample];
    riscv_dsp_convert_f32_q15(inputvec, q15_in, 2 * numSample);
    riscv_dsp_cmag_hp_q15(q15_in, q15_out, numSample);

    for(int k = 0; k < numSample; k++)
    {
        goldenvec[k] = goldenvec[k] / 2.0f;
    }
    riscv_dsp_convert_q15_f32(q15_out, outputvec, numSample);
    dump2_f32(goldenvec, outputvec, numSample);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(goldenvec, outputvec, numSample);

    if (NRMSD <= 0.00004)
        return 0;
    else
        return -1;
}
