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

#include "gtk_ltr_filter.h"

GT_UNUSED static void gtk_ltr_filter_list_view_changed(GtkTreeView *list_view,
                                             GT_UNUSED GtkLTRFilter *ltrfilt)
{
  GtkTreeModel *model;

  model = gtk_tree_view_get_model(list_view);
}

static void gtk_ltr_filter_cancel_clicked(GT_UNUSED GtkButton *button,
                                          gpointer user_data)
{
  gtk_widget_hide(GTK_WIDGET(user_data));
}

static void gtk_ltr_filter_change_filter_dir(GtkFileChooser *chooser,
                                             gpointer user_data)
{
  GtkTreeView *list_view = GTK_TREE_VIEW(user_data);
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreeSelection *selection;
  GDir *dir;
  gboolean valid;
  gchar *path;
  const char *file;

  model = gtk_tree_view_get_model(list_view);
  gtk_list_store_clear(GTK_LIST_STORE(model));

  path = gtk_file_chooser_get_filename(chooser);
  dir = g_dir_open(path, 0, NULL);

  while ((file = g_dir_read_name(dir))) {
    if (g_str_has_suffix(file, ".lua")) {
      gtk_list_store_append(GTK_LIST_STORE(model), &iter);
      gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                         0, file,
                         -1);
    }
  }
  valid = gtk_tree_model_get_iter_first(model, &iter);
  selection = gtk_tree_view_get_selection(list_view);
  if (!valid) {
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_NONE);
    gtk_list_store_append(GTK_LIST_STORE(model), &iter);
    gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                       0, "No filter scripts found!",
                       -1);
  } else
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_MULTIPLE);

  g_dir_close(dir);
  g_free(path);
}

static void gtk_ltr_filter_delete_event(GtkWidget *widget,
                                        GT_UNUSED GdkEvent *event,
                                        GT_UNUSED gboolean user_data)
{
  gtk_widget_hide(widget);
}

