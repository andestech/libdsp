#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_matrix_math.h"
#include "riscv_dsp_test_accuracy.h"

#define MATRIX_ROW 100
#define MATRIX_COL 100


int main(void)
{
    uint32_t numsamp = MATRIX_ROW * MATRIX_COL;

    q31_t pInA[numsamp];
    q31_t pInB[numsamp];
    q31_t output[numsamp];
    q31_t pGol[numsamp];

    FILE *fptr;
    FILE *gptr;
    FILE *kptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q31_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_q31_b.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mat_sub_q31.bin", srcdir);
    kptr = fopen(path_buffer, "r");
    if (kptr == NULL)
        return -1;

    test_Fread(pInA, sizeof(q31_t), numsamp, fptr);
    test_Fread(pInB, sizeof(q31_t), numsamp, gptr);
    test_Fread(pGol, sizeof(q31_t), numsamp, kptr);

    riscv_dsp_mat_sub_q31(pInA, pInB, output, MATRIX_ROW, MATRIX_COL);
    dump2_q31(pGol, output, numsamp);

    fclose(fptr);
    fclose(gptr);
    fclose(kptr);

    float32_t NRMSD;
    float32_t golden_f32[numsamp], output_f32[numsamp];
    riscv_dsp_convert_q31_f32(pGol, golden_f32, numsamp);
    riscv_dsp_convert_q31_f32(output, output_f32, numsamp);

    NRMSD = dump_rmsd(golden_f32, output_f32, numsamp);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
