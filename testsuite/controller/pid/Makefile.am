include $(top_srcdir)/testsuite/makefile.mk

SOURCES = t_pid_f32.c t_pid_q15.c t_pid_q31.c
check_PROGRAM = t_pid_f32 t_pid_q15 t_pid_q31
TESTS = $(check_PROGRAM)


$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c $(LIBRARY) -lm
