#ifndef PROJECT_WIZARD_H
#define PROJECT_WIZARD_H

#include <gtk/gtk.h>
#include "ltrgui.h"

void pw_get_widgets(GtkBuilder *builder, GUIData *ltrgui);

void pw_reset_defaults(GUIData *ltrgui);

void pw_cancel(GtkAssistant *assistant, GUIData *ltrgui);

gint pw_forward(gint current_page, GUIData *ltrgui);

void pw_file_add_button_clicked(GtkButton *button, GUIData *ltrgui);

void pw_remove_row(GtkTreeRowReference *rowref, GUIData *ltrgui);

void pw_file_remove_button_clicked(GtkButton *button, GUIData *ltrgui);

#endif // PROJECT_WIZARD_H
