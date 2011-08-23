#ifndef STATUSBAR_MAIN_H
#define STATUSBAR_MAIN_H

#include "ltrgui.h"

#define SB_MAIN_CONTEXT           "LTRGui"
#define SB_MAIN_MSG_WELCOME       "Welcome to LTRGui"
#define SB_MAIN_MENU_HINT         "menuhint"
#define SB_MAIN_MENU_HINT_NEW     "Create a new project"
#define SB_MAIN_MENU_HINT_OPEN    "Open a project"
#define SB_MAIN_MENU_HINT_SAVE    "Save the current project"
#define SB_MAIN_MENU_HINT_SAVE_AS "Save the current project " \
                                  "with a different name"
#define SB_MAIN_MENU_HINT_QUIT    "Quit the program"

void sb_main_init(GUIData *ltrgui);

void sb_main_set_status(GUIData *ltrgui, gchar *status_msg);

gboolean sb_main_menuhints(GtkMenuItem *menuitem, GdkEvent *event,
                               GUIData *ltrgui);

#endif // STATUSBAR_MAIN_H
