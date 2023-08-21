#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRIx32
#include "riscv_dsp_basic_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t i;
    uint32_t blocksize = 35;
    uint32_t loop = 32760 / blocksize;
    q31_t inputAvec[blocksize], inputBvec[blocksize];
    q63_t output[loop], golden[loop];

    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_dot_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_dot_b.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_dprod_q31.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&inputAvec, sizeof(q31_t), blocksize, fptr);
        test_Fread(&inputBvec, sizeof(q31_t), blocksize, f2ptr);
        output[i] = riscv_dsp_dprod_q31(inputAvec, inputBvec, blocksize);
    }
    test_Fread(&golden, sizeof(q63_t), loop, gptr);
    dump2_q63(golden, output, loop);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);

    float32_t NRMSD;
    double golden_f32[loop], output_f32[loop];
    _dsp_convert_q63_f64(golden, golden_f32, loop);
    _dsp_convert_q63_f64(output, output_f32, loop);

    NRMSD = dump_rmsd64(golden_f32, output_f32, loop);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
