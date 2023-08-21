include $(top_srcdir)/testsuite/makefile.mk
check_PROGRAM = t_lfir_f32 t_lfir_q15 t_lfir_q31
TESTS = $(check_PROGRAM)


$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
