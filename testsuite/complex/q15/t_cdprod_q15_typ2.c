#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_complex_math.h"

double dec2float(q15_t in);
int main(void)
{
    uint32_t datasize = 12700;
    uint32_t numsample = 127;
    uint32_t loop = datasize / numsample, i;
    short inputAvec[2*numsample] __attribute__ ((aligned(4)));
    short inputBvec[2*numsample] __attribute__ ((aligned(4)));
    q31_t outputvec[2*loop] __attribute__ ((aligned(4)));
    q31_t goldenvec[2*loop];

    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_cmc_q15_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_cmc_q15_b.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_cdprod_typ2_q15.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&goldenvec, sizeof(q31_t), 2*loop, gptr);

    for (i = 0; i < loop; i++)
    {
        test_Fread(&inputAvec, sizeof(q15_t), 2*numsample, fptr);
        test_Fread(&inputBvec, sizeof(q15_t), 2*numsample, f2ptr);
        riscv_dsp_cdprod_typ2_q15(inputAvec, inputBvec, numsample, &outputvec[2*i], &outputvec[2*i+1]);
    }
    dump2_q31(goldenvec, outputvec, 2*loop);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[2 * loop], output_f32[2 * loop];
    riscv_dsp_convert_q31_f32(goldenvec, golden_f32, 2 * loop);
    riscv_dsp_convert_q31_f32(outputvec, output_f32, 2 * loop);

    NRMSD = dump_rmsd(golden_f32, output_f32, 2 * loop);

    if (NRMSD <= 0.000000003)
        return 0;
    else
        return -1;
}
