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

#define LTR_FILTER_TEMPLATE "name        = \"Name of the script here\"\n" \
                            "author      = \"Your name here\"\n" \
                            "version     = \"Script version here\"\n" \
                            "email       = \"Your email here\"\n" \
                            "short_descr = \"Short description here\"\n" \
                            "description = \"Script desription here\"\n\n" \
                            "function filter(gn)\n"\
                            "  return true -- return false\n" \
                            "end"

#define LTR_FILTER_NOT_ADDED_FILES "Some files could not be added!\n" \
                                   "Possible reasons: Missing metadata, " \
                                   "missing function 'filter' or syntax " \
                                   "errors."

#define LTR_FILTER_NOT_SAVED_FILE  "File could not be saved!\n" \
                                   "Possible reasons: Missing metadata, " \
                                   "missing function 'filter' or syntax " \
                                   "errors."

#define LTR_FILTER_UNSAVED_CHANGES "You have unsaved changes. Are you sure " \
                                   "you want to close the window?"

#define LUA_PATTERN        ".lua"
#define LUA_FILTER_PATTERN "*.lua"

#define LTR_FILTER_ACTION_DELETE_TEXT  "Unclassify/Delete"
#define LTR_FILTER_ACTION_NEW_FAM_TEXT "Create new family"

enum {
  LTR_FILTER_ACTION_DELETE = 0,
  LTR_FILTER_ACTION_NEW_FAM
};

typedef struct _GtkLTRFilter GtkLTRFilter;
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
  GtkWidget *ltrfams;
  GtkTextBuffer *text_buffer;
  GtScriptFilter *script_filter;
  gchar *last_dir;
  gchar *cur_filename;
  GError *gerr;
};

struct _GtkLTRFilterClass
{
  GtkWindowClass parent_class;
  void (* gtk_ltr_filter) (GtkLTRFilter *ltrfilt);
};

/*
enum {
  LTR_FILT_MOVE_DOWN = 0,
  LTR_FILT_MOVE_UP
};
*/

enum {
  LTR_FILTER_LV_FILE = 0,
  LTR_FILTER_LV_SEL_NOT,
  LTR_FILTER_LV_SEL_N_COLUMNS
};

GType      gtk_ltr_filter_get_type(void);

GtkWidget* gtk_ltr_filter_new(GtkWidget *ltrfams);

void       gtk_ltr_filter_set_ltrfams(GtkLTRFilter *ltrfilt,
                                      GtkWidget *ltrfams);

#endif
