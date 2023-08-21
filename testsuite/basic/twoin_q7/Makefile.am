include $(top_srcdir)/testsuite/makefile.mk

SOURCES = t_add_q7.c t_mul_q7.c t_sub_q7.c t_dprod_q7.c t_add_u8_u16.c t_sub_u8_q7.c t_dprod_u8.c t_mul_u8_u16.c
check_PROGRAM = t_add_q7 t_mul_q7 t_sub_q7 t_dprod_q7 t_add_u8_u16 t_sub_u8_q7 t_dprod_u8 t_mul_u8_u16
TESTS = $(check_PROGRAM)

$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c $(LIBRARY) -lm
