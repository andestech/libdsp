include $(top_srcdir)/testsuite/makefile.mk

SOURCES = t_div_s64_u32.c t_div_u64_u32.c
check_PROGRAM = t_div_s64_u32 t_div_u64_u32
TESTS = $(check_PROGRAM)

$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c $(LIBRARY) -lm
