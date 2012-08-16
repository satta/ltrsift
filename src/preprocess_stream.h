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

#ifndef PREPROCESS_STREAM_H
#define PREPROCESS_STREAM_H

#include "genometools.h"

typedef struct LTRGuiPreprocessStream LTRGuiPreprocessStream;

const GtNodeStreamClass* ltrgui_preprocess_stream_class(void);

/* Implements the <GtNodeStream> interface. <LTRGuiPreprocessStream> processes
   each <GtGenomeNode> via <LTRGuiPreprocessVisitor> */
GtNodeStream* ltrgui_preprocess_stream_new(GtNodeStream *in_stream,
                                           GtHashmap *features,
                                           unsigned long *num,
                                           bool all_features,
                                           GtError *err);

GtArray*      ltrgui_preprocess_stream_get_region_nodes(
                                                    LTRGuiPreprocessStream *ps);

#endif
