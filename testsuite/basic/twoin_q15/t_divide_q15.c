#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "riscv_dsp_basic_math.h"
#include <inttypes.h>       //for PRIx32
#include "riscv_dsp_test_accuracy.h"


int main(void)
{
    uint32_t i;
    uint32_t blocksize = 799;
    float a_f32[blocksize];
    float b_f32[blocksize];

    float out_f32[blocksize];
    float golden_f32[blocksize];
    q15_t tmpout;
    int16_t tmp_shift;

    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32_2_rand_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_f32_2_rand_b.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_div_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&a_f32, sizeof(float), blocksize, fptr);
    test_Fread(&b_f32, sizeof(float), blocksize, f2ptr);
    test_Fread(&golden_f32, sizeof(float), blocksize, gptr);

    for (i = 0; i < blocksize; i++)
    {
        float tmpa = a_f32[i];
        float tmpb = b_f32[i];
        q15_t ina_q15 = convert_float_to_q15(tmpa);
        q15_t inb_q15 = convert_float_to_q15(tmpb);

        riscv_dsp_divide_q15(ina_q15, inb_q15, &tmpout, &tmp_shift);

        float tmpout_f32 = convert_q15_to_float(tmpout);
        out_f32[i] = (float) (tmpout_f32 * (1u << tmp_shift));
        //LOG("out_f32: %f, quo: %d(%f), sh: %d\n", out_f32[i], (int)tmpout, tmpout_f32, tmp_shift);
    }

    // coverage for denominator = 0
    riscv_dsp_divide_q15(0x3000, 0, &tmpout, &tmp_shift);
    riscv_dsp_divide_q15(0xa000, 0, &tmpout, &tmp_shift);
    // end coverage for denominator = 0

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);
    dump2_f32(golden_f32, out_f32, blocksize);
    float NRMSD = dump_rmsd(golden_f32, out_f32, blocksize);

    if (NRMSD <= 0.0003)
        return 0;
    else
        return -1;
}
