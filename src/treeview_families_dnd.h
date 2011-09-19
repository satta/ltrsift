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

#ifndef TREEVIEW_FAMILIES_DND_H
#define TREEVIEW_FAMILIES_DND_H

#include "ltrgui.h"

typedef struct _FamilyTransferData FamilyTransferData;

struct _FamilyTransferData
{
  gchar *row;
  GtGenomeNode **gn;
  GtHashmap *feat;
};

typedef enum {
  TARGET_STRING = 0
} FamilyTargets;

static const GtkTargetEntry family_drag_targets[] = {
  {"STRING", 0, TARGET_STRING}
};

void ltrfam_lv_on_drag_data_get(GtkWidget *widget,
                                GdkDragContext *drag_context,
                                GtkSelectionData *sdata,
                                guint info,
                                guint time,
                                gpointer user_data);

void tv_families_on_drag_data_received(GtkWidget *widget,
                                       GdkDragContext *drag_context,
                                       gint x,
                                       gint y,
                                       GtkSelectionData *sdata,
                                       guint info,
                                       guint time,
                                       gpointer user_data);

gboolean tv_families_on_drag_motion(GtkWidget *widget,
                                    GdkDragContext *context,
                                    gint x,
                                    gint y,
                                    guint time,
                                    gpointer user_data);

#endif /* TREEVIEW_FAMILIES_DND_H */
