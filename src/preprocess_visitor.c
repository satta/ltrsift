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

#include <string.h>
#include "preprocess_visitor.h"

struct LTRGuiPreprocessVisitor {
  const GtNodeVisitor parent_instance;
  GtHashmap *features;
  GtArray *regions;
  unsigned long *num;
  bool all_features;
};

void ltrgui_preprocess_visitor_free(GtNodeVisitor *nv)
{
  LTRGuiPreprocessVisitor *pv;
  pv = ltrgui_preprocess_visitor_cast(nv);
  gt_assert(pv);
  gt_array_delete(pv->regions);
}

static int ltrgui_preprocess_visitor_feature_node(GtNodeVisitor *gv,
                                                  GtFeatureNode *fn,
                                                  GtError *err)
{
  LTRGuiPreprocessVisitor *pv;
  const char *fnt = NULL, *clid = NULL, *attr = NULL;
  GtFeatureNode *curnode = NULL;
  GtFeatureNodeIterator *fni;
  unsigned long num;
  int had_err = 0;
  bool first_ltr = true,
       in_ltrretro = false;
  pv = ltrgui_preprocess_visitor_cast(gv);
  gt_assert(pv);
  gt_error_check(err);

  fni = gt_feature_node_iterator_new(fn);
  while (!had_err && (curnode = gt_feature_node_iterator_next(fni))) {
    fnt = gt_feature_node_get_type(curnode);
    if (strcmp(fnt, FNT_LTRRETRO) == 0) {
      in_ltrretro = true;
      continue;
    }
    if (in_ltrretro && (pv->all_features ||
                        (clid = gt_feature_node_get_attribute(curnode,
                                                              ATTR_CLUSTID)))) {
      num = *pv->num;
      if (strcmp(fnt, FNT_PROTEINM) == 0) {
        attr = gt_feature_node_get_attribute(curnode, ATTR_PFAMN);
        if (!attr)
          continue;
        if (!gt_hashmap_get(pv->features, (void*) attr)) {
          gt_hashmap_add(pv->features, (void*) gt_cstr_dup(attr), (void*) num);
          *pv->num = *pv->num + 1;
        }
      } else if (strcmp(fnt, FNT_LTR) == 0) {
        char *tmp;
        if (first_ltr) {
          tmp = gt_cstr_dup("lLTR");
          first_ltr = false;
        } else {
          tmp = gt_cstr_dup("rLTR");
          in_ltrretro = false;
        }
        if (!gt_hashmap_get(pv->features, (void*) tmp)) {
          gt_hashmap_add(pv->features, (void*) gt_cstr_dup(tmp), (void*) num);
          *pv->num = *pv->num + 1;
          gt_free(tmp);
        } else
          gt_free(tmp);
      } else {
        if (!gt_hashmap_get(pv->features, (void*) fnt)) {
          gt_hashmap_add(pv->features, (void*) gt_cstr_dup(fnt), (void*) num);
          *pv->num = *pv->num + 1;
        }
      }
    }
  }

  gt_feature_node_iterator_delete(fni);
  return had_err;
}

static int ltrgui_preprocess_visitor_region_node(GtNodeVisitor *gv,
                                                 GtRegionNode *rn,
                                                 GtError *err)
{
  LTRGuiPreprocessVisitor *pv;
  int had_err = 0;
  GtGenomeNode *gn;
  pv = ltrgui_preprocess_visitor_cast(gv);
  gt_assert(pv);
  gt_error_check(err);

  gn = gt_genome_node_ref((GtGenomeNode*) rn);
  gt_array_add(pv->regions, gn);
  return had_err;
}

const GtNodeVisitorClass* ltrgui_preprocess_visitor_class(void)
{
  static const GtNodeVisitorClass *gvc = NULL;
  if (!gvc) {
    gvc = gt_node_visitor_class_new(sizeof (LTRGuiPreprocessVisitor),
                                    ltrgui_preprocess_visitor_free,
                                    NULL,
                                    ltrgui_preprocess_visitor_feature_node,
                                    ltrgui_preprocess_visitor_region_node,
                                    NULL,
                                    NULL);
  }
  return gvc;
}

GtNodeVisitor* ltrgui_preprocess_visitor_new(GtHashmap *features,
                                             unsigned long *num,
                                             bool all_features,
                                             GT_UNUSED GtError *err)
{
  GtNodeVisitor *nv;
  LTRGuiPreprocessVisitor *pv;
  nv = gt_node_visitor_create(ltrgui_preprocess_visitor_class());
  pv = ltrgui_preprocess_visitor_cast(nv);
  gt_assert(pv);
  pv->features = features;
  pv->regions = gt_array_new(sizeof (GtRegionNode*));
  pv->num = num;
  pv->all_features = all_features;
  return nv;
}

GtArray* ltrgui_preprocess_visitor_get_region_nodes(LTRGuiPreprocessVisitor *v)
{
  gt_assert(v);
  return gt_array_ref(v->regions);
}
