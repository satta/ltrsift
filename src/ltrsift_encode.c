#include "genometools.h" /* include the GenomeTools ``all-in-one'' header */

int main(GT_UNUSED int argc, GT_UNUSED char *argv[])
{
  GtEncseqEncoder *e = NULL;
  GtStrArray *infiles = NULL;
  GtError *err = NULL;
  int had_err = 0;
  gt_lib_init();

  if (gt_version_check(GT_MAJOR_VERSION, GT_MINOR_VERSION, GT_MICRO_VERSION)) {
    fprintf(stderr, "error: %s\n", gt_version_check(GT_MAJOR_VERSION,
                                                    GT_MINOR_VERSION,
                                                    GT_MICRO_VERSION));
    return EXIT_FAILURE;
  }

  if (argc < 3 || !gt_file_exists(argv[1])) {
    fprintf(stderr, "Usage: %s <sequence file> <indexname>\n", argv[0]);
    gt_lib_clean();
    return EXIT_FAILURE;
  }

  err = gt_error_new();
  infiles = gt_str_array_new();
  gt_str_array_add_cstr(infiles, argv[1]);
  e = gt_encseq_encoder_new();
  gt_encseq_encoder_enable_lossless_support(e);
  had_err = gt_encseq_encoder_encode(e, infiles, argv[2], err);

  gt_str_array_delete(infiles);
  gt_encseq_encoder_delete(e);
  gt_error_delete(err);

  if (gt_lib_clean())
    return GT_EXIT_PROGRAMMING_ERROR;
  return had_err ? EXIT_FAILURE : EXIT_SUCCESS;
}
