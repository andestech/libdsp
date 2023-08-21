include $(top_srcdir)/testsuite/makefile.mk

SOURCES = t_clarke_f32.c t_clarke_q31.c t_inv_clarke_f32.c t_inv_clarke_q31.c t_park_f32.c t_park_q31.c t_inv_park_f32.c t_inv_park_q31.c
check_PROGRAM = t_clarke_f32 t_clarke_q31 t_inv_clarke_f32 t_inv_clarke_q31 t_park_f32 t_park_q31 t_inv_park_f32 t_inv_park_q31
TESTS = $(check_PROGRAM)


$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c $(LIBRARY) -lm
