#include <stdio.h>
#include <math.h>
#include "riscv_dsp_math_types.h"
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_controller_math.h"


int main(void)
{
    int i, loop = 1000;
    FILE *f1ptr, *f2ptr, *g1ptr, *g2ptr;
    float inputa, inputb;
    float outputa[loop], outputb[loop], goldena[loop], goldenb[loop];
    const char *srcdir = GETENV_SRCDIR;
    float32_t * tmpouta = outputa;
    float32_t * tmpoutb = outputb;
    float32_t result1[4], result2[4];

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32_a.bin", srcdir);
    f1ptr = fopen(path_buffer, "r");
    sprintf (path_buffer, "%s/input_f32_b.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    sprintf (path_buffer, "%s/golden_clarke_f32_a.bin", srcdir);
    g1ptr = fopen(path_buffer, "r");
    sprintf (path_buffer, "%s/golden_clarke_f32_b.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");

    for (i = 0; i < loop; i++)
    {
        test_Fread(&inputa, sizeof(float), 1, f1ptr);
        test_Fread(&inputb, sizeof(float), 1, f2ptr);
        riscv_dsp_clarke_f32(inputa, inputb, tmpouta, tmpoutb);
        tmpouta++;
        tmpoutb++;
    }
    test_Fread(&goldena, sizeof(float), loop, g1ptr);
    test_Fread(&goldenb, sizeof(float), loop, g2ptr);
    float NRMSD1, NRMSD2;
    NRMSD1 = dump_rmsd2(goldena, outputa, loop, result1);
    NRMSD2 = dump_rmsd2(goldenb, outputb, loop, result2);

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