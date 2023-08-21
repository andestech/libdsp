#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_utils_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    int i;
    FILE *fptr;
    FILE *gptr;
    short inputx, inputy;
    int datasize = 30000;

    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q15_y.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_q15_x.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    float fz[datasize], f2[datasize];

    for (i = 0; i < datasize; i++)
    {
        test_Fread(&inputy, sizeof(short), 1, fptr);
        test_Fread(&inputx, sizeof(short), 1, gptr);

        float fy = (float)inputy / 32768.0;
        float fx = (float)inputx / 32768.0;
        fz[i] = atan2f(fy, fx);

        /* ------------------performance calc.---------------- */

        q15_t q2 = riscv_dsp_atan2_q15(inputy, inputx);

        /* -----------------------------------------*/

        f2[i] = (float)q2 / 32768.0 * PI;
    }
    q15_t q3 = riscv_dsp_atan2_q15(0, 0x1000);
    if (q3 != 0) return -1;
    dump2_f32(fz, f2, datasize);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(fz, f2, datasize);

    if (NRMSD <= 0.00009)
        return 0;
    else
        return -1;
}
