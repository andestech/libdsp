#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_matrix_math.h"

#define MATRIX_M 50
#define MATRIX_N 50
#define MATRIX_P 50

int main(void)
{
    uint16_t asamp = MATRIX_M * MATRIX_N * 2;
    uint16_t bsamp = MATRIX_N * MATRIX_P * 2;
    uint16_t osamp = MATRIX_M * MATRIX_P * 2;

    float32_t pInA[asamp];
    float32_t pInB[bsamp];
    float32_t pGol[osamp];
    float32_t output[osamp];

    FILE *fptr;
    FILE *gptr;
    FILE *kptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32_testa.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_f32_test.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_cmat_mul_f32.bin", srcdir);
    kptr = fopen(path_buffer, "r");
    if (kptr == NULL)
        return -1;

    test_Fread(&pInA, sizeof(float32_t), asamp, fptr);
    test_Fread(&pInB, sizeof(float32_t), bsamp, gptr);
    test_Fread(&pGol, sizeof(float32_t), osamp, kptr);

    dump_f32("A", pInA, asamp);
    dump_f32("B", pInB, bsamp);

    riscv_dsp_cmat_mul_f32(pInA, pInB, output, MATRIX_M, MATRIX_N, MATRIX_P);
    dump2_f32(pGol, output, osamp);

    fclose(fptr);
    fclose(gptr);
    fclose(kptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(pGol, output, osamp);

    if (NRMSD <= 0.00000012)
        return 0;
    else
        return -1;
}
