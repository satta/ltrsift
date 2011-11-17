/*
  Copyright (c) 2011 Sascha Kastens <sascha.kastens@studium.uni-hamburg.de>
  Copyright (c) 2011 Center for Bioinformatics, University of Hamburg

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

#include "preprocess_stream.h"
#include "preprocess_visitor.h"

struct GtPreprocessStream {
  const GtNodeStream parent_instance;
  GtNodeStream *in_stream;
  GtPreprocessVisitor *pv;
};

#define gt_preprocess_stream_cast(GS)\
        gt_node_stream_cast(gt_preprocess_stream_class(), GS);

static int gt_preprocess_stream_next(GtNodeStream *gs, GtGenomeNode **gn,
                                     GtError *err)
{
  GtPreprocessStream *ps;

  int had_err = 0;

  gt_error_check(err);
  ps = gt_preprocess_stream_cast(gs);

  had_err = gt_node_stream_next(ps->in_stream, gn, err);

  if (!had_err && *gn) {
    (void) gt_genome_node_accept(*gn, (GtNodeVisitor*) ps->pv, err);
  }

  if (had_err)  {
    gt_genome_node_delete(*gn);
    *gn = NULL;
  }
  return had_err;
}

static void gt_preprocess_stream_free(GtNodeStream *gs)
{
  GtPreprocessStream *ps = gt_preprocess_stream_cast(gs);
  gt_node_visitor_delete((GtNodeVisitor*) ps->pv);
  gt_node_stream_delete(ps->in_stream);
}

const GtNodeStreamClass* gt_preprocess_stream_class(void)
{
  static const GtNodeStreamClass *gsc = NULL;
  if (!gsc)
    gsc = gt_node_stream_class_new(sizeof (GtPreprocessStream),
                                   gt_preprocess_stream_free,
                                   gt_preprocess_stream_next);
  return gsc;
}

GtNodeStream* gt_preprocess_stream_new(GtNodeStream *in_stream,
                                       GtHashmap *features,
                                       unsigned long *num,
                                       bool all_features,
                                       GtError *err)
{
  GtNodeStream *gs;
  GtPreprocessStream *ps;
  gs = gt_node_stream_create(gt_preprocess_stream_class(), true);
  ps = gt_preprocess_stream_cast(gs);
  ps->in_stream = gt_node_stream_ref(in_stream);
  ps->pv = (GtPreprocessVisitor*) gt_preprocess_visitor_new(features, num,
                                                            all_features, err);
  return gs;
}
