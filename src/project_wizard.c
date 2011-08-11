#include "project_wizard.h"
#include "statusbar_main.h"
#include "unused.h"

void pw_init(GUIData *ltrgui)
{
  gtk_label_set_text(GTK_LABEL(ltrgui->pw_label_projectname),
                     gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(
                                             ltrgui->pw_filech_projectfolder)));

  gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(
                                                  ltrgui->pw_treeview)),
                                                        GTK_SELECTION_MULTIPLE);

  gtk_assistant_set_forward_page_func(GTK_ASSISTANT(ltrgui->pw_window),
                               (GtkAssistantPageFunc) pw_forward, ltrgui, NULL);
}

void pw_reset_defaults(GUIData *ltrgui)
{
  GtkTreeModel *model;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->pw_treeview));
  gtk_list_store_clear(GTK_LIST_STORE(model));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(
                               ltrgui->pw_do_classification_cb), FALSE);
  gtk_assistant_set_page_complete(GTK_ASSISTANT(ltrgui->pw_window),
      gtk_assistant_get_nth_page(GTK_ASSISTANT(ltrgui->pw_window), 1), FALSE);
  gtk_assistant_set_page_complete(GTK_ASSISTANT(ltrgui->pw_window),
      gtk_assistant_get_nth_page(GTK_ASSISTANT(ltrgui->pw_window), 2), FALSE);
}

void pw_cancel(GtkAssistant *assistant, GUIData *ltrgui)
{
  gtk_widget_hide(GTK_WIDGET(assistant));
  sb_main_set_status(ltrgui, "Welcome to LTRGui");
  pw_reset_defaults(ltrgui);
}

gint pw_forward(gint current_page, GUIData *ltrgui)
{
  gint next_page = 0;

  switch (current_page) {
    case 0:
      next_page = 1;
      break;
    case 1:
      next_page = (gtk_toggle_button_get_active(
                   GTK_TOGGLE_BUTTON(ltrgui->pw_do_classification_cb)) ? 2 : 3);
      break;
    case 2:
      next_page = 3;
      break;
    default:
      next_page = -1;
  }
  return next_page;
}

void pw_update_label_projectname(GUIData *ltrgui)
{
  const gchar *filename;
  filename = gtk_entry_get_text(GTK_ENTRY(ltrgui->pw_entry_projectname));
  gchar *folder = gtk_file_chooser_get_filename(
                             GTK_FILE_CHOOSER(ltrgui->pw_filech_projectfolder));

  gchar *fullpath = g_strconcat(folder, "/", filename, NULL);

  gtk_label_set_text(GTK_LABEL(ltrgui->pw_label_projectname), fullpath);


  g_free(fullpath);
  g_free(folder);

}

void pw_entry_projectname_changed(G_UNUSED GtkEntry *entry,
                                  GUIData *ltrgui)
{
  pw_update_label_projectname(ltrgui);
}

void pw_filech_projectfolder_selection_changed(G_UNUSED GtkFileChooser *chooser,
                                               GUIData *ltrgui)
{
  pw_update_label_projectname(ltrgui);
}

void pw_file_add_button_clicked(G_UNUSED GtkButton *button, GUIData *ltrgui)
{
  GtkWidget *filechooser;
  GSList *filenames;

  filechooser = gtk_file_chooser_dialog_new("Select files...",
                                          GTK_WINDOW(ltrgui->pw_window),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);

  gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(filechooser), TRUE);

  gint result = gtk_dialog_run(GTK_DIALOG(filechooser));

  if (result == GTK_RESPONSE_ACCEPT) {
    GtkTreeIter iter;
    GtkTreeModel *model =
            gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->pw_treeview));
    gtk_assistant_set_page_complete(GTK_ASSISTANT(ltrgui->pw_window),
            gtk_assistant_get_nth_page(GTK_ASSISTANT(ltrgui->pw_window), 1),
                                                                          TRUE);
    gtk_list_store_clear(GTK_LIST_STORE(model));
    /*ltrgui->project_files = */
    filenames = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(filechooser));

    while (filenames != NULL) {
      gchar *file = (gchar*) filenames->data;

      gtk_list_store_append(GTK_LIST_STORE(model), &iter);
      gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, file, -1);

      filenames = filenames->next;
    }
  }

  gtk_widget_destroy(filechooser);

}

void pw_remove_row(GtkTreeRowReference *rowref, GUIData *ltrgui)
{
  GtkTreeIter iter, tmp;
  GtkTreePath *path;
  GtkTreeModel *model;
  gboolean empty;

  model =
     gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->pw_treeview));

  path = gtk_tree_row_reference_get_path(rowref);
  gtk_tree_model_get_iter(model, &iter, path);

  gtk_list_store_remove(GTK_LIST_STORE(model), &iter);

  empty = gtk_tree_model_get_iter_first(model, &tmp);

  if (!empty) {
    gtk_assistant_set_page_complete(GTK_ASSISTANT(ltrgui->pw_window),
        gtk_assistant_get_nth_page(GTK_ASSISTANT(ltrgui->pw_window), 1), FALSE);
  }

}

void pw_file_remove_button_clicked(G_UNUSED GtkButton *button, GUIData *ltrgui)
{
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  GtkTreeRowReference *rowref;
  GList *rows, *tmp, *references = NULL;

  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(ltrgui->pw_treeview));

  if (!gtk_tree_selection_count_selected_rows(sel)) return;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->pw_treeview));
  rows = gtk_tree_selection_get_selected_rows(sel, &model);

  tmp = rows;

  while (tmp != NULL) {
    rowref = gtk_tree_row_reference_new(model, (GtkTreePath*) tmp->data);
    references =
                g_list_prepend(references, gtk_tree_row_reference_copy(rowref));
    gtk_tree_row_reference_free(rowref);
    tmp = tmp->next;
  }

  g_list_foreach(references, (GFunc) pw_remove_row, ltrgui);
  g_list_foreach(references, (GFunc) gtk_tree_row_reference_free, NULL);
  g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
  g_list_free(references);
  g_list_free(rows);
}
