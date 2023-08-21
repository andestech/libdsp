include $(top_srcdir)/testsuite/makefile.mk

check_PROGRAM = t_dist_bray_curtis_f32 t_dist_canberra_f32 t_dist_chebyshev_f32 t_dist_city_block_f32 \
                t_dist_corr_f32 t_dist_cos_f32 t_dist_euclidean_f32 t_dist_jensen_shannon_f32 \
				t_dist_minkowski_f32
TESTS = $(check_PROGRAM)

$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c $(LIBRARY) -lm
