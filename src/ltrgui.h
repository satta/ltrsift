#ifndef GUI_TEST_H
#define GUI_TEST_H

#include <gtk/gtk.h>

#define LTR_GET_OBJECT(builder, name, type, data) \
  data->name = type(gtk_builder_get_object(builder, #name))
#define LTR_GET_WIDGET(builder, name, data) \
  LTR_GET_OBJECT(builder, name, GTK_WIDGET, data)

#define GUI_FILE "gui/ltr_gui.glade"

typedef struct _LTRData LTRData;
struct _LTRData
{
  GtkWidget *menubar_main;
  GtkWidget *menubar_main_project_new;
  GtkWidget *menubar_main_project_open;
  GtkWidget *menubar_main_project_save;
  GtkWidget *menubar_main_project_save_as;
  GtkWidget *menubar_main_project_quit;
  GtkWidget *statusbar_main;
  GtkWidget *window_main;
  guint statusbar_main_context_id;
};

#endif // GUI_TEST_H
