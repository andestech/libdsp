#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "riscv_dsp_filtering_math.h"
#include <inttypes.h>       //for PRId32
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
    uint32_t loop = 50;
    q31_t inputa[alen], inputb[blen], inputb_2[blen_2];
    q31_t output[clen * loop], output2[clen * loop];
    q31_t output3[clen * loop];
    q31_t *tmpout = output, * tmpout2 = output2, * tmpout3 = output3;

    FILE *fptr;
    FILE *gptr;
    FILE *g2ptr;
    FILE *kptr;
    FILE *k2ptr;
    FILE *k3ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q31_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_q31_b.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_q31_b_2.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_correlate_q31.bin", srcdir);
    kptr = fopen(path_buffer, "r");
    if (kptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_correlate_q31_2.bin", srcdir);
    k2ptr = fopen(path_buffer, "r");
    if (k2ptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_correlate_q31_3.bin", srcdir);
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
        test_Fread(&inputa, sizeof(q31_t), alen, fptr);
        test_Fread(&inputb, sizeof(q31_t), blen, gptr);
        test_Fread(&inputb_2, sizeof(q31_t), blen_2, g2ptr);

        // test1
        riscv_dsp_corr_q31(inputa, alen, inputb, blen, tmpout);
        // test2
        riscv_dsp_corr_q31(inputb, blen, inputa, alen, tmpout2);
        // test3
        riscv_dsp_corr_q31(inputa, alen, inputb_2, blen_2, tmpout3);

        tmpout += clen;
        tmpout2 += clen;
        tmpout3 += clen;
    }

    q31_t golden[loop * clen], golden2[loop * clen], golden3[loop * clen];
    test_Fread(&golden, sizeof(q31_t), clen * loop, kptr);
    test_Fread(&golden2, sizeof(q31_t), clen * loop, k2ptr);
    test_Fread(&golden3, sizeof(q31_t), clen * loop, k3ptr);
    dump2_q31(golden, output, clen * loop);
    dump2_q31(golden2, output2, clen * loop);
    dump2_q31(golden3, output3, clen * loop);

    fclose(fptr);
    fclose(gptr);
    fclose(kptr);

    float32_t tmpNRMSD;
    float32_t golden_f32[clen * loop], output_f32[clen * loop];

    riscv_dsp_convert_q31_f32(golden, golden_f32, clen * loop);
    riscv_dsp_convert_q31_f32(output, output_f32, clen * loop);
    tmpNRMSD = dump_rmsd_avg(golden_f32, output_f32, clen, loop);
    //    float32_t NRMSD;
    //    riscv_dsp_convert_q31_f32(golden2, golden_f32, clen * loop);
    //    riscv_dsp_convert_q31_f32(output2, output_f32, clen * loop);
    //    NRMSD = dump_rmsd_avg(golden_f32, output_f32, clen, loop);
    //    if (NRMSD > tmpNRMSD)
    //        tmpNRMSD = NRMSD;
    //    riscv_dsp_convert_q31_f32(golden3, golden_f32, clen * loop);
    //    riscv_dsp_convert_q31_f32(output3, output_f32, clen * loop);
    //    NRMSD = dump_rmsd_avg(golden_f32, output_f32, clen, loop);
    //    if (NRMSD > tmpNRMSD)
    //        tmpNRMSD = NRMSD;


    if (tmpNRMSD <= 0.00000001)
        return 0;
    else
        return -1;
}
