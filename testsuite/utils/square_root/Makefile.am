include $(top_srcdir)/testsuite/makefile.mk
SOURCES = t_sqrt_q15.c t_sqrt_q31.c t_sqrt_f32.c
check_PROGRAM = t_sqrt_q15 t_sqrt_q31 t_sqrt_f32
TESTS = $(check_PROGRAM)


$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
