include $(top_srcdir)/testsuite/makefile.mk
check_PROGRAM = t_bq_stereo_df2T_f32 t_bq_df2T_f32 t_bq_df2T_f64 t_bq_df1_f32  \
		t_bq_df1_q15 t_bq_df1_q31 t_bq_df1_32x64_q31 t_bq_df1_fast_q15 \
		t_bq_df1_fast_q31

TESTS = $(check_PROGRAM)


$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
