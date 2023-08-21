include $(top_srcdir)/testsuite/makefile.mk
check_PROGRAM = t_nlms_f32 t_nlms_q31 t_nlms_q15
TESTS = $(check_PROGRAM)


$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
