#ifndef ASSISTANT_PROJECT_H
#define ASSISTANT_PROJECT_H

#include <gtk/gtk.h>
#include "ltrgui.h"

void assistant_project_get_widgets(GtkBuilder *builder, LTRData *ltrgui);

void assistant_project_reset_defaults(LTRData *ltrgui);

void assistant_project_cancel(GtkAssistant *assistant, LTRData *ltrgui);

gint assistant_project_forward(gint current_page, LTRData *ltrgui);

void assistant_project_file_add_button_clicked(GtkButton *button,
                                               LTRData *ltrgui);

void assistant_project_remove_row(GtkTreeRowReference *rowref,
                                  LTRData *ltrgui);

void assistant_project_file_remove_button_clicked(GtkButton *button,
                                                  LTRData *ltrgui);

#endif // ASSISTANT_PROJECT_H
