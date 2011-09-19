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

#ifndef __GTK_LABEL_CLOSE_H__
#define __GTK_LABEL_CLOSE_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>

#define GTKLABELCLOSE_TYPE\
        gtk_label_close_get_type()
#define GTKLABELCLOSE(obj)\
        G_TYPE_CHECK_INSTANCE_CAST((obj), GTKLABELCLOSE_TYPE, GtkLabelClose)
#define GTKLABELCLOSE_CLASS(klass)\
        G_TYPE_CHECK_CLASS_CAST((klass), GTKLTRFAMILY_TYPE, GtkLabelCloseClass)
#define IS_GTKLABELCLOSE(obj)\
        G_TYPE_CHECK_INSTANCE_TYPE((obj), GTKLABELCLOSE_TYPE)
#define IS_GTKLABELCLOSE_CLASS(klass)\
        G_TYPE_CHECK_CLASS_TYPE((klass), GTKLABELCLOSE_TYPE)

typedef struct _GtkLabelClose GtkLabelClose;
typedef struct _GtkLabelCloseClass GtkLabelCloseClass;

struct _GtkLabelClose
{
  GtkHBox hbox;
  GtkWidget *label;
  GtkWidget *button;
};

struct _GtkLabelCloseClass
{
  GtkHBoxClass parent_class;
  void (* gtk_label_close) (GtkLabelClose *labelclose);
};

GType gtk_label_close_get_type(void);
GtkWidget* gtk_label_close_new(const gchar *text,
                               GCallback signal_func,
                               gpointer user_data);
void gtk_label_close_set_text(GtkLabelClose *labelclose, gchar *text);

const gchar* gtk_label_close_get_text(GtkLabelClose *labelclose);

void gtk_label_close_show_close(GtkLabelClose *labelclose);

void gtk_label_close_hide_close(GtkLabelClose *labelclose);

gpointer gtk_label_close_get_button_data(GtkLabelClose *labelclose,
                                         const char *type);

void gtk_label_close_set_button_data(GtkLabelClose *labelclose,
                                     const char *type,
                                     gpointer data);

#endif /* __GTK_LABEL_CLOSE_H__ */
