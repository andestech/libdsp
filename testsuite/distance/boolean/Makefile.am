include $(top_srcdir)/testsuite/makefile.mk

check_PROGRAM = t_bdist_dice_u32_f32 t_bdist_hamming_u32_f32 t_bdist_jaccard_u32_f32 t_bdist_kulsinski_u32_f32 t_bdist_sokal_michener_u32_f32 t_bdist_sokal_sneath_u32_f32 \
             t_bdist_rogers_tanimoto_u32_f32 t_bdist_yule_u32_f32 t_bdist_russell_rao_u32_f32
TESTS = $(check_PROGRAM)

$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c $(LIBRARY) -lm
