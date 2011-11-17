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

#include <string.h>
#include "preprocess_visitor.h"

struct GtPreprocessVisitor {
  const GtNodeVisitor parent_instance;
  GtHashmap *features;
  unsigned long *num;
  bool all_features;
};

static int gt_preprocess_visitor_feature_node(GtNodeVisitor *gv,
                                              GtFeatureNode *fn,
                                              GtError *err)
{
  GtPreprocessVisitor *pv;
  const char *fnt = NULL, *clid = NULL, *attr = NULL;
  GtFeatureNode *curnode = NULL;
  GtFeatureNodeIterator *fni;
  unsigned long num;
  int had_err = 0;
  bool first_ltr = true;

  pv = gt_preprocess_visitor_cast(gv);
  gt_assert(pv);
  gt_error_check(err);
  fni = gt_feature_node_iterator_new(fn);

  while (!had_err && (curnode = gt_feature_node_iterator_next(fni))) {
    if (pv->all_features ||
                (clid = gt_feature_node_get_attribute(curnode, ATTR_CLUSTID))) {
      fnt = gt_feature_node_get_type(curnode);
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
        } else
          tmp = gt_cstr_dup("rLTR");
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

const GtNodeVisitorClass* gt_preprocess_visitor_class(void)
{
  static const GtNodeVisitorClass *gvc = NULL;
  if (!gvc) {
    gvc = gt_node_visitor_class_new(sizeof (GtPreprocessVisitor),
                                    NULL,
                                    NULL,
                                    gt_preprocess_visitor_feature_node,
                                    NULL,
                                    NULL,
                                    NULL);
  }
  return gvc;
}

GtNodeVisitor* gt_preprocess_visitor_new(GtHashmap *features,
                                                unsigned long *num,
                                                bool all_features,
                                                GT_UNUSED GtError *err)
{
  GtNodeVisitor *nv;
  GtPreprocessVisitor *pv;
  nv = gt_node_visitor_create(gt_preprocess_visitor_class());
  pv = gt_preprocess_visitor_cast(nv);
  gt_assert(pv);
  pv->features = features;
  pv->num = num;
  pv->all_features = all_features;
  return nv;
}
