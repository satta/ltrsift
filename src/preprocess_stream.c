/*
  Copyright (c) 2011 Sascha Kastens <sascha.kastens@studium.uni-hamburg.de>
  Copyright (c) 2011 Center for Bioinformatics, University of Hamburg

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "preprocess_stream.h"
#include "preprocess_visitor.h"

struct LTRGuiPreprocessStream {
  const GtNodeStream parent_instance;
  GtNodeStream *in_stream;
  LTRGuiPreprocessVisitor *pv;
};

#define ltrgui_preprocess_stream_cast(GS)\
        gt_node_stream_cast(ltrgui_preprocess_stream_class(), GS);

static int ltrgui_preprocess_stream_next(GtNodeStream *gs, GtGenomeNode **gn,
                                         GtError *err)
{
  LTRGuiPreprocessStream *ps;

  int had_err = 0;

  gt_error_check(err);
  ps = ltrgui_preprocess_stream_cast(gs);

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

static void ltrgui_preprocess_stream_free(GtNodeStream *gs)
{
  LTRGuiPreprocessStream *ps = ltrgui_preprocess_stream_cast(gs);
  gt_node_visitor_delete((GtNodeVisitor*) ps->pv);
  gt_node_stream_delete(ps->in_stream);
}

const GtNodeStreamClass* ltrgui_preprocess_stream_class(void)
{
  static const GtNodeStreamClass *gsc = NULL;
  if (!gsc)
    gsc = gt_node_stream_class_new(sizeof (LTRGuiPreprocessStream),
                                   ltrgui_preprocess_stream_free,
                                   ltrgui_preprocess_stream_next);
  return gsc;
}

GtNodeStream* ltrgui_preprocess_stream_new(GtNodeStream *in_stream,
                                           GtHashmap *features,
                                           unsigned long *num,
                                           bool all_features, GtError *err)
{
  GtNodeStream *gs;
  LTRGuiPreprocessStream *ps;
  gs = gt_node_stream_create(ltrgui_preprocess_stream_class(), false);
  ps = ltrgui_preprocess_stream_cast(gs);
  ps->in_stream = gt_node_stream_ref(in_stream);
  ps->pv = (LTRGuiPreprocessVisitor*) ltrgui_preprocess_visitor_new(features,
                                                                   num,
                                                                   all_features,
                                                                   err);
  return gs;
}

GtArray* ltrgui_preprocess_stream_get_region_nodes(LTRGuiPreprocessStream *ps)
{
  gt_assert(ps && ps->pv);
  return ltrgui_preprocess_visitor_get_region_nodes(ps->pv);
}
