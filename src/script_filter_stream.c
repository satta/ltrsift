/*
  Copyright (c) 2012 Sascha Kastens <sascha.kastens@studium.uni-hamburg.de>
  Copyright (c) 2012 Center for Bioinformatics, University of Hamburg

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

#include "script_filter_stream.h"

enum {
  SCRIPT_FILTER_AND = 0,
  SCRIPT_FILTER_OR
};

struct LTRGuiScriptFilterStream {
  const GtNodeStream parent_instance;
  GtNodeStream *in_stream;
  GtArray *nodes,
          *filtered_nodes,
          *script_filters;
  GtBittab *negate;
  int logic;
  bool first_next;
  unsigned long next_index;
};

#define ltrgui_script_filter_stream_cast(GS)\
        gt_node_stream_cast(ltrgui_script_filter_stream_class(), GS);

static int filter_nodes_lua(GtArray *filters, GtFeatureNode *fn,
                            GtBittab *negate, int logic, bool *select_node,
                            GtError *err)
{
  int had_err = 0;
  unsigned long i;
  gt_assert(filters && fn && select_node);
  gt_error_check(err);

  for (i = 0; !had_err && i < gt_array_size(filters); i++) {
    bool result;
    GtScriptFilter *sf = *(GtScriptFilter**) gt_array_get(filters, i);
    had_err = gt_script_filter_run(sf, fn, &result, err);

    if (!had_err) {
      if (gt_bittab_bit_is_set(negate, i))
        result = !result;
      if (i == 0)
        *select_node = result;
      else if (logic == SCRIPT_FILTER_AND) {
        *select_node = *select_node || result;
        if (*select_node) {
          break;
        }
      } else if (logic == SCRIPT_FILTER_OR) {
        *select_node = *select_node && result;
        if (!*select_node) {
          break;
        }
      }
    }
  }
  return had_err;
}

static int filter_nodes(LTRGuiScriptFilterStream *sfs, GtError *err)
{
  GtGenomeNode *gn;
  int had_err = 0;
  unsigned long i;
  bool select_node = false;

  for (i = 0; i < gt_array_size(sfs->nodes); i++) {
    gn = *(GtGenomeNode**) gt_array_get(sfs->nodes, i);
    had_err = filter_nodes_lua(sfs->script_filters, (GtFeatureNode*) gn,
                               sfs->negate, sfs->logic, &select_node, err);
    if (!had_err && select_node)
      gt_array_add(sfs->filtered_nodes, gn);
  }
  return had_err;
}

static int ltrgui_script_filter_stream_next(GtNodeStream *gs, GtGenomeNode **gn,
                                            GtError *err)
{
  LTRGuiScriptFilterStream *sfs;
  GtGenomeNode *ref_gn;
  int had_err = 0;

  gt_error_check(err);
  sfs = ltrgui_script_filter_stream_cast(gs);

  if (sfs->first_next) {
    while (!(had_err = gt_node_stream_next(sfs->in_stream, gn, err)) && *gn) {
      gt_assert(*gn && !had_err);
      ref_gn = gt_genome_node_ref(*gn);
      gt_array_add(sfs->nodes, ref_gn);
    }
    if (!had_err)
      had_err = filter_nodes(sfs, err);
    if (!had_err) {
      if (gt_array_size(sfs->filtered_nodes) == 0)
        *gn = NULL;
      else {
        *gn = *(GtGenomeNode**) gt_array_get(sfs->filtered_nodes,
                                             sfs->next_index);
        sfs->next_index++;
        sfs->first_next = false;
      }
      return 0;
    }
  } else {
    if (sfs->next_index >= gt_array_size(sfs->filtered_nodes))
      *gn = NULL;
    else {
      *gn = *(GtGenomeNode**) gt_array_get(sfs->filtered_nodes,
                                           sfs->next_index);
      sfs->next_index++;
    }
    return 0;
  }
  return had_err;
}

static void ltrgui_script_filter_stream_free(GtNodeStream *gs)
{
  LTRGuiScriptFilterStream *sfs = ltrgui_script_filter_stream_cast(gs);
  unsigned long i;
  for (i = 0; i < gt_array_size(sfs->nodes); i++)
    gt_genome_node_delete(*(GtGenomeNode**) gt_array_get(sfs->nodes, i));
  gt_array_delete(sfs->nodes);
  gt_array_delete(sfs->filtered_nodes);
  gt_node_stream_delete(sfs->in_stream);
}

const GtNodeStreamClass* ltrgui_script_filter_stream_class(void)
{
  static const GtNodeStreamClass *gsc = NULL;
  if (!gsc)
    gsc = gt_node_stream_class_new(sizeof (LTRGuiScriptFilterStream),
                                   ltrgui_script_filter_stream_free,
                                   ltrgui_script_filter_stream_next);
  return gsc;
}

GtNodeStream* ltrgui_script_filter_stream_new(GtNodeStream *in_stream,
                                              GtStrArray *filter_files,
                                              GtBittab *negate,
                                              int logic,
                                              GtError *err)
{
  GtNodeStream *gs;
  LTRGuiScriptFilterStream *sfs;
  gs = gt_node_stream_create(ltrgui_script_filter_stream_class(), false);
  sfs = ltrgui_script_filter_stream_cast(gs);
  sfs->in_stream = gt_node_stream_ref(in_stream);
  sfs->nodes = gt_array_new(sizeof(GtGenomeNode*));
  sfs->filtered_nodes = gt_array_new(sizeof(GtGenomeNode*));
  sfs->logic = logic;
  sfs->first_next = true;
  sfs->next_index = 0;
  sfs->negate = negate;

  int i, j;
  sfs->script_filters = gt_array_new(sizeof (GtScriptFilter*));
  for (i = 0; i < gt_str_array_size(filter_files); i++) {
    GtScriptFilter *sf;
    sf = gt_script_filter_new(gt_str_array_get(filter_files, i), err);
    if (!sf) {
      for (j = 0; j < gt_array_size(sfs->script_filters); j++) {
        GtScriptFilter *todelete =
          *(GtScriptFilter**) gt_array_get(sfs->script_filters, j);
        gt_script_filter_delete(todelete);
      }
      return NULL;
      break;
    } else {
      gt_array_add(sfs->script_filters, sf);
    }
  }
  return gs;
}
