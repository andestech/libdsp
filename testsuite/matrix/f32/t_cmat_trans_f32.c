#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_matrix_math.h"

#define MATRIX_ROW 70
#define MATRIX_COL 70
#define CLX_DIM 2


int main(void)
{
    uint32_t numsamp = MATRIX_ROW * MATRIX_COL * CLX_DIM;

    float32_t pIn[numsamp];
    float32_t output[numsamp];
    float32_t golden[numsamp];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_cmat_trans_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&pIn, sizeof(float32_t), numsamp, fptr);
    test_Fread(&golden, sizeof(float32_t), numsamp, gptr);

    //for(int i = 0; i < MATRIX_ROW; i++)
    //{
    //    for(int j = 0; j < MATRIX_COL; j++)
    //    {
    //        printf("(%f, %f), ", pIn[2 * j + i * MATRIX_ROW * 2], pIn[2 * j + 1 + i * MATRIX_ROW * 2]);
    //    }
    //    printf("\n");
    //}

    riscv_dsp_cmat_trans_f32(pIn, output, MATRIX_ROW, MATRIX_COL);
    dump_cmat_f32(output, MATRIX_ROW, MATRIX_COL);
    dump2_f32(golden, output, numsamp);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden, output, numsamp);

    if (NRMSD <= 0.00000012)
        return 0;
    else
        return -1;
}
