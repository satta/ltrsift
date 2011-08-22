#include "treeview_main.h"

void tv_main_init(GUIData *ltrgui)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkTreeIter iter;
  GtkListStore *store;
  GType *types;

  int num_of_features = 2, nof_columns, i;

  nof_columns = TV_MAIN_N_COLUMS + num_of_features;
  types = g_new0(GType, nof_columns);

  ltrgui->tv_main = gtk_tree_view_new();

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(TV_MAIN_CAPTION_SEQID,
                                                    renderer, "text", TV_MAIN_SEQID, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrgui->tv_main), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(TV_MAIN_CAPTION_TYPE,
                                                    renderer, "text", TV_MAIN_TYPE, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrgui->tv_main), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(TV_MAIN_CAPTION_START, renderer,
                                                    "text", TV_MAIN_START, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrgui->tv_main), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(TV_MAIN_CAPTION_END, renderer,
                                                    "text", TV_MAIN_END, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrgui->tv_main), column);

  types[0] = G_TYPE_STRING;
  types[1] = G_TYPE_STRING;
  types[2] = G_TYPE_ULONG;
  types[3] = G_TYPE_ULONG;

  for (i = TV_MAIN_N_COLUMS; i < nof_columns; i++) {
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("TEST COLUMN",
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
                     -1);

  gtk_tree_view_set_model(GTK_TREE_VIEW(ltrgui->tv_main), GTK_TREE_MODEL(store));
  g_object_unref(store);

  gtk_container_add(GTK_CONTAINER(ltrgui->sw_main), ltrgui->tv_main);
  gtk_widget_show(ltrgui->tv_main);

  g_free(types);

  /*GtTypeChecker *type_checker = NULL;
  GtNodeStream *gff3_in_stream;
  int had_err = 0;
  GtError *err = NULL;
  gt_error_check(err);

  gff3_in_stream = gt_gff3_in_stream_new_sorted(ltrgui->project_filename);
  gt_gff3_in_stream_check_id_attributes((GtGFF3InStream*) gff3_in_stream);

  if (!had_err)
    had_err = gt_node_stream_pull(gff3_in_stream, err);

  if (!had_err)
    printf("input is valid GFF3\n");

  gt_node_stream_delete(gff3_in_stream);
  gt_type_checker_delete(type_checker);*/
}
