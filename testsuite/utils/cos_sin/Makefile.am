include $(top_srcdir)/testsuite/makefile.mk

SOURCES = t_cos_f32.c t_cos_q15.c t_cos_q31.c t_sin_f32.c  \
			t_sin_q15.c t_sin_q31.c
check_PROGRAM = t_cos_f32 t_cos_q15 t_cos_q31 t_sin_f32  \
				t_sin_q15 t_sin_q31
TESTS = $(check_PROGRAM)

#if defined, Q15/Q31 are for [-2pi,2pi) instead of [-pi,pi).
#CFLAGS_DSP += -DFOUR_PI_RANGE

$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
