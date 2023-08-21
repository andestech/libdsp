#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_matrix_math.h"
#include "riscv_dsp_test_accuracy.h"

#define MATRIX_ROW 55
#define MATRIX_COL 43
#define CLX_DIM 2


int main(void)
{
    uint32_t numsamp = MATRIX_ROW * MATRIX_COL * CLX_DIM;

    q31_t pIn[numsamp];
    q31_t output[numsamp];
    q31_t pGol[numsamp];

    FILE *fptr;
    FILE *kptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q31_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_cmat_trans_q31.bin", srcdir);
    kptr = fopen(path_buffer, "r");
    if (kptr == NULL)
        return -1;

    test_Fread(&pIn, sizeof(q31_t), numsamp, fptr);
    test_Fread(&pGol, sizeof(q31_t), numsamp, kptr);

    dump_cmat_q31(pIn, MATRIX_ROW, MATRIX_COL);
    riscv_dsp_cmat_trans_q31(pIn, output, MATRIX_ROW, MATRIX_COL);
    dump_cmat_q31(output, MATRIX_COL, MATRIX_ROW);
    dump2_q31(pGol, output, numsamp);

    fclose(fptr);
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
