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

#ifndef PREPROCESS_VISITOR_H
#define PREPROCESS_VISITOR_H

#include "genometools.h"

#define ATTR_CLUSTID "clid"
#define ATTR_PFAMN   "name"
#define FNT_PROTEINM "protein_match"
#define FNT_LTRRETRO "LTR_retrotransposon"
#define FNT_LTR      "long_terminal_repeat"
#define FNT_LLTR     "lLTR"
#define FNT_RLTR     "rLTR"

typedef struct LTRGuiPreprocessVisitor LTRGuiPreprocessVisitor;

const GtNodeVisitorClass* ltrgui_preprocess_visitor_class(void);

GtNodeVisitor* ltrgui_preprocess_visitor_new(GtHashmap *features,
                                             unsigned long *num,
                                             bool all_features,
                                             GtError *err);

#define ltrgui_preprocess_visitor_cast(GV)\
        gt_node_visitor_cast(ltrgui_preprocess_visitor_class(), GV)

#define ltrgui_preprocess_visitor_try_cast(GV)\
        gt_node_visitor_try_cast(ltrgui_preprocess_visitor_class(), GV)

#endif
