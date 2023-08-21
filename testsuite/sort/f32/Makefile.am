include $(top_srcdir)/testsuite/makefile.mk

SOURCES = t_sort_f32.c t_sort_merge_f32.c
check_PROGRAM = t_sort_f32 t_sort_merge_f32
TESTS = $(check_PROGRAM)

$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c $(LIBRARY) -lm
