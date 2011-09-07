#include "treeview_families.h"
#include "unused.h"

/*static void editing_canceled(GtkCellRenderer *renderer, GUIData ltrgui)
{

}*/

static void cell_edited(G_UNUSED GtkCellRendererText *cell, gchar *path_string,
                        gchar *new_text, GUIData *ltrgui)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GList *renderers;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->tv_families));
  gtk_tree_model_get_iter_from_string(model, &iter, path_string);
  gtk_tree_store_set(GTK_TREE_STORE(model), &iter, 0, new_text, -1);
  column = gtk_tree_view_get_column(GTK_TREE_VIEW(ltrgui->tv_families), 0);
  renderers = gtk_cell_layout_get_cells(GTK_CELL_LAYOUT(column));
  renderer = (GtkCellRenderer*) g_list_nth_data(renderers, 0);
  g_object_set(renderer, "editable", FALSE, NULL);
}

void tv_families_init(GUIData *ltrgui)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkTreeStore *store;

  ltrgui->tv_families = gtk_tree_view_new();
  renderer = gtk_cell_renderer_text_new();
  g_signal_connect(renderer, "edited", (GCallback) cell_edited, ltrgui);
  /*g_signal_connect(renderer, "editing-canceled",
                   (GCallback) editing_canceled, ltrgui);*/
  column  = gtk_tree_view_column_new_with_attributes("LTR Families",
                                                     renderer, "text",
                                                     0, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrgui->tv_families), column);

  store = gtk_tree_store_new(1, G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW(ltrgui->tv_families),
                          GTK_TREE_MODEL(store));
  g_object_unref(store);

  gtk_container_add(GTK_CONTAINER(ltrgui->sw_main), ltrgui->tv_families);
  gtk_widget_show(ltrgui->tv_families);
}

void add_family_button_clicked(G_UNUSED GtkWidget *button, GUIData *ltrgui)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkTreeStore *store;
  GtkTreeViewColumn *column;
  GtkTreePath *path;
  GtkCellRenderer *renderer;
  GList *renderers;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->tv_families));
  store = GTK_TREE_STORE(model);
  column = gtk_tree_view_get_column(GTK_TREE_VIEW(ltrgui->tv_families), 0);
  renderers = gtk_cell_layout_get_cells(GTK_CELL_LAYOUT(column));
  renderer = (GtkCellRenderer*) g_list_nth_data(renderers, 0);
  g_object_set(renderer, "editable", TRUE, NULL);

  gtk_tree_store_append(store, &iter, NULL);
  path = gtk_tree_model_get_path(model, &iter);

  gtk_tree_view_set_cursor(GTK_TREE_VIEW(ltrgui->tv_families),
                           path, column, TRUE);
}
