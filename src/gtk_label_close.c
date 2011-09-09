#include "gtk_label_close.h"
#include "unused.h"

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
      NULL, /* (GClassInitFunc) gtk_label_close_class_init, */
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
                               gpointer user_data,
                               gboolean hide_close)
{
  GtkLabelClose *labelclose;
  labelclose = gtk_type_new(GTKLABELCLOSE_TYPE);
  gtk_label_set_text(GTK_LABEL(labelclose->label), text);
  if (hide_close)
    gtk_widget_hide(labelclose->button);
  else
    g_signal_connect(G_OBJECT(labelclose->button), "clicked",
                     G_CALLBACK(signal_func), user_data);
  return GTK_WIDGET(labelclose);
}
