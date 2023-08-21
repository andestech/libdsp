#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_filtering_math.h"
#include <string.h>

#define SRCALEN 64
#define SRCBLEN 15
#define SRCBLEN_2 3
#define SRCCLEN 2 * SRCALEN -1

int main(void)
{
    uint32_t i;
    uint32_t alen = SRCALEN;
    uint32_t blen = SRCBLEN;
    uint32_t blen_2 = SRCBLEN_2;
    uint32_t clen = SRCCLEN;
    uint32_t loop = 100;
    q7_t inputa[alen] __attribute__ ((aligned(4)));
    q7_t inputb[blen] __attribute__ ((aligned(4)));
    q7_t inputb_2[blen_2] __attribute__ ((aligned(4)));
    q7_t output[clen * loop] __attribute__ ((aligned(4)));
    q7_t output2[clen * loop] __attribute__ ((aligned(4)));
    q7_t output3[clen * loop] __attribute__ ((aligned(4)));
    q7_t *tmpout = output, * tmpout2 = output2, * tmpout3 = output3;

    FILE *fptr;
    FILE *gptr;
    FILE *g2ptr;
    FILE *kptr;
    FILE *k2ptr;
    FILE *k3ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q7_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_q7_b.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_q7_b_2.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_correlate_q7.bin", srcdir);
    kptr = fopen(path_buffer, "r");
    if (kptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_correlate_q7_2.bin", srcdir);
    k2ptr = fopen(path_buffer, "r");
    if (k2ptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_correlate_q7_3.bin", srcdir);
    k3ptr = fopen(path_buffer, "r");
    if (k3ptr == NULL)
        return -1;

    for (i = 0; i < clen * loop; i++)
    {
        output[i] = 0;
        output2[i] = 0;
        output3[i] = 0;
    }

    for (i = 0; i < loop; i++)
    {
        test_Fread(&inputa, sizeof(q7_t), alen, fptr);
        test_Fread(&inputb, sizeof(q7_t), blen, gptr);
        test_Fread(&inputb_2, sizeof(q7_t), blen_2, g2ptr);

        riscv_dsp_corr_q7(inputa, alen, inputb, blen, tmpout);
        // test2
        riscv_dsp_corr_q7(inputb, blen, inputa, alen, tmpout2);
        // test3
        riscv_dsp_corr_q7(inputa, alen, inputb_2, blen_2, tmpout3);

        tmpout += clen;
        tmpout2 += clen;
        tmpout3 += clen;
    }


    q7_t golden[loop * clen], golden2[loop * clen], golden3[loop * clen];
    test_Fread(&golden, sizeof(q7_t), clen * loop, kptr);
    test_Fread(&golden2, sizeof(q7_t), clen * loop, k2ptr);
    test_Fread(&golden3, sizeof(q7_t), clen * loop, k3ptr);
    dump2_q7(golden, output, clen * loop);
    dump2_q7(golden2, output2, clen * loop);
    dump2_q7(golden3, output3, clen * loop);

    fclose(fptr);
    fclose(gptr);
    fclose(kptr);

    float32_t NRMSD;
    float32_t golden_f32[clen * loop], output_f32[clen * loop];
    riscv_dsp_convert_q7_f32(golden, golden_f32, clen * loop);
    riscv_dsp_convert_q7_f32(output, output_f32, clen * loop);
    NRMSD = dump_rmsd_avg(golden_f32, output_f32, clen, loop);

    riscv_dsp_convert_q7_f32(golden2, golden_f32, clen * loop);
    riscv_dsp_convert_q7_f32(output2, output_f32, clen * loop);
    NRMSD = dump_rmsd_avg(golden_f32, output_f32, clen, loop);

    riscv_dsp_convert_q7_f32(golden3, golden_f32, clen * loop);
    riscv_dsp_convert_q7_f32(output3, output_f32, clen * loop);
    NRMSD = dump_rmsd_avg(golden_f32, output_f32, clen, loop);

    if (NRMSD <= 0.0049)
        return 0;
    else
        return -1;
}
