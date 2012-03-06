/*
  Copyright (c) 2012 Sascha Kastens <sascha.kastens@studium.uni-hamburg.de>
  Copyright (c) 2012 Center for Bioinformatics, University of Hamburg

  Permission to use, copy, modify, and distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "refseq_match_stream.h"

#define ATTR_FLCAND "flcand"
#define ATTR_LTRFAM "ltrfam"
#define ATTR_NAME   "name"
#define NEW_FN_TYPE "nucleotide_match"
#define GT_BLAST_PATH_ENV "GT_BLAST_PATH"

struct LTRGuiRefseqMatchStream {
  const GtNodeStream parent_instance;
  GtNodeStream *in_stream;
  GtArray *nodes;
  GtHashmap *header_to_fn;
  const char *indexname,
             *refseq_file,
             *seq_file;
  unsigned long next_index;
  bool first_next,
       dust,
       flcands;
  float min_ali_len_perc;
  int word_size,
      gapopen,
      gapextend,
      penalty,
      reward,
      num_threads;
  double evalue,
         xdrop,
         identity;
  const char *moreblast;
};

#define ltrgui_refseq_match_stream_cast(GS)\
        gt_node_stream_cast(ltrgui_refseq_match_stream_class(), GS);

static void free_hash_elem(void *elem)
{
  gt_free(elem);
}

static int extract_sequences(LTRGuiRefseqMatchStream *rms, GtError *err)
{
  GtStr *seqid;
  GtGenomeNode *gn;
  GtFeatureNode *curnode;
  GtFeatureNodeIterator *fni;
  GtRange range;
  GtFile *outfp;
  GtEncseqLoader *el = NULL;
  GtEncseq *encseq = NULL;
  char *buffer, header[BUFSIZ];
  const char *attr;
  int had_err = 0;
  unsigned long i,
                seqnum,
                startpos;

  gt_assert(rms);
  gt_error_check(err);

  outfp = gt_file_new(rms->seq_file, "w", err);
  if (!outfp)
    had_err = -1;

  if (!had_err) {
    el = gt_encseq_loader_new();
    encseq = gt_encseq_loader_load(el, rms->indexname, err);
    if (!encseq)
      had_err = -1;
  }

  if (!had_err) {
    for (i = 0; i < gt_array_size(rms->nodes); i++) {
      gn = *(GtGenomeNode**) gt_array_get(rms->nodes, i);
      fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);
      curnode = gt_feature_node_iterator_next(fni);
      seqid = gt_genome_node_get_seqid((GtGenomeNode*) curnode);
      range = gt_genome_node_get_range((GtGenomeNode*) curnode);
      if (rms->flcands) {
        attr = gt_feature_node_get_attribute(curnode, ATTR_FLCAND);
        if (!attr) {
          gt_feature_node_iterator_delete(fni);
          continue;
        }
      }
      attr = gt_feature_node_get_attribute(curnode, ATTR_LTRFAM);
      if (attr)
        (void) snprintf(header, BUFSIZ, "%s_%s_%lu_%lu", attr,
                        gt_str_get(seqid), range.start, range.end);
      else
        (void) snprintf(header, BUFSIZ, "%s_%lu_%lu", gt_str_get(seqid),
                        range.start, range.end);
      gt_hashmap_add(rms->header_to_fn, (void*) gt_cstr_dup(header),
                     (void*) curnode);
      (void) sscanf(gt_str_get(seqid), "seq%lu", &seqnum);
      buffer = gt_calloc((size_t) gt_range_length(&range) + 1, sizeof (char));
      startpos = gt_encseq_seqstartpos(encseq, seqnum);
      gt_encseq_extract_decoded(encseq, buffer, startpos + range.start - 1,
                                startpos + range.end - 1);
      gt_fasta_show_entry(header, buffer, gt_range_length(&range), 50, outfp);
      gt_free(buffer);
      gt_feature_node_iterator_delete(fni);
    }
    gt_file_delete(outfp);
  }

  return had_err;
}

static void add_match_to_fn(LTRGuiRefseqMatchStream *rms, GtMatch *match,
                            GtError *err)
{
  GtStr *seq;
  GtFeatureNode *fn;
  GtGenomeNode *new_node;
  GtRange range;
  const char *seqid1, *seqid2;
  unsigned long ali_length,
                fn_length,
                min_ali_length;

  gt_assert(rms && match);
  gt_error_check(err);

  seqid1 = gt_match_get_seqid1(match);
  fn = (GtFeatureNode*) gt_hashmap_get(rms->header_to_fn, (void*) seqid1);
  fn_length = gt_genome_node_get_length((GtGenomeNode*) fn);
  min_ali_length = rms->min_ali_len_perc * fn_length;
  ali_length = gt_match_blast_get_align_length((GtMatchBlast*) match);
  if (ali_length < min_ali_length) {
    gt_match_delete(match);
    return;
  }
  gt_match_get_range_seq1(match, &range);
  seq = gt_genome_node_get_seqid((GtGenomeNode*) fn);
  new_node = gt_feature_node_new(seq, NEW_FN_TYPE, range.start,
                                 range.end, GT_STRAND_FORWARD);
  seqid2 = gt_match_get_seqid2(match);
  gt_feature_node_add_attribute((GtFeatureNode*) new_node, ATTR_NAME, seqid2);
  /* TODO: add attributes */
  gt_feature_node_add_child(fn, (GtFeatureNode*) new_node);

  gt_match_delete(match);
}

