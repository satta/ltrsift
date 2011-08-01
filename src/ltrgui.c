/*
Then save this file as main.c and compile it using this command
(those are backticks, not single quotes):
  gcc -Wall -g -o gui_test ltrgui.c `pkg-config --cflags --libs gtk+-2.0` -export-dynamic

*/
#include <gtk/gtk.h>
#include "ltrgui.h"
#include "statusbar_main.h"
#include "menubar_main.h"

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
  GW(statusbar_main);
#undef GW
  menubar_main_get_widgets(builder, ltrgui);

  gtk_builder_connect_signals(builder, ltrgui);

  g_object_unref(G_OBJECT (builder));

  /* set the default icon to the GTK "edit" icon */
  gtk_window_set_default_icon_name (GTK_STOCK_EDIT);

  /* setup and initialize our statusbar */
  id = gtk_statusbar_get_context_id(GTK_STATUSBAR(ltrgui->statusbar_main),
                                          "LTRGui");
  ltrgui->statusbar_main_context_id = id;
  gtk_statusbar_push(GTK_STATUSBAR(ltrgui->statusbar_main),
                     ltrgui->statusbar_main_context_id,
                     "Welcome to LTRGui");
    return TRUE;
}




int main(int argc, char *argv[])
{
  LTRData *ltrgui;

  /* allocate the memory needed by our TutorialTextEditor struct */
  ltrgui = g_slice_new(LTRData);

  /* initialize GTK+ libraries */
  gtk_init(&argc, &argv);

  if (init_gui(ltrgui) == FALSE) return 1; /* error loading UI */

  /* show the window */
  gtk_widget_show(ltrgui->window_main);

  /* enter GTK+ main loop */
  gtk_main();

  /* free memory we allocated for TutorialTextEditor struct */
  g_slice_free(LTRData, ltrgui);

  return 0;
}
