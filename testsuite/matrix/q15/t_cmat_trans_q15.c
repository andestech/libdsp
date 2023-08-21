#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_matrix_math.h"

#define MATRIX_ROW 55
#define MATRIX_COL 43
#define CLX_DIM 2


int main(void)
{
    uint32_t numsamp = MATRIX_ROW * MATRIX_COL * CLX_DIM;

    q15_t pIn[numsamp] __attribute__ ((aligned(4)));
    q15_t output[numsamp] __attribute__ ((aligned(4)));
    q15_t pGol[numsamp];

    FILE *fptr;
    FILE *kptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q15_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_cmat_trans_q15.bin", srcdir);
    kptr = fopen(path_buffer, "r");
    if (kptr == NULL)
        return -1;

    test_Fread(&pIn, sizeof(q15_t), numsamp, fptr);
    test_Fread(&pGol, sizeof(q15_t), numsamp, kptr);

    dump_cmat_q15(pIn, MATRIX_ROW, MATRIX_COL);

    riscv_dsp_cmat_trans_q15(pIn, output, MATRIX_ROW, MATRIX_COL);
    dump_cmat_q15(output, MATRIX_COL, MATRIX_ROW);
    dump2_q15(pGol, output, numsamp);

    fclose(fptr);
    fclose(kptr);

    float32_t NRMSD;
    float32_t golden_f32[numsamp], output_f32[numsamp];
    riscv_dsp_convert_q15_f32(pGol, golden_f32, numsamp);
    riscv_dsp_convert_q15_f32(output, output_f32, numsamp);

    NRMSD = dump_rmsd(golden_f32, output_f32, numsamp);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
