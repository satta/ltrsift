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

#ifndef LTRSIFT_H
#define LTRSIFT_H

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <gdk/gdkkeysyms.h>
#include <string.h>
#include "genometools.h"
#include "gtk_blastn_params.h"
#include "gtk_blastn_params_refseq.h"
#include "gtk_label_close.h"
#include "gtk_ltr_assistant.h"
#include "gtk_ltr_families.h"
#include "gtk_ltr_filter.h"
#include "gtk_project_settings.h"
#include "preprocess_stream.h"
#include "script_filter_stream.h"

typedef struct _GUIData GUIData;

struct _GUIData
{
  GtkWidget *menubar;
  GtkWidget *menubar_save;
  GtkWidget *menubar_save_as;
  GtkWidget *menubar_export;
  GtkWidget *menubar_close;
  GtkWidget *menubar_project;
  GtkWidget *menubar_view_columns;
  GtkWidget *vbox;
  GtkWidget *ltrfams;
  GtkWidget *ltrfilt;
  GtkWidget *projset;
  GtkWidget *progressbar;
  GtkWidget *statusbar;
  GtkWidget *main_window;
  GtkWidget *assistant;
  guint statusbar_context_id;
  gchar *style_file;
  GtError *err;
  GtHashmap *refseq_paramsets;
};

gint apply_gui_settings(GUIData *ltrgui);
gint save_gui_settings(GUIData *ltrgui);

#endif
