include $(top_srcdir)/testsuite/makefile.mk
check_PROGRAM = t_cconj_f32 t_cmag_f32 \
					t_cmag_sqr_f32 t_cmul_f32 \
						t_cmul_real_f32 t_cdprod_f32 t_cdprod_typ2_f32
TESTS = $(check_PROGRAM)


$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c $(LIBRARY) -lm
