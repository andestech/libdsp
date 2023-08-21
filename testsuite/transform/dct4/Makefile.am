include $(top_srcdir)/testsuite/makefile.mk

SOURCES =  t_dct4_f32.c t_dct4_q15.c t_dct4_q31.c
check_PROGRAM = t_dct4_f32 t_dct4_q15 t_dct4_q31

TESTS = $(check_PROGRAM)

#for better performance of DCT4, RES_LOGN >= FFT_LOGN + 3
#The FFT_LOGN support for DCT4:
# FFL_LOGN: 4 for 16, 5 for 32, 6 for 64, 7 for 128, 8 for 256, 9 for 512, 10 for 1024,11 for 2048.
#CFLAGS_DSP += -DFFT_LOGN=7

$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
