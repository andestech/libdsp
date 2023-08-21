include $(top_srcdir)/testsuite/makefile.mk
check_PROGRAM = t_mat_add_q31 t_mat_sub_q31 t_mat_mul_q31  \
			t_mat_scale_q31 t_mat_trans_q31 t_mat_oprod_q31 t_cmat_mul_q31 t_mat_mul_fast_q31
TESTS = $(check_PROGRAM)


$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
