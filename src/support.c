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

#include "support.h"

void create_recently_used_resource(const gchar *filename)
{
  GtkRecentManager *manager;
  GtkRecentData *data;
  static gchar *groups[2] = {GUI_RECENT_GROUP, NULL};
  gchar *uri;

  /* Create a new recently used resource. */
  data = g_slice_new(GtkRecentData);
  data->display_name = NULL;
  data->description = NULL;
  data->mime_type = "text/plain";
  data->app_name = (gchar*) g_get_application_name();
  data->app_exec = g_strjoin(" ", g_get_prgname(), "%u", NULL);
  data->groups = groups;
  data->is_private = FALSE;
  uri = g_filename_to_uri(filename, NULL, NULL);
  manager = gtk_recent_manager_get_default();
  gtk_recent_manager_add_full(manager, uri, data);
  g_free(uri);
  g_free(data->app_exec);
  g_slice_free(GtkRecentData, data);
}

void free_gt_hash_elem(void *elem)
{
  gt_free(elem);
}

void reset_progressbar(GtkWidget *progressbar)
{
  gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progressbar), "");
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressbar), 0.0);
  gtk_widget_hide(progressbar);
}
