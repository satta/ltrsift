#include "statusbar_main.h"

gboolean sb_main_hints(GtkMenuItem *menuitem, GdkEvent *event,
                              LTRData *ltrgui)
{
  gchar *menu_hint;

  if (event->type == GDK_ENTER_NOTIFY) {
    menu_hint = (gchar*) g_object_get_data(G_OBJECT(menuitem), "menuhint");
    gtk_statusbar_push(GTK_STATUSBAR(ltrgui->sb_main),
                       ltrgui->sb_main_context_id, menu_hint);
  } else if (event->type == GDK_LEAVE_NOTIFY) {
    gtk_statusbar_pop(GTK_STATUSBAR(ltrgui->sb_main),
                      ltrgui->sb_main_context_id);
  }

  return FALSE;
}
