#ifndef PROJECT_WIZARD_H
#define PROJECT_WIZARD_H

#include <gtk/gtk.h>
#include "ltrgui.h"

void pw_get_widgets(GtkBuilder *builder, GUIWidgets *ltrgui);

void pw_reset_defaults(GUIWidgets *ltrgui);

void pw_cancel(GtkAssistant *assistant, GUIWidgets *ltrgui);

gint pw_forward(gint current_page, GUIWidgets *ltrgui);

void pw_file_add_button_clicked(GtkButton *button, GUIWidgets *ltrgui);

void pw_remove_row(GtkTreeRowReference *rowref, GUIWidgets *ltrgui);

void pw_file_remove_button_clicked(GtkButton *button, GUIWidgets *ltrgui);

#endif // PROJECT_WIZARD_H
