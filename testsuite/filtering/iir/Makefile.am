include $(top_srcdir)/testsuite/makefile.mk
check_PROGRAM = t_liir_f32 t_liir_q15 t_liir_q31 t_liir_fast_q31 t_liir_fast_q15
TESTS = $(check_PROGRAM)


$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
