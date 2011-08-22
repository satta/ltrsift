#ifndef GUI_TEST_H
#define GUI_TEST_H

#include <gtk/gtk.h>
#include "genometools.h"

#define LTR_GET_OBJECT(builder, name, type, data) \
  data->name = type(gtk_builder_get_object(builder, #name))
#define LTR_GET_WIDGET(builder, name, data) \
  LTR_GET_OBJECT(builder, name, GTK_WIDGET, data)

#define GUI_FILE "ltrgui.glade"

typedef struct _GUIData GUIData;
struct _GUIData
{
  GtkWidget *mb_main;
  GtkWidget *mb_main_project_new;
  GtkWidget *mb_main_project_open;
  GtkWidget *mb_main_project_save;
  GtkWidget *mb_main_project_save_as;
  GtkWidget *mb_main_project_quit;
  GtkWidget *sb_main;
  GtkWidget *main_window;
  GtkWidget *pw_window;
  GtkWidget *pw_treeview;
  GtkWidget *pw_do_classification_cb;
  GtkWidget *pw_entry_projectname;
  GtkWidget *pw_filech_projectfolder;
  GtkWidget *pw_label_projectname;

  GtkWidget *sw_main;
  GtkWidget *tv_main;

  guint sb_main_context_id;
  GSList *project_files;
  gchar *project_filename;
};

gboolean init_gui(GUIData *ltrgui, GError **err);

#endif // GUI_TEST_H
