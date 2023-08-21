#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_matrix_math.h"

#define MATRIX_M 99
#define MATRIX_N 99
#define MATRIX_P 99

#define MATRIX_1 1
#define MATRIX_2 2

int main(void)
{
    uint16_t asamp = MATRIX_M * MATRIX_N;
    uint16_t bsamp = MATRIX_N * MATRIX_P;
    uint16_t osamp = MATRIX_M * MATRIX_P;

    q15_t pInA[asamp] __attribute__ ((aligned(4)));
    q15_t pInB[bsamp] __attribute__ ((aligned(4)));
    q15_t pGol[osamp];
    q15_t output[osamp] __attribute__ ((aligned(4)));

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

    sprintf (path_buffer, "%s/golden_mat_mult_q15.bin", srcdir);
    kptr = fopen(path_buffer, "r");
    if (kptr == NULL)
        return -1;

    test_Fread(&pInA, sizeof(q15_t), asamp, fptr);
    test_Fread(&pInB, sizeof(q15_t), bsamp, gptr);
    test_Fread(&pGol, sizeof(q15_t), osamp, kptr);

    riscv_dsp_mat_mul_q15(pInA, pInB, output, MATRIX_M, MATRIX_N, MATRIX_P);
    dump2_q15(pGol, output, osamp);

    fclose(fptr);
    fclose(gptr);
    fclose(kptr);

    //corner case testing
    q15_t A[MATRIX_2 * MATRIX_2] = {0x2000, 0x3000, 0x2000, 0x3000};
    q15_t B[MATRIX_2 * MATRIX_2] = {0x1000, 0x2000, 0x1000, 0x2000};
    q15_t C1[MATRIX_1];
    q15_t C2[MATRIX_1];
    q15_t C3[MATRIX_2];
    q15_t C4[MATRIX_2];


    riscv_dsp_mat_mul_q15(A, B, C1, MATRIX_1, MATRIX_1, MATRIX_1);
    if (C1[0] != 0x400)
    {
        printf("corner case1 fail\n");
        return -1;
    }
    riscv_dsp_mat_mul_q15(A, B, C2, MATRIX_1, MATRIX_2, MATRIX_1);
    if (C2[0] != 0x1000)
    {
        printf("corner case2 fail\n");
        return -1;
    }
    riscv_dsp_mat_mul_q15(A, B, C3, MATRIX_1, MATRIX_2, MATRIX_2);
    if ((C3[0] != 0xA00) || (C3[1] != 0x1400))
    {
        printf("corner case3 fail\n");
        return -1;
    }
    riscv_dsp_mat_mul_q15(A, B, C4, MATRIX_2, MATRIX_2, MATRIX_1);
    if (C4[0] != 0x1000 || (C4[1] != 0x1000))
    {
        printf("corner case4 fail\n");
        return -1;
    }
    printf("corner case pass\n");
    float32_t NRMSD;
    float32_t golden_f32[osamp], output_f32[osamp];
    riscv_dsp_convert_q15_f32(pGol, golden_f32, osamp);
    riscv_dsp_convert_q15_f32(output, output_f32, osamp);

    NRMSD = dump_rmsd(golden_f32, output_f32, osamp);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
