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

/* Implements the <GtNodeVisitor> interface. <LTRGuiPreprocessVisitor>
   traverses each <GtFeatureNode> and looks for features with given cluster id
   (if <all_features> is set to false). Features will be stored in <features>
   which will be used by LTRGui. If <all_features> is set to true, features
   without cluster id will be stored in <features>. <num> will be the total
   amount of unique features found. */
GtNodeVisitor* ltrgui_preprocess_visitor_new(GtHashmap *features,
                                             unsigned long *num,
                                             bool all_features,
                                             GtError *err);

GtArray*       ltrgui_preprocess_visitor_get_region_nodes(
                                                    LTRGuiPreprocessVisitor *v);

#define ltrgui_preprocess_visitor_cast(GV)\
        gt_node_visitor_cast(ltrgui_preprocess_visitor_class(), GV)

#define ltrgui_preprocess_visitor_try_cast(GV)\
        gt_node_visitor_try_cast(ltrgui_preprocess_visitor_class(), GV)

#endif
