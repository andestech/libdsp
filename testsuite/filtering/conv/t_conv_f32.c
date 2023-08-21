#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_filtering_math.h"

#define SRCALEN 64
#define SRCBLEN 16
#define SRCCLEN SRCALEN+SRCBLEN-1

int main(void)
{
    uint32_t i;
    uint32_t alen = SRCALEN;
    uint32_t blen = SRCBLEN;
    uint32_t clen = SRCCLEN;
    uint32_t loop = 10;
    float32_t inputa[alen], inputb[blen];
    float32_t output[clen * loop];
    float32_t output2[clen * loop];
    float32_t * tmpout = output;
    float32_t * tmpout2 = output2;

    FILE *fptr;
    FILE *gptr;
    FILE *kptr;
    FILE *k2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_f32_b.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_conv_f32.bin", srcdir);
    kptr = fopen(path_buffer, "r");
    if (kptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_conv_f32_2.bin", srcdir);
    k2ptr = fopen(path_buffer, "r");
    if (k2ptr == NULL)
        return -1;
    for (i = 0; i < loop; i++)
    {
        test_Fread(&inputa, sizeof(float32_t), alen, fptr);
        test_Fread(&inputb, sizeof(float32_t), blen, gptr);

        riscv_dsp_conv_f32(inputa, alen, inputb, blen, tmpout);
        riscv_dsp_conv_f32(inputb, blen, inputa, alen, tmpout2);
        tmpout += clen;
        tmpout2 += clen;
    }
    float32_t golden[loop * clen];
    float32_t golden2[loop * clen];
    test_Fread(&golden, sizeof(float32_t), clen * loop, kptr);
    test_Fread(&golden2, sizeof(float32_t), clen * loop, k2ptr);
    dump2_f32(golden, output, clen * loop);
    dump2_f32(golden2, output2, clen * loop);

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
