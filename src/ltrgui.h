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

#ifndef LTRGUI_H
#define LTRGUI_H

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

#define LTR_GET_OBJECT(builder, name, type, data) \
  data->name = type(gtk_builder_get_object(builder, #name))
#define LTR_GET_WIDGET(builder, name, data) \
  LTR_GET_OBJECT(builder, name, GTK_WIDGET, data)

#define GUI_FILE            "ltrgui.glade"
#define GUI_RECENT_GROUP    "ltrgui"
#define GUI_DIALOG_OPEN     "Open file..."
#define GUI_DIALOG_IMPORT   "Import GFF3 file..."
#define GUI_DIALOG_SAVE_AS  "Save file as..."

/* Information used by gtk_about_dialog */
#define GUI_VERSION         "0.0.1"
#define GUI_COPYRIGHT       "(c) 2011-2012 Sascha Kastens"
#define GUI_COMMENTS        "COMMENT PLACEHOLDER"
#define GUI_LICENSE         "LICENSE PLACEHOLDER"
#define GUI_WEBSITE         "http://www.genometools.org"
#define GUI_LOGO            "../logo.png"

#define UNTITLED_PROJECT "Untitled project"

#define NO_DATA_DIALOG          "No data for export selected!"
#define UNSAVED_CHANGES_DIALOG  "The project data has been modified.\nDo you "\
                                "want to save your changes or discard them?"
#define NO_PROJECT_DIALOG       "The data has not been saved as a project yet."\
                                "\n Do you want to save the data as a project "\
                                "or discard them?"

typedef struct _GUIData GUIData;

struct _GUIData
{
  GtkWidget *mb_main;
  GtkWidget *mb_main_file_new;
  GtkWidget *mb_main_file_open;
  GtkWidget *mb_main_file_open_recent;
  GtkWidget *mb_main_file_save;
  GtkWidget *mb_main_file_save_as;
  GtkWidget *mb_main_file_import;
  GtkWidget *mb_main_file_close;
  GtkWidget *mb_main_file_quit;
  GtkWidget *mb_main_file_export_gff3;
  GtkWidget *mb_main_file_export_fasta;
  GtkWidget *mb_main_project_settings;
  GtkWidget *mb_main_project_filter;
  GtkWidget *mb_main_view_columns;
  GtkWidget *vbox1_main;
  GtkWidget *ltrfams;
  GtkWidget *ltrfilt;
  GtkWidget *projset;
  GtkWidget *progressbar;
  GtkWidget *sb_main;
  GtkWidget *main_window;
  GtkWidget *assistant;
  guint sb_main_context_id;
  GError *err;
};

#endif
