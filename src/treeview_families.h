/*
  Copyright (c) 2011-2012 Sascha Kastens <sascha.kastens@studium.uni-hamburg.de>
  Copyright (c) 2011-2012 Center for Bioinformatics, University of Hamburg

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

#ifndef TREEVIEW_FAMILIES_H
#define TREEVIEW_FAMILIES_H

#include "ltrgui.h"

typedef enum {
  TV_FAM_NODE_ARRAY = 0,
  TV_FAM_TAB_CHILD,
  TV_FAM_TAB_LABEL,
  TV_FAM_CURNAME,
  TV_FAM_OLDNAME,
  TV_FAM_N_COLUMS
} TreeViewFamiliesColumns;

void tv_families_init(GUIData *ltrgui);

void tv_families_add_family_button_clicked(GtkWidget *button, GUIData *ltrgui);

void remove_node_from_array(GtArray *nodes, GtGenomeNode **gn);

gboolean ltrfam_lv_button_pressed(GtkWidget *treeview, GdkEventButton *event,
                                    GUIData *ltrgui);

#endif /* TREEVIEW_FAMILIES_H */
