include $(top_srcdir)/testsuite/makefile.mk

SOURCES = t_set_f32.c t_set_q15.c t_set_q31.c t_set_q7.c t_dup_f32.c t_dup_q15.c t_dup_q31.c t_dup_q7.c
check_PROGRAM = t_set_f32 t_set_q15 t_set_q31 t_set_q7 t_dup_f32 t_dup_q15 t_dup_q31 t_dup_q7
TESTS = $(check_PROGRAM)

$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
