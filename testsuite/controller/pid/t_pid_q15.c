#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_controller_math.h"

#define KP_VAL 293
#define KI_VAL 111
#define KD_VAL 0x4000
#define RESET 1

int main(void)
{
    int i, loop = 20000;
    riscv_dsp_pid_q15_t Si;
    Si.Kp = KP_VAL;
    Si.Ki = KI_VAL;
    Si.Kd = KD_VAL;
    int32_t resetState = RESET;

    riscv_dsp_init_pid_q15(&Si, resetState);

    LOG("gain1:%d, gain2:%d, gain3:%d\n", Si.gain1, Si.gain2, Si.gain3);
    LOG("STATE:[%d %d %d]\n", Si.state[0], Si.state[1], Si.state[2]);
    LOG("Kp:%d, Ki:%d, Kd:%d\n", Si.Kp, Si.Ki, Si.Kd);

    FILE *f1ptr, *g1ptr;
    q15_t inputa, golden[loop], out[loop];

    const char *srcdir = GETENV_SRCDIR;
    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q15_a.bin", srcdir);
    f1ptr = fopen(path_buffer, "r");
    sprintf (path_buffer, "%s/golden_pid_q15.bin", srcdir);
    g1ptr = fopen(path_buffer, "r");

    for (i = 0; i < 20000; i++)
    {
        test_Fread(&inputa, sizeof(q15_t), 1, f1ptr);
        out[i] = riscv_dsp_pid_q15(&Si, inputa);
    }
    test_Fread(&golden, sizeof(q15_t), loop, g1ptr);
    dump2_q15(golden, out, loop);

    fclose(f1ptr);
    fclose(g1ptr);

    float32_t NRMSD;
    float32_t golden_f32[loop], output_f32[loop];
    riscv_dsp_convert_q15_f32(golden, golden_f32, loop);
    riscv_dsp_convert_q15_f32(out, output_f32, loop);

    NRMSD = dump_rmsd(golden_f32, output_f32, loop);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
