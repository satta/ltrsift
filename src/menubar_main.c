#include "menubar_main.h"

void mb_main_get_widgets(GtkBuilder *builder, GUIData *ltrgui)
{

#define GW(name) LTR_GET_WIDGET(builder, name, ltrgui)
  GW(mb_main);
  GW(mb_main_project_new);
  GW(mb_main_project_open);
  GW(mb_main_project_save);
  GW(mb_main_project_save_as);
  GW(mb_main_project_quit);
#undef GW

  g_object_set_data(G_OBJECT(ltrgui->mb_main_project_new), "menuhint",
                    (gpointer) "Create a new project.");
  g_object_set_data(G_OBJECT(ltrgui->mb_main_project_open), "menuhint",
                    (gpointer) "Open an existing project.");
  g_object_set_data(G_OBJECT(ltrgui->mb_main_project_save), "menuhint",
                    (gpointer) "Save current project.");
  g_object_set_data(G_OBJECT(ltrgui->mb_main_project_save_as), "menuhint",
                    (gpointer) "Save current project.");
  g_object_set_data(G_OBJECT(ltrgui->mb_main_project_quit), "menuhint",
                    (gpointer) "Quit the application.");
}

gchar* mb_main_project_open_get_filename(GUIData *ltrgui)
{
  GtkWidget *filechooser;
  gchar *filename = NULL;

  filechooser = gtk_file_chooser_dialog_new("Open file...",
                                            GTK_WINDOW (ltrgui->main_window),
                                            GTK_FILE_CHOOSER_ACTION_OPEN,
                                            GTK_STOCK_CANCEL,
                                            GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN,
                                            GTK_RESPONSE_ACCEPT, NULL);

  if (gtk_dialog_run(GTK_DIALOG(filechooser)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser));
  }

  gtk_widget_destroy(filechooser);

  return filename;
}

gchar* mb_main_project_save_as_get_filename(GUIData *ltrgui)
{
  GtkWidget *filechooser;
  gchar *filename = NULL;

  filechooser = gtk_file_chooser_dialog_new("Save file as...",
                                            GTK_WINDOW (ltrgui->main_window),
                                            GTK_FILE_CHOOSER_ACTION_SAVE,
                                            GTK_STOCK_CANCEL,
                                            GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE,
                                            GTK_RESPONSE_ACCEPT, NULL);

  if (gtk_dialog_run(GTK_DIALOG(filechooser)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser));
  }

  gtk_widget_destroy(filechooser);

  return filename;
}

void mb_main_project_save_activate(GtkMenuItem *menuitem, GUIData *ltrgui)
{
  gchar *filename;

  if (ltrgui->project_filename == NULL) {
    filename = mb_main_project_save_as_get_filename(ltrgui);
    if (filename != NULL) ltrgui->project_filename = filename;
    /* TODO: if (filename != NULL) Save project data */
  } /* TODO: else Save project data*/

}

void mb_main_project_save_as_activate(GtkMenuItem *menuitem, GUIData *ltrgui)
{
  gchar *filename;

  filename = mb_main_project_save_as_get_filename(ltrgui);

  if (filename != NULL) {
    gtk_statusbar_push(GTK_STATUSBAR(ltrgui->sb_main),
                       ltrgui->sb_main_context_id, filename);

    if (ltrgui->project_filename != NULL) g_free(ltrgui->project_filename);
    ltrgui->project_filename = filename;
  }

  /* TODO: - Check for overwriting existing file
           - Save project data */
}

void mb_main_project_open_activate(GtkMenuItem *menuitem, GUIData *ltrgui)
{
  gchar *filename;

  /* TODO: check for modified project (check for save) */

  filename = mb_main_project_open_get_filename(ltrgui);

  if (filename != NULL) {
    gtk_statusbar_push(GTK_STATUSBAR(ltrgui->sb_main),
                       ltrgui->sb_main_context_id, filename);

    if (ltrgui->project_filename != NULL) g_free(ltrgui->project_filename);
    ltrgui->project_filename = filename;
  }

  /* TODO: Load project data */

}

void mb_main_project_new_activate(GtkMenuItem *menuitem,
                                  GUIData *ltrgui)
{
  gtk_widget_show(ltrgui->pw_window);
}
