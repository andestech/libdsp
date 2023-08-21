#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_matrix_math.h"
#include "riscv_dsp_test_accuracy.h"

#define MATRIX_M 18
#define MATRIX_N 38
#define MATRIX_P 21

int main(void)
{
    uint16_t asamp = MATRIX_M * MATRIX_N * 2;
    uint16_t bsamp = MATRIX_N * MATRIX_P * 2;
    uint16_t osamp = MATRIX_M * MATRIX_P * 2;

    q31_t pInA[asamp];
    q31_t pInB[bsamp];
    q31_t output[osamp];
    q31_t pGol[osamp];

    FILE *fptr;
    FILE *gptr;
    FILE *kptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q31_a_1.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_q31_b_1.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_cmat_mul_q31.bin", srcdir);
    kptr = fopen(path_buffer, "r");
    if (kptr == NULL)
        return -1;

    test_Fread(&pInA, sizeof(q31_t), asamp, fptr);
    test_Fread(&pInB, sizeof(q31_t), bsamp, gptr);
    test_Fread(&pGol, sizeof(q31_t), osamp, kptr);

    riscv_dsp_cmat_mul_q31(pInA, pInB, output, MATRIX_M, MATRIX_N, MATRIX_P);
    dump2_q31(pGol, output, osamp);

    fclose(fptr);
    fclose(gptr);
    fclose(kptr);

    float32_t NRMSD;
    float32_t golden_f32[osamp], output_f32[osamp];
    riscv_dsp_convert_q31_f32(pGol, golden_f32, osamp);
    riscv_dsp_convert_q31_f32(output, output_f32, osamp);

    NRMSD = dump_rmsd(golden_f32, output_f32, osamp);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
