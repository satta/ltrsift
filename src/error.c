/*
  Copyright (c) 2011-2012 Sascha Kastens <sascha.kastens@studium.uni-hamburg.de>
  Copyright (c) 2011-2012 Center for Bioinformatics, University of Hamburg

  Permission to use, copy, modify, and distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "error.h"

void error_handle(GUIData *ltrgui)
{
  if (ltrgui->err != NULL) {
    GtkWidget *dialog;

    /* create an error message dialog and display modally to the user */
    dialog = gtk_message_dialog_new(GTK_WINDOW(ltrgui->main_window),
                                    GTK_DIALOG_MODAL |
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                    "%s",
                                    ltrgui->err->message);
    gtk_window_set_title(GTK_WINDOW(dialog), "An Error has occurred!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    g_clear_error(&ltrgui->err);
  }
}
