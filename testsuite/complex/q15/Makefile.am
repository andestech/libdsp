include $(top_srcdir)/testsuite/makefile.mk
SOURCES = t_cconj_q15.c t_cmag_q15.c \
			t_cmag_sqr_q15.c t_cmul_q15.c \
				t_cmul_real_q15.c t_cdprod_q15.c \
				t_cdprod_q15_typ2.c
check_PROGRAM = t_cconj_q15 t_cmag_q15 \
					t_cmag_sqr_q15 t_cmul_q15 \
						t_cmul_real_q15 t_cdprod_q15 \
						t_cdprod_q15_typ2
TESTS = $(check_PROGRAM)


$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c $(LIBRARY) -lm
