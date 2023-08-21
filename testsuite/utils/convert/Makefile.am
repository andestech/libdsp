include $(top_srcdir)/testsuite/makefile.mk

SOURCES = t_convert_f32_q15.c t_convert_f32_q31.c t_convert_f32_q7.c t_convert_q15_f32.c t_convert_q15_q31.c t_convert_q15_q7.c t_convert_q31_f32.c t_convert_q31_q15.c t_convert_q31_q7.c t_convert_q7_f32.c t_convert_q7_q15.c t_convert_q7_q31.c
check_PROGRAM = t_convert_f32_q15 t_convert_f32_q31 t_convert_f32_q7 t_convert_q15_f32 t_convert_q15_q31 t_convert_q15_q7 t_convert_q31_f32 t_convert_q31_q15 t_convert_q31_q7 t_convert_q7_f32 t_convert_q7_q15 t_convert_q7_q31
TESTS = $(check_PROGRAM)

$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
