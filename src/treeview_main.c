#include "treeview_main.h"

void tv_main_init(GUIData *ltrgui)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkTreeIter iter;
  GtkListStore *store;
  GType *types;

  GtStrArray *tests = gt_str_array_new();
  unsigned long nof_columns, i;
  gt_str_array_add_cstr(tests, "pbs");
  gt_str_array_add_cstr(tests, "ltr");
  gt_str_array_add_cstr(tests, "tsd");

  nof_columns = TV_MAIN_N_COLUMS + gt_str_array_size(tests);
  types = g_new0(GType, nof_columns);

  ltrgui->tv_main = gtk_tree_view_new();

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(TV_MAIN_CAPTION_SEQID,
                                                    renderer, "text",
                                                    TV_MAIN_SEQID, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrgui->tv_main), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(TV_MAIN_CAPTION_TYPE,
                                                    renderer, "text",
                                                    TV_MAIN_TYPE, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrgui->tv_main), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(TV_MAIN_CAPTION_START,
                                                    renderer, "text",
                                                    TV_MAIN_START, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrgui->tv_main), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(TV_MAIN_CAPTION_END,
                                                    renderer, "text",
                                                    TV_MAIN_END, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrgui->tv_main), column);

  types[0] = G_TYPE_STRING;
  types[1] = G_TYPE_STRING;
  types[2] = G_TYPE_ULONG;
  types[3] = G_TYPE_ULONG;
  gchar testcaption[200];

  for (i = TV_MAIN_N_COLUMS; i < nof_columns; i++) {
    renderer = gtk_cell_renderer_text_new();
    sprintf(testcaption, "%s %s", gt_str_array_get(tests, i - TV_MAIN_N_COLUMS),
                                  TV_MAIN_CAPTION_CLUSTER);
    column = gtk_tree_view_column_new_with_attributes(testcaption,
                                                      renderer, "text", i, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(ltrgui->tv_main), column);
    types[i] = G_TYPE_UINT;
  }

  store = gtk_list_store_newv(nof_columns, types);

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter,
                     TV_MAIN_SEQID, "seq0",
                     TV_MAIN_TYPE, "repeat_region",
                     TV_MAIN_START, (gulong) 123,
                     TV_MAIN_END, (gulong) 456,
                     4, 12,
                     5, 17,
                     6, 22,
                     -1);

  gtk_tree_view_set_model(GTK_TREE_VIEW(ltrgui->tv_main), GTK_TREE_MODEL(store));
  gtk_tree_view_set_headers_clickable(GTK_TREE_VIEW(ltrgui->tv_main), TRUE);
  g_object_unref(store);

  gtk_container_add(GTK_CONTAINER(ltrgui->sw_main), ltrgui->tv_main);
  gtk_widget_show(ltrgui->tv_main);

  g_free(types);
}
