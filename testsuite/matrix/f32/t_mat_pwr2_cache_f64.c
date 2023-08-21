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
    uint32_t osamp = MATRIX_M * MATRIX_P;

    float64_t pInA[asamp];
    float64_t output[osamp];
    float64_t golden[osamp];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f64_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mat_pow2_cache_f64.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&pInA, sizeof(float64_t), asamp, fptr);
    test_Fread(&golden, sizeof(float64_t), osamp, gptr);

    riscv_dsp_mat_pwr2_cache_f64(pInA, output, MATRIX_M);
    dump2_f64(golden, output, osamp);

    int32_t tmpret;
    float64_t *tmpin = &pInA[0];
    float64_t tmpout[63 * 63];
    tmpret = riscv_dsp_mat_pwr2_cache_f64(tmpin, tmpout, 63);
    if (tmpret != -1) return -1;

    fclose(fptr);
    fclose(gptr);

    float64_t NRMSD;
    NRMSD = dump_rmsd64(golden, output, osamp);

    if (NRMSD <= 0.00000012)
        return 0;
    else
        return -1;
}
