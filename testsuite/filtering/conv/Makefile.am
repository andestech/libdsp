include $(top_srcdir)/testsuite/makefile.mk
SOURCES = t_conv_f32.c t_conv_q15.c t_conv_q31.c t_conv_q7.c
check_PROGRAM = t_conv_f32 t_conv_q15 t_conv_q31 t_conv_q7
TESTS = $(check_PROGRAM)


$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
