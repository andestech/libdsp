include $(top_srcdir)/testsuite/makefile.mk

SOURCES = t_add_f32.c t_mul_f32.c t_sub_f32.c t_dprod_f32.c  \
			t_div_f32.c
check_PROGRAM = t_add_f32 t_mul_f32 t_sub_f32 t_dprod_f32  \
			t_div_f32
TESTS = $(check_PROGRAM)

$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c $(LIBRARY) -lm
