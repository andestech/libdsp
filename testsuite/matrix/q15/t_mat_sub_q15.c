#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_matrix_math.h"

#define MATRIX_ROW 100
#define MATRIX_COL 100


int main(void)
{
    uint32_t numsamp = MATRIX_ROW * MATRIX_COL;

    q15_t pInA[numsamp] __attribute__ ((aligned(4)));
    q15_t pInB[numsamp] __attribute__ ((aligned(4)));
    q15_t output[numsamp] __attribute__ ((aligned(4)));
    q15_t golden[numsamp];

    FILE *fptr;
    FILE *gptr;
    FILE *kptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q15_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_q15_b.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mat_sub_q15.bin", srcdir);
    kptr = fopen(path_buffer, "r");
    if (kptr == NULL)
        return -1;

    test_Fread(&pInA, sizeof(q15_t), numsamp, fptr);
    test_Fread(&pInB, sizeof(q15_t), numsamp, gptr);
    test_Fread(golden, sizeof(q15_t), numsamp, kptr);

    riscv_dsp_mat_sub_q15(pInA, pInB, output, MATRIX_ROW, MATRIX_COL);
    dump2_q15(golden, output, numsamp);

    fclose(fptr);
    fclose(gptr);
    fclose(kptr);

    float32_t NRMSD;
    float32_t golden_f32[numsamp], output_f32[numsamp];
    riscv_dsp_convert_q15_f32(golden, golden_f32, numsamp);
    riscv_dsp_convert_q15_f32(output, output_f32, numsamp);

    NRMSD = dump_rmsd(golden_f32, output_f32, numsamp);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
