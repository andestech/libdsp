include $(top_srcdir)/testsuite/makefile.mk
check_PROGRAM = t_cconj_q31 t_cmag_q31  \
		t_cmag_sqr_q31 t_cmul_q31  \
		t_cmul_real_q31 t_cdprod_q31 t_cdprod_q31_typ2
TESTS = $(check_PROGRAM)


$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c $(LIBRARY) -lm
