#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_filtering_math.h"

#define SRCALEN 64
#define SRCALEN_2 3
#define SRCBLEN 16
#define SRCCLEN SRCALEN+SRCBLEN-1
#define SRCCLEN_2 SRCALEN_2+SRCBLEN-1
int main(void)
{
    uint32_t i;
    uint32_t alen = SRCALEN;
    uint32_t alen2 = SRCALEN_2;
    uint32_t blen = SRCBLEN;
    uint32_t clen = SRCCLEN;
    uint32_t clen2 = SRCCLEN_2;
    uint32_t loop = 100;
    q7_t inputa[alen] __attribute__ ((aligned(4)));
    q7_t inputa2[alen2] __attribute__ ((aligned(4)));
    q7_t inputb[blen] __attribute__ ((aligned(4)));
    q7_t output[clen * loop] __attribute__ ((aligned(4)));
    q7_t output2[clen2 * loop] __attribute__ ((aligned(4)));
    q7_t * tmpout = output;
    q7_t * tmpout2 = output2;

    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    FILE *kptr;
    FILE *k2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q7_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_q7_a_2.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (f2ptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_q7_b.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_conv_q7.bin", srcdir);
    kptr = fopen(path_buffer, "r");
    if (kptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_conv_q7_2.bin", srcdir);
    k2ptr = fopen(path_buffer, "r");
    if (k2ptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&inputa, sizeof(q7_t), alen, fptr);
        test_Fread(&inputa2, sizeof(q7_t), alen2, f2ptr);
        test_Fread(&inputb, sizeof(q7_t), blen, gptr);

        riscv_dsp_conv_q7(inputa, alen, inputb, blen, tmpout);
        riscv_dsp_conv_q7(inputa2, alen2, inputb, blen, tmpout2);
        tmpout += clen;
        tmpout2 += clen2;
    }
    q7_t golden[loop * clen];
    q7_t golden2[loop * clen2];
    test_Fread(&golden, sizeof(q7_t), clen * loop, kptr);
    test_Fread(&golden2, sizeof(q7_t), clen2 * loop, k2ptr);
    dump2_q7(golden, output, clen * loop);
    dump2_q7(golden2, output2, clen2 * loop);

    fclose(fptr);
    fclose(gptr);
    fclose(kptr);

    float32_t NRMSD;
    float32_t golden_f32[clen * loop], output_f32[clen * loop];
    riscv_dsp_convert_q7_f32(golden, golden_f32, clen * loop);
    riscv_dsp_convert_q7_f32(output, output_f32, clen * loop);

    NRMSD = dump_rmsd_avg(golden_f32, output_f32, clen, loop);

    if (NRMSD <= 0.0046)
        return 0;
    else
        return -1;
}
