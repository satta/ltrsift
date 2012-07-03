/*
  Copyright (c) 2011-2012 Sascha Kastens <sascha.kastens@studium.uni-hamburg.de>
  Copyright (c) 2011-2012 Center for Bioinformatics, University of Hamburg

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
