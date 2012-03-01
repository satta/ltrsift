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

#include "message_strings.h"
#include "statusbar.h"

void statusbar_init(GUIData *ltrgui)
{
  GtkWidget *hbox;
  gchar msg[BUFSIZ];
  gint id;

  ltrgui->statusbar = gtk_statusbar_new();
  ltrgui->progressbar = gtk_progress_bar_new();

  hbox = gtk_hbox_new(TRUE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), ltrgui->statusbar, FALSE, TRUE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), ltrgui->progressbar, TRUE, TRUE, 1);
  gtk_widget_show_all(hbox);

  id = gtk_statusbar_get_context_id(GTK_STATUSBAR(ltrgui->statusbar),
                                    STATUSBAR_CONTEXT);

  ltrgui->statusbar_context_id = id;
  g_snprintf(msg, BUFSIZ, STATUSBAR_MSG_WELCOME, GUI_NAME);
  gtk_statusbar_push(GTK_STATUSBAR(ltrgui->statusbar),
                     ltrgui->statusbar_context_id, msg);
  gtk_box_pack_end(GTK_BOX(ltrgui->vbox), hbox, FALSE, FALSE, 0);
}

void statusbar_set_status(GtkWidget *sb, gchar *status_msg)
{
  gint id;

  id = gtk_statusbar_get_context_id(GTK_STATUSBAR(sb), STATUSBAR_CONTEXT);
  gtk_statusbar_pop(GTK_STATUSBAR(sb), id);
  gtk_statusbar_push(GTK_STATUSBAR(sb), id, status_msg);
}

gboolean statusbar_menuhints(GtkMenuItem *menuitem, GdkEvent *event,
                             GUIData *ltrgui)
{
  gchar *menu_hint = NULL;

  if (event->type == GDK_ENTER_NOTIFY) {
    menu_hint = (gchar*) g_object_get_data(G_OBJECT(menuitem),
                                           STATUSBAR_MENU_HINT);
    gtk_statusbar_push(GTK_STATUSBAR(ltrgui->statusbar),
                       ltrgui->statusbar_context_id, menu_hint);
  } else if (event->type == GDK_LEAVE_NOTIFY) {
    gtk_statusbar_pop(GTK_STATUSBAR(ltrgui->statusbar),
                      ltrgui->statusbar_context_id);
  }
  return FALSE;
}
