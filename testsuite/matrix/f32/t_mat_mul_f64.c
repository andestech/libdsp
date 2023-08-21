#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_matrix_math.h"

#define MATRIX_M 100
#define MATRIX_N 100
#define MATRIX_P 100


int main(void)
{
    uint32_t asamp = MATRIX_M * MATRIX_N;
    uint32_t bsamp = MATRIX_N * MATRIX_P;
    uint32_t osamp = MATRIX_M * MATRIX_P;

    float64_t pInA[asamp];
    float64_t pInB[bsamp];
    float64_t output[osamp];
    float64_t golden[osamp];

    FILE *fptr;
    FILE *gptr;
    FILE *f2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f64_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_f64_b.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (f2ptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mat_mult_f64.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&pInA, sizeof(float64_t), asamp, fptr);
    test_Fread(&pInB, sizeof(float64_t), bsamp, f2ptr);
    test_Fread(&golden, sizeof(float64_t), osamp, gptr);

    riscv_dsp_mat_mul_f64(pInA, pInB, output, MATRIX_M, MATRIX_N, MATRIX_P);
    dump2_f64(golden, output, osamp);

    fclose(fptr);
    fclose(gptr);
    fclose(f2ptr);

    float64_t NRMSD;
    NRMSD = dump_rmsd64(golden, output, osamp);

    if (NRMSD <= 0.00000012)
        return 0;
    else
        return -1;
}
