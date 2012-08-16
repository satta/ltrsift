/*
  Copyright (c) 2011-2012 Sascha Kastens <sascha.kastens@studium.uni-hamburg.de>
  Copyright (c) 2011-2012 Center for Bioinformatics, University of Hamburg

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

#ifndef MENUBAR_H
#define MENUBAR_H

#include "ltrsift.h"

void first_save_and_reload(GUIData *ltrgui, GtArray *nodes, GtArray *regions,
                           const gchar *projectfile);

void menubar_save_activate(GtkMenuItem *menuitem, GUIData *ltrgui);

void menubar_init(GUIData *ltrgui);

#endif
