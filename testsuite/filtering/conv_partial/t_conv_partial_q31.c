#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define SRCALEN 64
#define SRCBLEN 15
#define SRCCLEN SRCALEN+SRCBLEN-1
#define STARTINDEX 5
#define SIZE 35

int main(void)
{
    uint32_t i;
    uint32_t alen = SRCALEN;
    uint32_t blen = SRCBLEN;
    uint32_t clen = SRCCLEN;
    uint32_t startindex = STARTINDEX;
    uint32_t size = SIZE;
    uint32_t loop = 100;
    q31_t inputa[alen], inputb[blen];
    q31_t output[clen * loop];
    q31_t * tmpout = output;

    FILE *fptr;
    FILE *gptr;
    FILE *kptr;
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

    sprintf (path_buffer, "%s/golden_conv_partial_q31.bin", srcdir);
    kptr = fopen(path_buffer, "r");
    if (kptr == NULL)
        return -1;

    //initialize output buffer
    for(i = 0; i < clen * loop; i++)
        output[i] = 0;

    for (i = 0; i < loop; i++)
    {
        if (i == 96)
        {
            blen = 14;
        }
        else if (i == 97)
        {
            alen = 4;
            blen = 3;
            size = 5;
            startindex = 0;
        }
        else if (i == 98)
        {
            alen = 14;
            blen = 15;
            size = 10;
            startindex = 5;
        }
        else if(i == 99)
        {
            startindex = 14;
        }

        test_Fread(&inputa, sizeof(q31_t), alen, fptr);
        test_Fread(&inputb, sizeof(q31_t), blen, gptr);

        riscv_dsp_conv_partial_q31(inputa, alen, inputb, blen, tmpout, startindex, size);
        tmpout += clen;
    }
    q31_t golden[loop * clen];
    test_Fread(&golden, sizeof(q31_t), clen * loop, kptr);
    dump2_q31(golden, output, clen * loop);

    fclose(fptr);
    fclose(gptr);
    fclose(kptr);

    float32_t NRMSD;
    float32_t golden_f32[clen * loop], output_f32[clen * loop];
    riscv_dsp_convert_q31_f32(golden, golden_f32, clen * loop);
    riscv_dsp_convert_q31_f32(output, output_f32, clen * loop);

    NRMSD = dump_rmsd_avg(golden_f32, output_f32, clen, loop);

    if (NRMSD <= 0.00000001)
        return 0;
    else
        return -1;
}
