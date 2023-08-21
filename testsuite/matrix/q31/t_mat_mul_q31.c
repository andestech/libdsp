#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_matrix_math.h"
#include "riscv_dsp_test_accuracy.h"

#define MATRIX_M 39
#define MATRIX_N 39
#define MATRIX_P 39

#define MATRIX_1 1
#define MATRIX_2 2

int main(void)
{
    uint16_t asamp = MATRIX_M * MATRIX_N;
    uint16_t bsamp = MATRIX_N * MATRIX_P;
    uint16_t osamp = MATRIX_M * MATRIX_P;

    q31_t pInA[asamp];
    q31_t pInB[bsamp];
    q31_t output[osamp];
    q31_t pGol[osamp];

    FILE *fptr;
    FILE *gptr;
    FILE *kptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q31_a_1.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_q31_b_1.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mat_mult_q31.bin", srcdir);
    kptr = fopen(path_buffer, "r");
    if (kptr == NULL)
        return -1;

    test_Fread(&pInA, sizeof(q31_t), asamp, fptr);
    test_Fread(&pInB, sizeof(q31_t), bsamp, gptr);
    test_Fread(&pGol, sizeof(q31_t), osamp, kptr);

    riscv_dsp_mat_mul_q31(pInA, pInB, output, MATRIX_M, MATRIX_N, MATRIX_P);
    dump2_q31(pGol, output, osamp);

    fclose(fptr);
    fclose(gptr);
    fclose(kptr);

    //corner case testing
    q31_t A[MATRIX_2 * MATRIX_2] = {0x20000000, 0x30000000, 0x20000000, 0x30000000};
    q31_t B[MATRIX_2 * MATRIX_2] = {0x10000000, 0x20000000, 0x10000000, 0x20000000};
    q31_t C1[MATRIX_1];
    q31_t C2[MATRIX_1];
    q31_t C3[MATRIX_2];
    q31_t C4[MATRIX_2];


    riscv_dsp_mat_mul_q31(A, B, C1, MATRIX_1, MATRIX_1, MATRIX_1);
    if (C1[0] != 0x4000000)
    {
        printf("corner case1 fail\n");
        return -1;
    }
    riscv_dsp_mat_mul_q31(A, B, C2, MATRIX_1, MATRIX_2, MATRIX_1);
    if (C2[0] != 0x10000000)
    {
        printf("corner case2 fail\n");
        return -1;
    }
    riscv_dsp_mat_mul_q31(A, B, C3, MATRIX_1, MATRIX_2, MATRIX_2);
    if ((C3[0] != 0xA000000) || (C3[1] != 0x14000000))
    {
        printf("corner case3 fail\n");
        return -1;
    }
    riscv_dsp_mat_mul_q31(A, B, C4, MATRIX_2, MATRIX_2, MATRIX_1);
    if ((C4[0] != 0x10000000) || (C4[1] != 0x10000000))
    {
        printf("corner case4 fail\n");
        return -1;
    }
    printf("corner case pass\n");

    float32_t NRMSD;
    float32_t golden_f32[osamp], output_f32[osamp];
    riscv_dsp_convert_q31_f32(pGol, golden_f32, osamp);
    riscv_dsp_convert_q31_f32(output, output_f32, osamp);

    NRMSD = dump_rmsd(golden_f32, output_f32, osamp);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
