#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_matrix_math.h"

#define MATRIX_M 47
#define MATRIX_N 37
#define MATRIX_P 33

int main(void)
{
    uint16_t asamp = MATRIX_M * MATRIX_N * 2;
    uint16_t bsamp = MATRIX_N * MATRIX_P * 2;
    uint16_t osamp = MATRIX_M * MATRIX_P * 2;

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

    sprintf (path_buffer, "%s/golden_cmat_mul_q15.bin", srcdir);
    kptr = fopen(path_buffer, "r");
    if (kptr == NULL)
        return -1;

    test_Fread(&pInA, sizeof(q15_t), asamp, fptr);
    test_Fread(&pInB, sizeof(q15_t), bsamp, gptr);
    test_Fread(&pGol, sizeof(q15_t), osamp, kptr);
    dump_q15("A", pInA, asamp);
    dump_q15("B", pInB, bsamp);

    riscv_dsp_cmat_mul_q15(pInA, pInB, output, MATRIX_M, MATRIX_N, MATRIX_P);
    dump2_q15(pGol, output, osamp);

    fclose(fptr);
    fclose(gptr);
    fclose(kptr);

    // corner case
    printf("---- check corner case \n");
    q15_t a[8] = {0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,  0x8000, 0x8000};
    q15_t b[8] = {0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,  0x8000, 0x8000};
    q15_t c[8];
    q15_t corner_golden[8] = {0x0, 0x7fff, 0x0, 0x7fff, 0x0, 0x7fff, 0x0, 0x7fff};
    riscv_dsp_cmat_mul_q15(a, b, c, 2, 2, 2);
    for(int i = 0; i < 8; i++)
    {
        if(c[i] != corner_golden[i])
        {
            printf("overflow\n");
            return -1;
        }
    }
    printf("---- corner case: pass\n");

    float32_t NRMSD;
    float32_t golden_f32[osamp], output_f32[osamp];
    riscv_dsp_convert_q15_f32(pGol, golden_f32, osamp);
    riscv_dsp_convert_q15_f32(output, output_f32, osamp);

    NRMSD = dump_rmsd(golden_f32, output_f32, osamp);

    if (NRMSD < 0.000008)
        return 0;
    else
        return -1;
}
