#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_matrix_math.h"

#define MATRIX_ROW 100
#define MATRIX_COL 100


int main(void)
{
    uint32_t numsamp = MATRIX_ROW * MATRIX_COL;

    float64_t pIn[numsamp];
    float64_t output[numsamp];
    float64_t golden[numsamp];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f64_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mat_trans_f64.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&pIn, sizeof(float64_t), numsamp, fptr);
    test_Fread(&golden, sizeof(float64_t), numsamp, gptr);

    riscv_dsp_mat_trans_f64(pIn, output, MATRIX_ROW, MATRIX_COL);
    dump2_f64(golden, output, numsamp);

    fclose(fptr);
    fclose(gptr);

    float64_t NRMSD;
    NRMSD = dump_rmsd64(golden, output, numsamp);

    if (NRMSD <= 0.00000012)
        return 0;
    else
        return -1;
}
