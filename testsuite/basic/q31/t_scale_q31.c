#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_basic_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t i;
    uint32_t blocksize = 32767;
    q31_t inputvec[blocksize], output1[blocksize], output2[blocksize];
    q31_t golden[2 * blocksize + 6];
    int8_t shift1 = -5, shift2 = 5;
    q31_t scale1 = 0x41000, scale2 = 0x41000;

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q31.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;
    sprintf (path_buffer, "%s/golden_scale_q31.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(inputvec, sizeof(q31_t), blocksize, fptr);
    //dump_q31("randomly generated input array", inputvec, blocksize);
    test_Fread(golden, sizeof(q31_t), 2 * blocksize + 6, gptr);

    //test1
    riscv_dsp_scale_q31(inputvec, scale1, shift1, output1, blocksize);
    //test2
    riscv_dsp_scale_q31(inputvec, scale2, shift2, output2, blocksize);
    //test3
    q31_t in[6] = {0x7ffffff0, 0x7ffffff0, 0x7ffffff0, 0x7ffffff0, 0x7ffffff0, 0x7ffffff0};
    q31_t output3[6];
    q31_t scaleval_3 = 0x7ffffff0;
    int8_t shiftval_3 = 5;
    riscv_dsp_scale_q31(in, scaleval_3, shiftval_3, output3, 6);

    //test4
    q31_t in2[3] = {0x8f000000, 0x8f000000, 0x8f000000};
    q31_t output4[3];
    q31_t scaleval_4 = 0x70000000;
    int8_t shiftval_4 = 5;
    riscv_dsp_scale_q31(in2, scaleval_4, shiftval_4, output4, 3);
    //test4 corner case for shift = 0
    q31_t gold_4[9] = {0x576fdab1, 0x400bef3d, 0x4af42473, 0x464d666d, 0x2ff45a1a, 0x20a8e990, 0x1a478afe, 0x4f5c232f, 0x2b19d47b};
    q31_t in_4[9] = {0x5851f42d, 0x40b18ccf, 0x4bb5f646, 0x47033129, 0x30705b04, 0x20fd5db4, 0x1a8b7f78, 0x502959d8, 0x2b894868};
    q31_t output5[9];
    q31_t scaleval_5 = 0x7eb851eb;
    riscv_dsp_scale_q31(in_4, scaleval_5, 0, output5, 9);
    for (i = 0; i < 9; i++)
    {
        if (output5[i] != gold_4[i])
            printf("corner case fail\n");
    }


    // NRMSD
    float32_t NRMSD;
    float32_t golden_f32[2 * blocksize + 6], output_f32[2 * blocksize + 6];
    q31_t tmpoutput[2 * blocksize + 6];
    for (i = 0; i < blocksize; i++)
    {
        tmpoutput[i] = output1[i];
        tmpoutput[i + blocksize] = output2[i];
    }
    for (i = 0; i < 3; i++)
    {
        tmpoutput[2 * blocksize + i] = output3[i];
        tmpoutput[2 * blocksize + i + 3] = output4[i];
    }
    fclose(fptr);
    fclose(gptr);

    riscv_dsp_convert_q31_f32(golden, golden_f32, 2 * blocksize + 6);
    riscv_dsp_convert_q31_f32(tmpoutput, output_f32, 2 * blocksize + 6);
    dump2_q31(golden, tmpoutput, 2 * blocksize + 6);

    NRMSD = dump_rmsd(golden_f32, output_f32, 2 * blocksize + 6);

    if (NRMSD <= 0.0000001)
        return 0;
    else
        return -1;
}
