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

#ifndef GTK_LTR_FILTER_H
#define GTK_LTR_FILTER_H

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include "genometools.h"

#define GTK_LTR_FILTER_TYPE\
        gtk_ltr_filter_get_type()
#define GTK_LTR_FILTER(obj)\
        G_TYPE_CHECK_INSTANCE_CAST((obj), GTK_LTR_FILTER_TYPE, GtkLTRFilter)
#define GTK_LTR_FILTER_CLASS(klass)\
        G_TYPE_CHECK_CLASS_CAST((klass), GTK_LTR_FILTER_TYPE, GtkLTRFilterClass)
#define IS_GTK_LTR_FILTER(obj)\
        G_TYPE_CHECK_INSTANCE_TYPE((obj), GTK_LTR_FILTER_TYPE)
#define IS_GTK_LTR_FILTER_CLASS(klass)\
        G_TYPE_CHECK_CLASS_TYPE((klass), GTK_LTR_FILTER_TYPE)

typedef struct _GtkLTRFilter      GtkLTRFilter;
typedef struct _GtkLTRFilterClass GtkLTRFilterClass;

struct _GtkLTRFilter
{
  GtkWindow window;
  GtkWidget *label_descr;
  GtkWidget *label_author;
  GtkWidget *label_email;
  GtkWidget *list_view_all;
  GtkWidget *list_view_sel;
  GtkWidget *edit_dialog;
  GtkWidget *filter_action;
  GtkWidget *filter_logic;
  GtkWidget *apply;
  GtkWidget *ltrfams;
  GtkTextBuffer *text_buffer;
  GtScriptFilter *script_filter;
  gchar *last_dir;
  gchar *cur_filename;
  gint range;
};

struct _GtkLTRFilterClass
{
  GtkWindowClass parent_class;
  void (* gtk_ltr_filter) (GtkLTRFilter *ltrfilt);
};

enum {
  LTR_FILTER_ACTION_DELETE = 0,
  LTR_FILTER_ACTION_NEW_FAM
};

enum {
  LTR_FILTER_LOGIC_AND = 0,
  LTR_FILTER_LOGIC_OR
};

enum {
  LTR_FILTER_RANGE_PROJECT = 0,
  LTR_FILTER_RANGE_FAMILIES,
  LTR_FILTER_RANGE_CANDIDATES
};

enum {
  LTR_FILTER_LV_FILE = 0,
  LTR_FILTER_LV_SEL_NOT,
  LTR_FILTER_LV_SEL_N_COLUMNS
};

GType      gtk_ltr_filter_get_type(void);

void       gtk_ltr_filter_set_ltrfams(GtkLTRFilter *ltrfilt,
                                      GtkWidget *ltrfams);

gint       gtk_ltr_filter_get_filter_files_from_sql(GtkLTRFilter *ltrfilt,
                                                    GtError *err);

void       gtk_ltr_filter_set_range(GtkLTRFilter *ltrfilt, gint range);

void       gtk_ltr_filter_set_apply_text(GtkLTRFilter *ltrfilt,
                                         const gchar *text);

gint       gtk_ltr_filter_save_data(GtkLTRFilter *ltrfilt, GtError *err);

GtkWidget* gtk_ltr_filter_new(GtkWidget *ltrfams);

#endif
