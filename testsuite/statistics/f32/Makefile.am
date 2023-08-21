include $(top_srcdir)/testsuite/makefile.mk
check_PROGRAM = t_max_f32 t_max_val_f32 t_min_f32 t_mean_f32 t_pwr_f32  \
			t_var_f32 t_rms_f32 t_std_f32 t_entropy_f32 t_relative_entropy_f32 \
			t_lse_f32 t_lse_dprod_f32 t_gaussian_naive_bayes_est_f32
TESTS = $(check_PROGRAM)


$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
