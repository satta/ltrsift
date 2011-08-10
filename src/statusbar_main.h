#ifndef STATUSBAR_MAIN_H
#define STATUSBAR_MAIN_H

#include "ltrgui.h"

void sb_main_init(GUIData *ltrgui);

void sb_main_set_status(GUIData *ltrgui, gchar *status_msg);

gboolean sb_main_menuhints(GtkMenuItem *menuitem, GdkEvent *event,
                               GUIData *ltrgui);

#endif // STATUSBAR_MAIN_H
