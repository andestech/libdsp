include $(top_srcdir)/testsuite/makefile.mk

check_PROGRAM = t_cfft_rd4_f32 t_cfft_rd4_q15 t_cfft_rd4_q31

TESTS = $(check_PROGRAM)

# CFFT_rd4 support FFT_LOGN:
#FFT_LOGN: 4 for 16, 6 for 64, 8 for 256, 10 for 1024,12 for 4096, and 14 for 16384.
#CFLAGS_DSP += -DFFT_LOGN=4

$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
