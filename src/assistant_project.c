#include "assistant_project.h"
#include <string.h>

void assistant_project_get_widgets(GtkBuilder *builder, LTRData *ltrgui)
{
#define GW(name) LTR_GET_WIDGET(builder, name, ltrgui)
  GW(assistant_project);
  GW(assistant_project_treeview);
  GW(assistant_project_class_cb);
#undef GW
  gtk_tree_selection_set_mode(
                             gtk_tree_view_get_selection(
                             GTK_TREE_VIEW(ltrgui->assistant_project_treeview)),
                             GTK_SELECTION_MULTIPLE);

  gtk_assistant_set_forward_page_func(GTK_ASSISTANT(ltrgui->assistant_project),
                               (GtkAssistantPageFunc) assistant_project_forward,
                                      ltrgui, NULL);
}

void assistant_project_reset_defaults(LTRData *ltrgui)
{
  GtkTreeModel *model;

  model =
     gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->assistant_project_treeview));
  gtk_list_store_clear(GTK_LIST_STORE(model));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(
                                    ltrgui->assistant_project_class_cb), FALSE);
  gtk_assistant_set_page_complete(GTK_ASSISTANT(ltrgui->assistant_project),
      gtk_assistant_get_nth_page(GTK_ASSISTANT(ltrgui->assistant_project), 1),
                                  FALSE);
  gtk_assistant_set_page_complete(GTK_ASSISTANT(ltrgui->assistant_project),
      gtk_assistant_get_nth_page(GTK_ASSISTANT(ltrgui->assistant_project), 2),
                                  FALSE);
}

void assistant_project_cancel(GtkAssistant *assistant, LTRData *ltrgui)
{
  gtk_widget_hide(GTK_WIDGET(assistant));
  assistant_project_reset_defaults(ltrgui);
}

gboolean foreach_func(GtkTreeModel *model, GtkTreePath *path,
                      GtkTreeIter *iter, gchar *user_data)
{
    gchar *file;

    gtk_tree_model_get (model, iter, 0, &file, -1);
    g_warning("%s", file);

    return FALSE;
}


gint assistant_project_forward(gint current_page, LTRData *ltrgui)
{
  gint next_page = 0;

  switch (current_page) {
    case 0:
      next_page = 1;
      break;
    case 1:
      next_page = (gtk_toggle_button_get_active(
                GTK_TOGGLE_BUTTON(ltrgui->assistant_project_class_cb)) ? 2 : 3);
      break;
    case 2:
      next_page = 3;
      break;
    default:
      next_page = -1;
  }
  return next_page;
}

void assistant_project_file_add_button_clicked(GtkButton *button,
                                               LTRData *ltrgui)
{
  GtkWidget *filechooser;
  GSList *filenames;

  filechooser = gtk_file_chooser_dialog_new("Select files...",
                                          GTK_WINDOW(ltrgui->assistant_project),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);

  gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(filechooser), TRUE);

  gint result = gtk_dialog_run(GTK_DIALOG(filechooser));

  if (result == GTK_RESPONSE_ACCEPT) {
    GtkTreeIter iter;
    GtkTreeModel *model =
     gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->assistant_project_treeview));
    gtk_assistant_set_page_complete(GTK_ASSISTANT(ltrgui->assistant_project),
        gtk_assistant_get_nth_page(GTK_ASSISTANT(ltrgui->assistant_project), 1),
                                    TRUE);
    gtk_list_store_clear(GTK_LIST_STORE(model));
    ltrgui->project_files = filenames =
                  gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(filechooser));

    while (filenames != NULL) {
      gchar *file = (gchar*) filenames->data;

      gtk_list_store_append(GTK_LIST_STORE(model), &iter);
      gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, file, -1);

      filenames = filenames->next;
    }
  }

  gtk_widget_destroy(filechooser);

}

void assistant_project_remove_row(GtkTreeRowReference *rowref,
                                  LTRData *ltrgui)
{
  GtkTreeIter iter, tmp;
  GtkTreePath *path;
  GtkTreeModel *model;
  gboolean empty;

  model =
     gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->assistant_project_treeview));

  path = gtk_tree_row_reference_get_path(rowref);
  gtk_tree_model_get_iter(model, &iter, path);

  gtk_list_store_remove(GTK_LIST_STORE(model), &iter);

  empty = gtk_tree_model_get_iter_first(model, &tmp);

  if (!empty) {
    gtk_assistant_set_page_complete(GTK_ASSISTANT(ltrgui->assistant_project),
        gtk_assistant_get_nth_page(GTK_ASSISTANT(ltrgui->assistant_project), 1),
                                    FALSE);
  }

}

void assistant_project_file_remove_button_clicked(GtkButton *button,
                                                  LTRData *ltrgui)
{
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  GtkTreeRowReference *rowref;
  GList *rows, *tmp, *references = NULL;

  sel = gtk_tree_view_get_selection(
                             GTK_TREE_VIEW(ltrgui->assistant_project_treeview));

  if (!gtk_tree_selection_count_selected_rows(sel))
      return;

  model =
     gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->assistant_project_treeview));
  rows = gtk_tree_selection_get_selected_rows(sel, &model);

  tmp = rows;

  while (tmp != NULL) {
    rowref = gtk_tree_row_reference_new(model, (GtkTreePath*) tmp->data);
    references =
                g_list_prepend(references, gtk_tree_row_reference_copy(rowref));
    gtk_tree_row_reference_free(rowref);
    tmp = tmp->next;
  }

  g_list_foreach(references, (GFunc) assistant_project_remove_row, ltrgui);

  g_list_foreach(references, (GFunc) gtk_tree_row_reference_free, NULL);
  g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
  g_list_free(references);
  g_list_free(rows);
}
