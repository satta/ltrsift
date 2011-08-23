#include "statusbar_main.h"

void sb_main_init(GUIData *ltrgui)
{
  gint id;
  id = gtk_statusbar_get_context_id(GTK_STATUSBAR(ltrgui->sb_main),
                                    SB_MAIN_CONTEXT);

  ltrgui->sb_main_context_id = id;

  gtk_statusbar_push(GTK_STATUSBAR(ltrgui->sb_main), ltrgui->sb_main_context_id,
                     SB_MAIN_MSG_WELCOME);
}

void sb_main_set_status(GUIData *ltrgui, gchar *status_msg)
{
  gtk_statusbar_pop(GTK_STATUSBAR(ltrgui->sb_main),
                    ltrgui->sb_main_context_id);
  gtk_statusbar_push(GTK_STATUSBAR(ltrgui->sb_main),
                     ltrgui->sb_main_context_id, status_msg);
}

gboolean sb_main_menuhints(GtkMenuItem *menuitem, GdkEvent *event,
                               GUIData *ltrgui)
{
  gchar *menu_hint = NULL;

  if (event->type == GDK_ENTER_NOTIFY) {
    menu_hint = (gchar*) g_object_get_data(G_OBJECT(menuitem), SB_MAIN_MENU_HINT);
    gtk_statusbar_push(GTK_STATUSBAR(ltrgui->sb_main),
                       ltrgui->sb_main_context_id, menu_hint);
  } else if (event->type == GDK_LEAVE_NOTIFY) {
    gtk_statusbar_pop(GTK_STATUSBAR(ltrgui->sb_main),
                      ltrgui->sb_main_context_id);
  }

  return FALSE;
}