static void gtk_ltr_filter_init(GtkLTRFilter *ltrfilt)
{
  GtkWidget *apply,
            *cancel,
            *vbox,
            *hbox,
            *sw1,
            *label1,
            *label2,
            *hsep2,
            *button;
  GtkListStore *store;
  GtkTreeViewColumn *column;
  GtkCellRenderer *renderer;

  /* select filter */
  sw1 = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw1),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
  ltrfilt->list_view_all = gtk_tree_view_new();
  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("lua filter files",
                                                    renderer,
                                                    "text",
                                                    0,
                                                    NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrfilt->list_view_all), column);
  store = gtk_list_store_new(1, G_TYPE_STRING);
  gtk_tree_view_set_model(GTK_TREE_VIEW(ltrfilt->list_view_all),
                          GTK_TREE_MODEL(store));
  g_object_unref(store);
  gtk_container_add(GTK_CONTAINER(sw1), ltrfilt->list_view_all);

  hbox = gtk_hbox_new(FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), sw1, TRUE, TRUE, 1);

  /* add/remove filter buttons */
  vbox = gtk_vbox_new(FALSE, 1);
  button = gtk_button_new_from_stock(GTK_STOCK_ADD);
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 1);
  button = gtk_button_new_from_stock(GTK_STOCK_REMOVE);
  gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 1);

  /* selected filter */
  sw1 = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw1),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
  ltrfilt->list_view_sel = gtk_tree_view_new();
  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Selected lua filter files",
                                                    renderer,
                                                    "text",
                                                    0,
                                                    NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrfilt->list_view_sel), column);
  store = gtk_list_store_new(1, G_TYPE_STRING);
  gtk_tree_view_set_model(GTK_TREE_VIEW(ltrfilt->list_view_sel),
                          GTK_TREE_MODEL(store));
  g_object_unref(store);
  gtk_container_add(GTK_CONTAINER(sw1), ltrfilt->list_view_sel);
  gtk_box_pack_start(GTK_BOX(hbox), sw1, TRUE, TRUE, 1);

  vbox = gtk_vbox_new(FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 1);

  label1 = gtk_label_new("Directory containing filter scripts (*.lua)");
  ltrfilt->dir_chooser =
             gtk_file_chooser_button_new("Choose a Folder",
                                         GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
  gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(ltrfilt->dir_chooser),
                                      g_get_home_dir());
  gtk_box_pack_start(GTK_BOX(vbox), label1, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrfilt->dir_chooser, FALSE, FALSE, 1);

  label2 = gtk_label_new("Script information");
  gtk_box_pack_start(GTK_BOX(vbox), label2, FALSE, FALSE, 1);
  ltrfilt->label_name = gtk_label_new("");
  ltrfilt->label_descr = gtk_label_new("");
  ltrfilt->label_author = gtk_label_new("");
  ltrfilt->label_version = gtk_label_new("");
  ltrfilt->label_email = gtk_label_new("");

  label2 = gtk_label_new("Name:");
  gtk_misc_set_alignment(GTK_MISC(label2), 0.0, 0.5);
  hsep2 = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(vbox), label2, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrfilt->label_name, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), hsep2, FALSE, FALSE, 1);

  label2 = gtk_label_new("Author:");
  gtk_misc_set_alignment(GTK_MISC(label2), 0.0, 0.5);
  hsep2 = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(vbox), label2, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrfilt->label_author, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), hsep2, FALSE, FALSE, 1);

  label2 = gtk_label_new("Author email:");
  gtk_misc_set_alignment(GTK_MISC(label2), 0.0, 0.5);
  hsep2 = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(vbox), label2, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrfilt->label_email, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), hsep2, FALSE, FALSE, 1);

  label2 = gtk_label_new("Version:");
  gtk_misc_set_alignment(GTK_MISC(label2), 0.0, 0.5);
  hsep2 = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(vbox), label2, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrfilt->label_version, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), hsep2, FALSE, FALSE, 1);

  label2 = gtk_label_new("Description:");
  gtk_misc_set_alignment(GTK_MISC(label2), 0.0, 0.5);
  hsep2 = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(vbox), label2, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrfilt->label_descr, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), hsep2, FALSE, FALSE, 1);

  hbox = gtk_hbox_new(FALSE, 0);
  apply = gtk_button_new_with_mnemonic("_Apply");
  cancel = gtk_button_new_with_mnemonic("_Cancel");
  gtk_box_pack_start(GTK_BOX(hbox), apply, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), cancel, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 1);

  gtk_container_add(GTK_CONTAINER(ltrfilt), vbox);
  /* connect signals */
  g_signal_connect(G_OBJECT(ltrfilt->dir_chooser), "selection-changed",
                   G_CALLBACK(gtk_ltr_filter_change_filter_dir),
                   ltrfilt->list_view_all);
  g_signal_connect(G_OBJECT(cancel), "clicked",
                   G_CALLBACK(gtk_ltr_filter_cancel_clicked), ltrfilt);
  gtk_widget_show_all(GTK_WIDGET(ltrfilt));
  gtk_window_resize(GTK_WINDOW(ltrfilt), 800, 600);
}

GType gtk_ltr_filter_get_type(void)
{
  static GType ltr_filter_type = 0;

  if (!ltr_filter_type) {
    const GTypeInfo ltr_filter_info =
    {
      sizeof (GtkLTRFilterClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      NULL, /*(GClassInitFunc) */
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (GtkLTRFilter),
      0, /* n_preallocs */
      (GInstanceInitFunc) gtk_ltr_filter_init,
    };
    ltr_filter_type = g_type_register_static(GTK_TYPE_WINDOW, "GtkLTRFilter",
                                             &ltr_filter_info, 0);
  }
  return ltr_filter_type;
}

GtkWidget* gtk_ltr_filter_new()
{
  GtkLTRFilter *ltrfilt;
  ltrfilt = gtk_type_new(GTK_LTR_FILTER_TYPE);
  g_signal_connect(G_OBJECT(ltrfilt), "delete_event",
                   G_CALLBACK(gtk_ltr_filter_delete_event), NULL);
  gtk_window_set_position(GTK_WINDOW(ltrfilt), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_window_set_modal(GTK_WINDOW(ltrfilt), TRUE);
  gtk_window_set_title(GTK_WINDOW(ltrfilt), "Filter");
  gtk_container_set_border_width(GTK_CONTAINER(ltrfilt), 5);

  return GTK_WIDGET(ltrfilt);
}
