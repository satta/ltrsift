/*
  Copyright (c) 2011-2012 Sascha Kastens <sascha.kastens@studium.uni-hamburg.de>
  Copyright (c) 2011-2012 Center for Bioinformatics, University of Hamburg

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "error.h"

void _error_handle(GtkWidget *toplevel, GtError *err,
                   GT_UNUSED const char *func,
                   GT_UNUSED const char *file,
                   GT_UNUSED int line)
{
  if ((err != NULL) && gt_error_is_set(err)) {
    GtkWidget *dialog;

    /* create an error message dialog and display modally to the user */
    dialog = gtk_message_dialog_new(GTK_WINDOW(toplevel),
                                    GTK_DIALOG_MODAL |
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                    "%s",
                                    gt_error_get(err));
    gtk_window_set_title(GTK_WINDOW(dialog), "Error");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gt_error_unset(err);
  }
}
