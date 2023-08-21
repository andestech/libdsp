include $(top_srcdir)/testsuite/makefile.mk

SOURCES = t_add_q31.c t_mul_q31.c t_sub_q31.c t_dprod_q31.c t_div_q31.c
check_PROGRAM = t_add_q31 t_mul_q31 t_sub_q31 t_dprod_q31 t_div_q31
TESTS = $(check_PROGRAM)

$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c $(LIBRARY) -lm
