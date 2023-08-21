include $(top_srcdir)/testsuite/makefile.mk
check_PROGRAM = t_max_q15 t_min_q15 t_mean_q15 t_pwr_q15  \
			t_var_q15 t_rms_q15 t_std_q15
TESTS = $(check_PROGRAM)


$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
