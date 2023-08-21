include $(top_srcdir)/testsuite/makefile.mk
SOURCES = t_fir_f32.c t_fir_q15.c t_fir_q31.c t_fir_q7.c  \
			t_dcmfir_f32.c t_dcmfir_q15.c \
				t_dcmfir_q31.c t_fir_fast_q31.c t_fir_fast_q15.c \
				t_dcmfir_fast_q31.c t_dcmfir_fast_q15.c
check_PROGRAM = t_fir_f32 t_fir_q15 t_fir_q31 t_fir_q7  \
			t_dcmfir_f32 t_dcmfir_q15 \
				t_dcmfir_q31 t_fir_fast_q31 t_fir_fast_q15 \
				t_dcmfir_fast_q31 t_dcmfir_fast_q15
TESTS = $(check_PROGRAM)


$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
