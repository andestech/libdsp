#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_controller_math.h"
#include "riscv_dsp_test_accuracy.h"

#define KP_VAL 0.02
#define KI_VAL 0.03
#define KD_VAL 0.014
#define RESET 1

int main(void)
{
    int i, loop = 20000;
    riscv_dsp_pid_f32_t Si;
    Si.Kp = KP_VAL;
    Si.Ki = KI_VAL;
    Si.Kd = KD_VAL;
    int32_t resetState = RESET;

    riscv_dsp_init_pid_f32(&Si, resetState);

    LOG("gain1:%f, gain2:%f, gain3:%f\n", Si.gain1, Si.gain2, Si.gain3);
    LOG("STATE:[%f %f %f]\n", Si.state[0], Si.state[1], Si.state[2]);
    LOG("Kp:%f, Ki:%f, Kd:%f\n", Si.Kp, Si.Ki, Si.Kd);

    FILE *f1ptr, *g1ptr;
    float inputa, golden[loop], out[loop];

    const char *srcdir = GETENV_SRCDIR;
    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32_a.bin", srcdir);
    f1ptr = fopen(path_buffer, "r");
    sprintf (path_buffer, "%s/golden_pid_f32.bin", srcdir);
    g1ptr = fopen(path_buffer, "r");

    for (i = 0; i < 20000; i++)
    {
        test_Fread(&inputa, sizeof(float), 1, f1ptr);
        out[i] = riscv_dsp_pid_f32(&Si, inputa);
    }
    test_Fread(&golden, sizeof(float), loop, g1ptr);
    dump2_f32(golden, out, loop);

    fclose(f1ptr);
    fclose(g1ptr);

    float NRMSD;
    NRMSD = dump_rmsd(golden, out, loop);

    if (NRMSD <= 0.00000001)
        return 0;
    else
        return -1;
}
