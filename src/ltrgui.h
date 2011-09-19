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

#ifndef GUI_TEST_H
#define GUI_TEST_H

#include <gtk/gtk.h>
#include "genometools.h"
#include "gtk_label_close.h"
#include "gtk_ltr_family.h"

#define LTR_GET_OBJECT(builder, name, type, data) \
  data->name = type(gtk_builder_get_object(builder, #name))
#define LTR_GET_WIDGET(builder, name, data) \
  LTR_GET_OBJECT(builder, name, GTK_WIDGET, data)

#define GUI_FILE            "ltrgui.glade"
#define GUI_DIALOG_OPEN     "Open file..."
#define GUI_DIALOG_SAVE_AS  "Save file as"

/* Information used by gtk_about_dialog */
#define GUI_NAME            "LTRGui"
#define GUI_VERSION         "0.0.1"
#define GUI_COPYRIGHT       "(c) 2011-2012 Sascha Kastens"
#define GUI_COMMENTS        "COMMENT PLACEHOLDER"
#define GUI_LICENSE         "LICENSE PLACEHOLDER"
#define GUI_WEBSITE         "http://www.genometools.org"
#define GUI_LOGO            "../logo.png"

typedef struct _GUIData GUIData;

struct _GUIData
{
  GtkWidget *mb_main;
  GtkWidget *mb_main_project_new;
  GtkWidget *mb_main_project_open;
  GtkWidget *mb_main_project_save;
  GtkWidget *mb_main_project_save_as;
  GtkWidget *mb_main_project_quit;
  GtkWidget *tv_families;
  GtkWidget *hbox1_main;
  GtkWidget *nb_families;
  GtkWidget *sb_main;
  GtkWidget *main_window;
  GtkWidget *pw_window;
  GtkWidget *pw_treeview_gff3;
  GtkWidget *pw_do_classification_cb;
  GtkWidget *pw_label_projectname;
  GtkWidget *pw_label_encseq;
  GtkWidget *sw_main;
  guint sb_main_context_id;
  GSList *project_files;
  gchar *project_filename;
  GtHashmap *features;
  unsigned long n_features;
};

gboolean init_gui(GUIData *ltrgui, GError **err);

#endif /* GUI_TEST_H */
