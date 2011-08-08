#include "error.h"

void error_handle(GError *err)
{
  if (err != NULL) {
    GtkWidget *dialog;

    /* log to stderr */
    fprintf(stderr, "ERROR: %s\n", err->message);

    /* create an error message dialog and display modally to the user */
    dialog = gtk_message_dialog_new(NULL,
                                    GTK_DIALOG_MODAL |
                                                 GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "test");
    gtk_window_set_title(GTK_WINDOW(dialog), "ERROR!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  }
}
