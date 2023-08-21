include $(top_srcdir)/testsuite/makefile.mk
check_PROGRAM = t_max_q7 t_min_q7 t_mean_q7 t_pwr_q7 t_max_u8 t_min_u8 t_mean_u8 t_std_u8
TESTS = $(check_PROGRAM)


$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
