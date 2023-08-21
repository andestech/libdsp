#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define SRCALEN 64
#define SRCBLEN 15
#define SRCBLEN_2 3
#define SRCCLEN (2 * SRCALEN - 1)

int main(void)
{
    uint32_t i;
    uint32_t alen = SRCALEN;
    uint32_t blen = SRCBLEN;
    uint32_t blen_2 = SRCBLEN_2;
    uint32_t clen = SRCCLEN;
    uint32_t loop = 10;
    float32_t inputa[alen], inputb[blen],  inputb_2[blen_2];
    float32_t output[clen * loop];
    float32_t output2[clen * loop];
    float32_t output3[clen * loop];
    float32_t *tmpout = output;
    float32_t *tmpout2 = output2;
    float32_t *tmpout3 = output3;

    FILE *fptr;
    FILE *gptr;
    FILE *g2ptr;
    FILE *kptr;
    FILE *k2ptr;
    FILE *k3ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32_a.bin", srcdir);
    fptr = fopen(path_buffer, "rb");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_f32_b.bin", srcdir);
    gptr = fopen(path_buffer, "rb");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_f32_b_2.bin", srcdir);
    g2ptr = fopen(path_buffer, "rb");
    if (g2ptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_correlate_f32.bin", srcdir);
    kptr = fopen(path_buffer, "rb");
    if (kptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_correlate_f32_2.bin", srcdir);
    k2ptr = fopen(path_buffer, "rb");
    if (k2ptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_correlate_f32_3.bin", srcdir);
    k3ptr = fopen(path_buffer, "rb");
    if (k3ptr == NULL)
        return -1;

    for (i = 0; i < clen * loop; i++)
    {
        output[i] = 0.0F;
        output2[i] = 0.0F;
        output3[i] = 0.0F;
    }

    for (i = 0; i < loop; i++)
    {
        test_Fread(inputa, sizeof(float32_t), alen, fptr);
        test_Fread(inputb, sizeof(float32_t), blen, gptr);
        test_Fread(inputb_2, sizeof(float32_t), blen_2, g2ptr);

        // test1
        riscv_dsp_corr_f32(inputa, alen, inputb, blen, tmpout);
        // test2
        riscv_dsp_corr_f32(inputb, blen, inputa, alen, tmpout2);
        // test3
        riscv_dsp_corr_f32(inputa, alen, inputb_2, blen_2, tmpout3);

        tmpout += clen;
        tmpout2 += clen;
        tmpout3 += clen;
    }

    float32_t golden[loop * clen];
    float32_t golden2[loop * clen];
    float32_t golden3[loop * clen];
    test_Fread(&golden, sizeof(float32_t), clen * loop, kptr);
    test_Fread(&golden2, sizeof(float32_t), clen * loop, k2ptr);
    test_Fread(&golden3, sizeof(float32_t), clen * loop, k3ptr);
    dump2_f32(golden, output, clen * loop);
    dump2_f32(golden2, output2, clen * loop);
    dump2_f32(golden3, output3, clen * loop);

    fclose(fptr);
    fclose(gptr);
    fclose(kptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd_avg(golden, output, clen, loop);

    if (NRMSD <= 0.00000003)
        return 0;
    else
        return -1;
}
