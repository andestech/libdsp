include $(top_srcdir)/testsuite/makefile.mk
check_PROGRAM = t_mat_add_f32 t_mat_inverse_f32 t_mat_mul_f32 \
				t_mat_scale_f32 t_mat_sub_f32 t_mat_trans_f32 t_mat_inverse_f64 \
				t_mat_mul_f64 t_mat_pwr2_cache_f64 t_cmat_mul_f32
TESTS = $(check_PROGRAM)


$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
