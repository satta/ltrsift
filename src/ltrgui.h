#ifndef GUI_TEST_H
#define GUI_TEST_H

#include <gtk/gtk.h>

#define LTR_GET_OBJECT(builder, name, type, data) \
  data->name = type(gtk_builder_get_object(builder, #name))
#define LTR_GET_WIDGET(builder, name, data) \
  LTR_GET_OBJECT(builder, name, GTK_WIDGET, data)

#define GUI_FILE "gui/ltrgui.glade"

typedef struct _LTRData LTRData;
struct _LTRData
{
  GtkWidget *mb_main;
  GtkWidget *mb_main_project_new;
  GtkWidget *mb_main_project_open;
  GtkWidget *mb_main_project_save;
  GtkWidget *mb_main_project_save_as;
  GtkWidget *mb_main_project_quit;
  GtkWidget *sb_main;
  GtkWidget *window_main;
  GtkWidget *assistant_project;
  GtkWidget *assistant_project_treeview;
  GtkWidget *assistant_project_class_cb;
  guint sb_main_context_id;
  GSList *project_files;
  gchar *project_filename;
};

#endif // GUI_TEST_H
