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

    uint8_t pIn[numsamp] __attribute__ ((aligned(4)));
    uint8_t output[numsamp] __attribute__ ((aligned(4)));
    uint8_t pGol[numsamp];

    FILE *fptr;
    FILE *kptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_u8_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mat_trans_u8.bin", srcdir);
    kptr = fopen(path_buffer, "r");
    if (kptr == NULL)
        return -1;

    test_Fread(&pIn, sizeof(uint8_t), numsamp, fptr);
    test_Fread(&pGol, sizeof(uint8_t), numsamp, kptr);

    riscv_dsp_mat_trans_u8(pIn, output, MATRIX_ROW, MATRIX_COL);
    dump2_u8(pGol, output, numsamp);

    fclose(fptr);
    fclose(kptr);

    float32_t NRMSD;
    float32_t golden_f32[numsamp], output_f32[numsamp];
    _dsp_convert_u8_f32(pGol, golden_f32, numsamp);
    _dsp_convert_u8_f32(output, output_f32, numsamp);

    NRMSD = dump_rmsd(golden_f32, output_f32, numsamp);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
