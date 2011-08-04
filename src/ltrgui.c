/*
Then save this file as main.c and compile it using this command
(those are backticks, not single quotes):
  gcc -Wall -g -o gui_test ltrgui.c `pkg-config --cflags --libs gtk+-2.20` -export-dynamic

*/
#include <gtk/gtk.h>
#include "ltrgui.h"
#include "statusbar_main.h"
#include "menubar_main.h"
#include "assistant_project.h"

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

gboolean init_gui(LTRData *ltrgui)
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
  assistant_project_get_widgets(builder, ltrgui);

  gtk_window_set_transient_for(GTK_WINDOW(ltrgui->assistant_project),
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

int main(int argc, char *argv[])
{
  LTRData *ltrgui;

  /* allocate the memory needed by our LTRData struct */
  ltrgui = g_slice_new(LTRData);

  /* initialize GTK+ libraries */
  gtk_init(&argc, &argv);

  if (init_gui(ltrgui) == FALSE) return 1; /* error loading UI */

  /* show the window */
  gtk_widget_show(ltrgui->window_main);

  /* enter GTK+ main loop */
  gtk_main();

  /* free memory we allocated for LTRData struct */
  g_slice_free(LTRData, ltrgui);

  return 0;
}
