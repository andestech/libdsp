include $(top_srcdir)/testsuite/makefile.mk

SOURCES = t_abs_q31.c t_neg_q31.c t_offset_q31.c t_scale_q31.c t_shift_q31.c
check_PROGRAM = t_abs_q31 t_neg_q31 t_offset_q31 t_scale_q31 t_shift_q31
TESTS = $(check_PROGRAM)

$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c $(LIBRARY) -lm
