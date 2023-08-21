include $(top_srcdir)/testsuite/makefile.mk

SOURCES = t_abs_q7.c t_neg_q7.c t_offset_q7.c t_scale_q7.c t_shift_q7.c t_offset_u8.c t_scale_u8.c t_shift_u8.c
check_PROGRAM = t_abs_q7 t_neg_q7 t_offset_q7 t_scale_q7 t_shift_q7 t_offset_u8 t_scale_u8 t_shift_u8
TESTS = $(check_PROGRAM)

$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c $(LIBRARY) -lm
