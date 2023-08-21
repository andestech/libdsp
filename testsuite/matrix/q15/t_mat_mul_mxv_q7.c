#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_matrix_math.h"

#define MATRIX_M 49
#define MATRIX_N 37
#define MATRIX_P 1

#define MATRIX_1 1
#define MATRIX_2 2

int main(void)
{
    uint16_t asamp = MATRIX_M * MATRIX_N;
    uint16_t bsamp = MATRIX_N * MATRIX_P;
    uint16_t osamp = MATRIX_M * MATRIX_P;

    q7_t pInA[asamp] __attribute__ ((aligned(4)));
    q7_t pInB[bsamp] __attribute__ ((aligned(4)));
    q7_t pGol[osamp];
    q7_t output[osamp] __attribute__ ((aligned(4)));

    FILE *fptr;
    FILE *gptr;
    FILE *kptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q7_little_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_q7_little_b.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mat_mul_mxv_q7.bin", srcdir);
    kptr = fopen(path_buffer, "r");
    if (kptr == NULL)
        return -1;

    test_Fread(&pInA, sizeof(q7_t), asamp, fptr);
    test_Fread(&pInB, sizeof(q7_t), bsamp, gptr);
    test_Fread(&pGol, sizeof(q7_t), osamp, kptr);

    riscv_dsp_mat_mul_mxv_q7(pInA, pInB, output, MATRIX_M, MATRIX_N);
    dump2_q7(pGol, output, osamp);

    fclose(fptr);
    fclose(gptr);
    fclose(kptr);

    float32_t NRMSD;
    float32_t golden_f32[osamp], output_f32[osamp];
    riscv_dsp_convert_q7_f32(pGol, golden_f32, osamp);
    riscv_dsp_convert_q7_f32(output, output_f32, osamp);

    NRMSD = dump_rmsd(golden_f32, output_f32, osamp);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
