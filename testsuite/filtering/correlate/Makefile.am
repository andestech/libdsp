include $(top_srcdir)/testsuite/makefile.mk
SOURCES = t_corr_f32.c t_corr_q15.c t_corr_q31.c  \
	t_corr_q7.c
check_PROGRAM = t_corr_f32 t_corr_q15 t_corr_q31  \
	t_corr_q7
TESTS = $(check_PROGRAM)


$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
