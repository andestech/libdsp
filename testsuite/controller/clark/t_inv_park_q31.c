#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_math_types.h"
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_controller_math.h"


int main(void)
{
    int i, loop = 20000;
    FILE *f1ptr, *f2ptr, *g1ptr, *g2ptr;
    FILE *f3ptr, *f4ptr;
    q31_t inputa, inputb;
    q31_t outputa[loop], outputb[loop], goldena[loop], goldenb[loop];
    q31_t cosval, sinval;
    const char *srcdir = GETENV_SRCDIR;
    q31_t * tmpouta = outputa;
    q31_t * tmpoutb = outputb;
    float32_t result1[4], result2[4];

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q31_a.bin", srcdir);
    g1ptr = fopen(path_buffer, "r");
    sprintf (path_buffer, "%s/input_q31_b.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    sprintf (path_buffer, "%s/cosval_q31.bin", srcdir);
    f3ptr = fopen(path_buffer, "r");
    sprintf (path_buffer, "%s/sinval_q31.bin", srcdir);
    f4ptr = fopen(path_buffer, "r");
    sprintf (path_buffer, "%s/golden_park_q31_a.bin", srcdir);
    f1ptr = fopen(path_buffer, "r");
    sprintf (path_buffer, "%s/golden_park_q31_b.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");

    for (i = 0; i < 20000; i++)
    {
        test_Fread(&inputa, sizeof(q31_t), 1, f1ptr);
        test_Fread(&inputb, sizeof(q31_t), 1, f2ptr);
        test_Fread(&cosval, sizeof(q31_t), 1, f3ptr);
        test_Fread(&sinval, sizeof(q31_t), 1, f4ptr);
        riscv_dsp_inv_park_q31(inputa, inputb, tmpouta, tmpoutb, sinval, cosval);
        tmpouta++;
        tmpoutb++;
    }
    q31_t a[2], b[2];
    riscv_dsp_inv_park_q31(0x7fff0000, 0x7fff0000, &a[0], &b[0], 0x7fff0000, 0x7fff0000);
    riscv_dsp_inv_park_q31(0x7fff0000, 0x8fff0000, &a[1], &b[1], 0x7fff0000, 0x7fff0000);
    if ((a[0] != 0) || (b[0] != 0x7fffffff) || (a[1] != 0x7fffffff) || b[1] != 0x0ffde004)
    {
        printf("OV case FAIL\n");
        return -1;
    }
    test_Fread(&goldena, sizeof(q31_t), loop, g1ptr);
    test_Fread(&goldenb, sizeof(q31_t), loop, g2ptr);
    float NRMSD1, NRMSD2;
    float32_t goldena_f32[loop], outputa_f32[loop];
    float32_t goldenb_f32[loop], outputb_f32[loop];
    riscv_dsp_convert_q31_f32(goldena, goldena_f32, loop);
    riscv_dsp_convert_q31_f32(goldenb, goldenb_f32, loop);
    riscv_dsp_convert_q31_f32(outputa, outputa_f32, loop);
    riscv_dsp_convert_q31_f32(outputb, outputb_f32, loop);
    NRMSD1 = dump_rmsd2(goldena_f32, outputa_f32, loop, result1);
    NRMSD2 = dump_rmsd2(goldenb_f32, outputb_f32, loop, result2);


    NRMSD1 = (NRMSD1 + NRMSD2) / 2;
    result1[0] = (result1[0] + result2[0]) / 2;
    result1[1] = (result1[1] + result2[1]) / 2;
    result1[2] = (result1[2] + result2[2]) / 2;
    result1[3] = (result1[3] + result2[3]) / 2;

    printf("--------------------\n");
    printf("MAE is %5.8f, RMSD is %5.8f, NRMSD is %5.8f, MAXDIFF is %5.8f, SNR is %5.8f\n", result1[0], result1[1], NRMSD1, result1[2], result1[3]);

    fclose(f1ptr);
    fclose(f2ptr);
    fclose(g1ptr);
    fclose(g2ptr);

    if (NRMSD1 <= 0.00000002)
        return 0;
    else
        return -1;
}
