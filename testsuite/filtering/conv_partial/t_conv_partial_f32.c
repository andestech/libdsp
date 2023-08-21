#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_filtering_math.h"

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
    int status;
    float32_t inputa[alen], inputb[alen];
    float32_t output[clen * loop];
    float32_t * tmpout = output;

    FILE *fptr;
    FILE *gptr;
    FILE *kptr;
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

    sprintf (path_buffer, "%s/golden_conv_partial_f32.bin", srcdir);
    kptr = fopen(path_buffer, "r");
    if (kptr == NULL)
        return -1;

    //initialize output buffer
    for (i = 0; i < clen * loop; i++)
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
        else if (i == 99)
        {
            startindex = 14;
        }

        test_Fread(&inputa, sizeof(float32_t), alen, fptr);
        test_Fread(&inputb, sizeof(float32_t), blen, gptr);

        status = riscv_dsp_conv_partial_f32(inputa, alen, inputb, blen, tmpout, startindex, size);

        if (status != 0)
            return -1;
        tmpout += clen;
    }

    float32_t golden[loop * clen];
    test_Fread(&golden, sizeof(float32_t), clen * loop, kptr);
    dump2_f32(golden, output, clen * loop);

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