static int refseq_match(LTRGuiRefseqMatchStream *rms, GtError *err)
{
  GtMatchIterator *mi;
  GtMatch *match = NULL;
  GtMatchIteratorStatus status;
  char makeblastdb_call[BUFSIZ],
       *env = NULL;
  int had_err = 0;

  gt_assert(rms);
  gt_error_check(err);

  env = getenv(GT_BLAST_PATH_ENV);
  if (env)
    (void) snprintf(makeblastdb_call, BUFSIZ,
                    "%s/makeblastdb -in %s -dbtype nucl -logfile "
                    "makeblastdb.txt",
                    env, rms->refseq_file);
  else
    (void) snprintf(makeblastdb_call, BUFSIZ,
                    "makeblastdb -in %s -dbtype nucl -logfile "
                    "makeblastdb.txt",
                    rms->refseq_file);
  had_err = system(makeblastdb_call);
  if (!had_err) {
    mi = gt_match_iterator_blastn_process_new(rms->seq_file,
                                              rms->refseq_file,
                                              rms->evalue,
                                              rms->dust,
                                              rms->word_size,
                                              rms->gapopen,
                                              rms->gapextend,
                                              rms->penalty,
                                              rms->reward,
                                              rms->identity,
                                              rms->num_threads,
                                              rms->xdrop,
                                              rms->moreblast,
                                              err);
    if (mi) {
      while ((status = gt_match_iterator_next(mi, &match, err))
             != GT_MATCHER_STATUS_END) {
        if (status == GT_MATCHER_STATUS_OK) {
          add_match_to_fn(rms, match, err);
        } else
          had_err = -1;
      }
    } else
      had_err = -1;
  } else
    gt_error_set(err, "Could not run makeblastdb.");

  return had_err;
}

static int ltrgui_refseq_match_stream_next(GtNodeStream *ns, GtGenomeNode **gn,
                                           GtError *err)
{
  LTRGuiRefseqMatchStream *rms;
  GtGenomeNode *ref_gn;
  int had_err = 0;

  gt_error_check(err);
  rms = ltrgui_refseq_match_stream_cast(ns);
  if (rms->first_next) {
    while (!(had_err = gt_node_stream_next(rms->in_stream, gn, err)) && *gn) {
      gt_assert(*gn && !had_err);
      ref_gn = gt_genome_node_ref(*gn);
      gt_array_add(rms->nodes, ref_gn);
    }
    if (!had_err)
      had_err = extract_sequences(rms, err);
    if (!had_err)
      had_err = refseq_match(rms, err);
    if (!had_err) {
      *gn = *(GtGenomeNode**) gt_array_get(rms->nodes, rms->next_index);
      rms->next_index++;
      rms->first_next = false;
      return 0;
    }
  } else {
    if (rms->next_index >= gt_array_size(rms->nodes))
      *gn = NULL;
    else {
      *gn = *(GtGenomeNode**) gt_array_get(rms->nodes, rms->next_index);
      rms->next_index++;
    }
    return 0;
  }

  return had_err;
}

static void ltrgui_refseq_match_stream_free(GtNodeStream *gs)
{
  LTRGuiRefseqMatchStream *rms = ltrgui_refseq_match_stream_cast(gs);
  gt_hashmap_delete(rms->header_to_fn);
  gt_node_stream_delete(rms->in_stream);
}

const GtNodeStreamClass* ltrgui_refseq_match_stream_class(void)
{
  static const GtNodeStreamClass *gsc = NULL;
  if (!gsc)
    gsc = gt_node_stream_class_new(sizeof (LTRGuiRefseqMatchStream),
                                   ltrgui_refseq_match_stream_free,
                                   ltrgui_refseq_match_stream_next);
  return gsc;
}

GtNodeStream* ltrgui_refseq_match_stream_new(GtNodeStream *in_stream,
                                             const char *indexname,
                                             const char *refseq_file,
                                             const char *seq_file,
                                             double evalue,
                                             bool dust,
                                             int word_size,
                                             int gapopen,
                                             int gapextend,
                                             int penalty,
                                             int reward,
                                             int num_threads,
                                             double xdrop,
                                             double identity,
                                             const char *moreblast,
                                             bool flcands,
                                             float min_ali_len_perc,
                                             GT_UNUSED GtError *err)
{
  GtNodeStream *gs;
  LTRGuiRefseqMatchStream *rms;
  gs = gt_node_stream_create(ltrgui_refseq_match_stream_class(), true);
  rms = ltrgui_refseq_match_stream_cast(gs);
  rms->in_stream = gt_node_stream_ref(in_stream);
  rms->first_next = true;
  rms->next_index = 0;
  rms->header_to_fn = gt_hashmap_new(GT_HASH_STRING, free_hash_elem, NULL);
  rms->indexname = indexname;
  rms->refseq_file = refseq_file;
  rms->seq_file = seq_file;
  rms->evalue = evalue;
  rms->dust = dust;
  rms->word_size = word_size;
  rms->gapopen = gapopen;
  rms->gapextend = gapextend;
  rms->penalty = penalty;
  rms->reward = reward;
  rms->num_threads = num_threads;
  rms->xdrop = xdrop;
  rms->identity = identity;
  rms->moreblast = moreblast;
  rms->flcands = flcands;
  rms->min_ali_len_perc = min_ali_len_perc;

  return gs;
}
