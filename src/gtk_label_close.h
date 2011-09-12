#ifndef __GTK_LABEL_CLOSE_H__
#define __GTK_LABEL_CLOSE_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>

#define GTKLABELCLOSE_TYPE            (gtk_label_close_get_type())
#define GTKLABELCLOSE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GTKLABELCLOSE_TYPE, GtkLabelClose))
#define GTKLABELCLOSE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), GTKLTRFAMILY_TYPE, GtkLabelCloseClass))
#define IS_GTKLABELCLOSE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GTKLABELCLOSE_TYPE))
#define IS_GTKLABELCLOSE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), GTKLABELCLOSE_TYPE))

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

#endif // GTK_LABEL_CLOSE_H
