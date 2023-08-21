include $(top_srcdir)/testsuite/makefile.mk

SOURCES = t_add_q15.c t_mul_q15.c t_sub_q15.c t_dprod_q15.c t_dprod_u8xq15.c t_dprod_q7xq15.c
check_PROGRAM = t_add_q15 t_mul_q15 t_sub_q15 t_dprod_q15 t_dprod_u8xq15 t_dprod_q7xq15

TESTS = $(check_PROGRAM)

$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c $(LIBRARY) -lm
