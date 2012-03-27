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

#include "gtk_label_close.h"

void gtk_label_close_hide_close(GtkLabelClose *labelclose)
{
  gtk_widget_hide(labelclose->button);
}

void gtk_label_close_show_close(GtkLabelClose *labelclose)
{
  gtk_widget_show(labelclose->button);
}

const gchar* gtk_label_close_get_text(GtkLabelClose *labelclose)
{
  return gtk_label_get_text(GTK_LABEL(labelclose->label));
}

void gtk_label_close_set_text(GtkLabelClose *labelclose, gchar *text)
{
  gtk_label_set_text(GTK_LABEL(labelclose->label), text);
}

void gtk_label_close_set_button_data(GtkLabelClose *labelclose,
                                     const char *type,
                                     gpointer data)
{
  g_object_set_data(G_OBJECT(labelclose->button), type, data);
}

gpointer gtk_label_close_get_button_data(GtkLabelClose *labelclose,
                                         const char *type)
{
  return g_object_get_data(G_OBJECT(labelclose->button), type);
}

static void gtk_label_close_init(GtkLabelClose *labelclose)
{
  GtkWidget *image;

  gtk_box_set_homogeneous(GTK_BOX(labelclose), FALSE);
  gtk_box_set_spacing(GTK_BOX(labelclose), 3);

  labelclose->label = gtk_label_new("");
  gtk_box_pack_start(GTK_BOX(labelclose), labelclose->label,
                     TRUE, TRUE, 0);
  labelclose->button = gtk_button_new();
  gtk_box_pack_start(GTK_BOX(labelclose), labelclose->button,
                     FALSE, FALSE, 0);
  gtk_widget_set_size_request(labelclose->button, 20, 18);
  image = gtk_image_new_from_stock(GTK_STOCK_CLOSE, GTK_ICON_SIZE_MENU);
  gtk_container_add(GTK_CONTAINER(labelclose->button), image);
  gtk_widget_show_all(GTK_WIDGET(labelclose));
}

GType gtk_label_close_get_type(void)
{
  static GType labelclose_type = 0;

  if (!labelclose_type) {
    const GTypeInfo labelclose_info =
    {
      sizeof (GtkLabelCloseClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      NULL, /*(GClassInitFunc) gtk_label_close_class_init,*/
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (GtkLabelClose),
      0,    /* n_preallocs */
      (GInstanceInitFunc) gtk_label_close_init,
    };
    labelclose_type = g_type_register_static(GTK_TYPE_HBOX, "GtkLabelClose",
                                             &labelclose_info, 0);
  }
  return labelclose_type;
}

GtkWidget* gtk_label_close_new(const gchar *text,
                               GCallback signal_func,
                               gpointer user_data)
{
  GtkLabelClose *labelclose;
  labelclose = gtk_type_new(GTK_LABEL_CLOSE_TYPE);
  gtk_label_set_text(GTK_LABEL(labelclose->label), text);
  if (signal_func)
    g_signal_connect(G_OBJECT(labelclose->button), "clicked",
                     G_CALLBACK(signal_func), user_data);
  return GTK_WIDGET(labelclose);
}
