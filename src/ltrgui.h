#ifndef GUI_TEST_H
#define GUI_TEST_H

#include <gtk/gtk.h>
#include "genometools.h"
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
  GtkWidget *hbox1_main;
  GtkWidget *nb_main_families;
  GtkWidget *sb_main;
  GtkWidget *main_window;
  GtkWidget *pw_window;
  GtkWidget *pw_treeview_gff3;
  GtkWidget *pw_do_classification_cb;
  GtkWidget *pw_label_projectname;
  GtkWidget *pw_label_encseq;
  /* LTRGui is redesigned, no functionality needed atm */
  //GtkWidget *sw_main;
  GtkWidget *tv_main;
  guint sb_main_context_id;
  GSList *project_files;
  gchar *project_filename;
  GtHashmap *features;
  /* GtkWidget *pw_treeview_encseq;
  not needed unless the user should be able
  to select more than one seq file */
};

gboolean init_gui(GUIData *ltrgui, GError **err);

#endif // GUI_TEST_H
