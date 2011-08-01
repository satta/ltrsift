#ifndef STATUSBAR_MAIN_H
#define STATUSBAR_MAIN_H

#include <gtk/gtk.h>
#include "ltrgui.h"

gboolean statusbar_main_hints(GtkMenuItem *menuitem, GdkEvent *event,
                              LTRData *ltrgui);

#endif // STATUSBAR_MAIN_H
