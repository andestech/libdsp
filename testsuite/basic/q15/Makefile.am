include $(top_srcdir)/testsuite/makefile.mk

SOURCES = t_abs_q15.c t_neg_q15.c t_offset_q15.c t_scale_q15.c t_shift_q15.c
check_PROGRAM = t_abs_q15 t_neg_q15 t_offset_q15 t_scale_q15 t_shift_q15
TESTS = $(check_PROGRAM)

$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c $(LIBRARY) -lm
