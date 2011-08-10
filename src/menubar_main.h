#ifndef MENUBAR_MAIN_H
#define MENUBAR_MAIN_H

#include "ltrgui.h"

#define LTR_GET_OBJECT(builder, name, type, data) \
  data->name = type(gtk_builder_get_object(builder, #name))
#define LTR_GET_WIDGET(builder, name, data) \
  LTR_GET_OBJECT(builder, name, GTK_WIDGET, data)

void mb_main_get_widgets(GtkBuilder *builder, GUIData *ltrgui);

gchar* mb_main_project_open_get_filename(GUIData *ltrgui);

gchar* mb_main_project_save_as_get_filename(GUIData *ltrgui);

void mb_main_project_new_activate(GtkMenuItem *menuitem,
                                  GUIData *ltrgui);

#endif // MENUBAR_MAIN_H
