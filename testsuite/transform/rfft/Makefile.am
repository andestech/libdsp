include $(top_srcdir)/testsuite/makefile.mk

SOURCES = t_rfft_f32.c t_rfft_q15.c t_rfft_q31.c

check_PROGRAM = t_rfft_f32 t_rfft_q15 t_rfft_q31

TESTS = $(check_PROGRAM)

# RFFT support FFT_LOGN:
# if define USE_CFFT_RADIX2: 4 for 16, 5 for 32, 6 for 64, 7 for 128, 8 for 256, 9 for 512, 10 for 1024,11 for 2048, 12 for 4096, 13 for 8192, and 14 for 16384.
# else use radix4: 5 for 32, 7 for 128, 9 for 512, 11 for 2048, 13 for 8192.
#CFLAGS_DSP += -DFFT_LOGN=5

$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
