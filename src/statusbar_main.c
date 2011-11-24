/*
  Copyright (c) 2011-2012 Sascha Kastens <sascha.kastens@studium.uni-hamburg.de>
  Copyright (c) 2011-2012 Center for Bioinformatics, University of Hamburg

  Permission to use, copy, modify, and distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

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

void sb_main_set_status(GtkWidget *sb, gchar *status_msg)
{
  gint id;

  id = gtk_statusbar_get_context_id(GTK_STATUSBAR(sb), SB_MAIN_CONTEXT);
  gtk_statusbar_pop(GTK_STATUSBAR(sb), id);
  gtk_statusbar_push(GTK_STATUSBAR(sb), id, status_msg);
}

gboolean sb_main_menuhints(GtkMenuItem *menuitem, GdkEvent *event,
                               GUIData *ltrgui)
{
  gchar *menu_hint = NULL;

  if (event->type == GDK_ENTER_NOTIFY) {
    menu_hint = (gchar*) g_object_get_data(G_OBJECT(menuitem),
                                           SB_MAIN_MENU_HINT);
    gtk_statusbar_push(GTK_STATUSBAR(ltrgui->sb_main),
                       ltrgui->sb_main_context_id, menu_hint);
  } else if (event->type == GDK_LEAVE_NOTIFY) {
    gtk_statusbar_pop(GTK_STATUSBAR(ltrgui->sb_main),
                      ltrgui->sb_main_context_id);
  }
  return FALSE;
}
