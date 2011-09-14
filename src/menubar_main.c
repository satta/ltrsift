#include "menubar_main.h"
#include "statusbar_main.h"
#include "gtk_ltr_family.h"
#include "notebook_families.h"
#include "unused.h"

void mb_main_init(GUIData *ltrgui)
{
  g_object_set_data(G_OBJECT(ltrgui->mb_main_project_new),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_NEW);
  g_object_set_data(G_OBJECT(ltrgui->mb_main_project_open),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_OPEN);
  g_object_set_data(G_OBJECT(ltrgui->mb_main_project_save),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_SAVE);
  g_object_set_data(G_OBJECT(ltrgui->mb_main_project_save_as),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_SAVE_AS);
  g_object_set_data(G_OBJECT(ltrgui->mb_main_project_quit),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_QUIT);
}

static gchar* mb_main_project_open_get_filename(GUIData *ltrgui)
{
  GtkWidget *filechooser;
  gchar *filename = NULL;

  filechooser = gtk_file_chooser_dialog_new(GUI_DIALOG_OPEN,
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

static gchar* mb_main_project_save_as_get_filename(GUIData *ltrgui)
{
  GtkWidget *filechooser;
  gchar *filename = NULL;

  filechooser = gtk_file_chooser_dialog_new(GUI_DIALOG_SAVE_AS,
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

void mb_main_project_save_activate(G_UNUSED GtkMenuItem *menuitem,
                                   GUIData *ltrgui)
{
  gchar *filename;

  if (ltrgui->project_filename == NULL) {
    filename = mb_main_project_save_as_get_filename(ltrgui);
    if (filename != NULL) ltrgui->project_filename = filename;
    /* TODO: if (filename != NULL) Save project data */
  } /* TODO: else Save as project data*/
}

void mb_main_project_save_as_activate(G_UNUSED GtkMenuItem *menuitem,
                                      GUIData *ltrgui)
{
  gchar *filename;

  filename = mb_main_project_save_as_get_filename(ltrgui);

  if (filename != NULL) {
    if (ltrgui->project_filename != NULL) g_free(ltrgui->project_filename);
    ltrgui->project_filename = filename;
  }
  /* TODO: - Check for overwriting existing file
           - Save project data */
}

void mb_main_project_open_activate(G_UNUSED GtkMenuItem *menuitem,
                                   GUIData *ltrgui)
{
  gchar *filename;

  /* TODO: check for modified project (check for save) */

  filename = mb_main_project_open_get_filename(ltrgui);

  if (filename != NULL) {
    if (ltrgui->project_filename != NULL)
      g_free(ltrgui->project_filename);
    ltrgui->project_filename = filename;

    GtNodeStream *last_stream = NULL,
                 *gff3_in_stream = NULL,
                 *preprocess_stream = NULL;
    GtError *err = NULL;
    int had_err = 0;
    GtArray *nodes = gt_array_new(sizeof(GtGenomeNode*));
    ltrgui->n_features = LTRFAM_LV_N_COLUMS;

    last_stream = gff3_in_stream = gt_gff3_in_stream_new_sorted(filename);
    last_stream = preprocess_stream =
                  gt_ltrgui_preprocess_stream_new(last_stream, nodes,
                                                  ltrgui->features,
                                                  &ltrgui->n_features, err);
    had_err = gt_node_stream_pull(last_stream, err);
    gt_node_stream_delete(preprocess_stream);
    gt_node_stream_delete(gff3_in_stream);

    nb_families_init(ltrgui, nodes);

    gtk_widget_show_all(ltrgui->hbox1_main);

    gt_array_delete(nodes);
  }
  /* TODO: Load project data */
}

void mb_main_project_new_activate(G_UNUSED GtkMenuItem *menuitem,
                                  GUIData *ltrgui)
{
  gtk_widget_show(ltrgui->pw_window);
}

void mb_main_help_about_activate(G_UNUSED GtkMenuItem *menutiem,
                                 G_UNUSED GUIData *ltrgui)
{
  GtkWidget *dialog;
  GdkPixbuf *logo;
  GError *err = NULL;

  dialog = gtk_about_dialog_new();

  logo = gdk_pixbuf_new_from_file(GUI_LOGO, &err);

  if (err == NULL)
    gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), logo);
  else {
    if (err->domain == GDK_PIXBUF_ERROR)
      g_warning("GdkPixbufError: %s\n", err->message);
    else if (err->domain == G_FILE_ERROR)
      g_warning("GFileError: %s\n", err->message);
    else
      g_warning("An error in the domain: %d has occured!\n", err->domain);

    g_error_free(err);
  }

  gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), GUI_NAME);
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), GUI_VERSION);
  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), GUI_COPYRIGHT);
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), GUI_COMMENTS);
  gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(dialog), GUI_LICENSE);
  gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), GUI_WEBSITE);

  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}
