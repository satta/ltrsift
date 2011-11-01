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

#ifndef MENUBAR_MAIN_H
#define MENUBAR_MAIN_H

#include "ltrgui.h"

typedef struct _MenuThreadData MenuThreadData;

struct _MenuThreadData
{
  GUIData *ltrgui;
  GtkWidget *window;
  GtkWidget *progressbar;
  GtError *err;
  gchar *filename;
  gchar *tmp_filename;
  GtArray *nodes;
  GtHashmap *features;
  gboolean save_as;
  gboolean open;
  gboolean bakfile;
  unsigned long progress,
                n_features;
  int had_err;
};

void mb_main_init(GUIData *ltrgui);

void mb_main_file_save_activate(GtkMenuItem *menuitem, GUIData *ltrgui);

void mb_main_file_save_as_activate(GtkMenuItem *menuitem, GUIData *ltrgui);

void mb_main_file_import_activate(GtkMenuItem *menuitem, GUIData *ltrgui);

void mb_main_file_new_activate(GtkMenuItem *menuitem, GUIData *ltrgui);

void mb_main_file_open_activate(GtkMenuItem *menuitem, GUIData *ltrgui);

void mb_main_help_about_activate(GtkMenuItem *menutiem, GUIData *ltrgui);

void mb_main_view_columns_set_submenu(GUIData *ltrgui, GtHashmap *features,
                                      GtError *err, gboolean sqlt);
void mb_main_activate_menuitems(GUIData *ltrgui);

GtkWidget* unsaved_changes_dialog(GUIData *ltrgui, const gchar *text);

#endif /* MENUBAR_MAIN_H */
