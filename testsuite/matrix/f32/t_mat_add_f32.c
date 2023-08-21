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

    float32_t pInA[numsamp];
    float32_t pInB[numsamp];
    float32_t output[numsamp];
    float32_t golden[numsamp];

    FILE *fptr;
    FILE *gptr;
    FILE *f2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_f32_b.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (f2ptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mat_add_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&pInA, sizeof(float32_t), numsamp, fptr);
    test_Fread(&pInB, sizeof(float32_t), numsamp, f2ptr);
    test_Fread(&golden, sizeof(float32_t), numsamp, gptr);

    riscv_dsp_mat_add_f32(pInA, pInB, output, MATRIX_ROW, MATRIX_COL);
    dump2_f32(golden, output, numsamp);

    fclose(fptr);
    fclose(gptr);
    fclose(f2ptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden, output, numsamp);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
