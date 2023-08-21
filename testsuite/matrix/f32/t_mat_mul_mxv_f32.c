#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_matrix_math.h"

#define MATRIX_M 99
#define MATRIX_N 87
#define MATRIX_P 1


int main(void)
{
    uint32_t asamp = MATRIX_M * MATRIX_N;
    uint32_t bsamp = MATRIX_N * MATRIX_P;
    uint32_t osamp = MATRIX_M * MATRIX_P;

    float32_t pInA[asamp];
    float32_t pInB[bsamp];
    float32_t output[osamp];
    float32_t golden[osamp];

    FILE *fptr;
    FILE *gptr;
    FILE *f2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32_testa.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_f32_test.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (f2ptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mat_mul_mxv_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&pInA, sizeof(float32_t), asamp, fptr);
    test_Fread(&pInB, sizeof(float32_t), bsamp, f2ptr);
    test_Fread(&golden, sizeof(float32_t), osamp, gptr);

    riscv_dsp_mat_mul_mxv_f32(pInA, pInB, output, MATRIX_M, MATRIX_N);
    dump2_f32(golden, output, osamp);

    fclose(fptr);
    fclose(gptr);
    fclose(f2ptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden, output, osamp);

    if (NRMSD <= 0.0000002)
        return 0;
    else
        return -1;
}
