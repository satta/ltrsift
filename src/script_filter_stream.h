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

#ifndef SCRIPT_FILTER_STREAM_H
#define SCRIPT_FILTER_STREAM_H

#include "genometools.h"

typedef struct LTRGuiScriptFilterStream LTRGuiScriptFilterStream;

const GtNodeStreamClass* ltrgui_script_filter_stream_class(void);

GtNodeStream* ltrgui_script_filter_stream_new(GtNodeStream *in_stream,
                                              GtStrArray *filter_files,
                                              GtBittab *negate,
                                              int logic,
                                              GtError *err);

#endif
