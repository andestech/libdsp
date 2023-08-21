include $(top_srcdir)/testsuite/makefile.mk

check_PROGRAM = t_cfft_rd2_f32 t_cfft_rd2_q15 t_cfft_rd2_q31

TESTS = $(check_PROGRAM)

#FFT_LOGN: 3 for 8, 4 for 16, 5 for 32, 6 for 64, 7 for 128, 8 for 256, 9 for 512, 10 for 1024,11 for 2048, 12 for 4096, 13 for 8192, and 14 for 16384.
#CFLAGS_DSP += -DFFT_LOGN=3

$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
