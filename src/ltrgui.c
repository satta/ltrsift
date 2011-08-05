#include <gtk/gtk.h>
#include "ltrgui.h"
#include "statusbar_main.h"
#include "menubar_main.h"
#include "project_wizard.h"

/* TODO: Error handling */

void error_message(const gchar *message)
{
  GtkWidget *dialog;

  /* log to terminal window */
  g_warning("%s", message);

  /* create an error message dialog and display modally to the user */
  dialog = gtk_message_dialog_new(NULL,
                                  GTK_DIALOG_MODAL |
                                                 GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "test");
  gtk_window_set_title(GTK_WINDOW(dialog), "Error!");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

gboolean init_gui(GUIWidgets *ltrgui)
{
  GtkBuilder *builder;
  GError *err = NULL;
  guint id;

  builder = gtk_builder_new();

  if (gtk_builder_add_from_file(builder, GUI_FILE, &err) == 0) {
    error_message(err->message);
    g_error_free(err);
    return FALSE;
  }

  /* Get objects from UI */
#define GW(name) LTR_GET_WIDGET(builder, name, ltrgui)
  GW(window_main);
  GW(sb_main);
#undef GW
  mb_main_get_widgets(builder, ltrgui);
  pw_get_widgets(builder, ltrgui);

  gtk_window_set_transient_for(GTK_WINDOW(ltrgui->pw_window),
                               GTK_WINDOW(ltrgui->window_main));

  gtk_builder_connect_signals(builder, ltrgui);

  g_object_unref(G_OBJECT(builder));

  ltrgui->project_filename = NULL;
  //ltrgui->project_files = NULL;

  /* setup and initialize statusbar */
  id = gtk_statusbar_get_context_id(GTK_STATUSBAR(ltrgui->sb_main), "LTRGui");
  ltrgui->sb_main_context_id = id;
  gtk_statusbar_push(GTK_STATUSBAR(ltrgui->sb_main), ltrgui->sb_main_context_id,
                     "Welcome to LTRGui");

  return TRUE;
}

gint main(gint argc, gchar *argv[])
{
  GUIWidgets *ltrgui;

  /* allocate the memory needed by our GUIWidgets struct */
  ltrgui = g_slice_new(GUIWidgets);

  /* initialize GTK+ libraries */
  gtk_init(&argc, &argv);

  if (init_gui(ltrgui) == FALSE) return 1; /* error loading UI */

  /* show the window */
  gtk_widget_show(ltrgui->window_main);

  /* enter GTK+ main loop */
  gtk_main();

  /* free memory we allocated for GUIWidgets struct */
  g_slice_free(GUIWidgets, ltrgui);

  return 0;
}
