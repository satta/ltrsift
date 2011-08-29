#include "treeview_main.h"
#include "unused.h"

static int add_feature_columns(void *key, void *value, void *ltrgui, GT_UNUSED GtError *err)
{
  GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
  GtkTreeViewColumn *column;
  const char *caption = (const char*) key;
  unsigned long num = (unsigned long) value;
  GUIData *gui = (GUIData*) ltrgui;

//  g_warning("num: %lu", num);

  column = gtk_tree_view_column_new_with_attributes(caption, renderer, "text",
                                                    num, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(gui->tv_main), column);

  return 0;
}

void tv_main_init(GUIData *ltrgui, GtArray *nodes, GtHashmap *features,
                  unsigned long noc)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkTreeIter iter;
  GtkTreeStore *store;
  GType *types;
  GtError *err = NULL;
  unsigned long i;

  types = g_new0(GType, noc);

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

  gt_hashmap_foreach(features, add_feature_columns, (void*) ltrgui, err);

  types[0] = G_TYPE_STRING;
  types[1] = G_TYPE_STRING;
  types[2] = G_TYPE_ULONG;
  types[3] = G_TYPE_ULONG;

  for (i = TV_MAIN_N_COLUMS; i < noc; i++) {
    types[i] = G_TYPE_UINT;
  }

  store = gtk_tree_store_newv(noc, types);

  for (i = 0; i < gt_array_size(nodes); i++) {
    GtGenomeNode **gn;
    GtFeatureNode *curnode;
    GtFeatureNodeIterator *fni;
    const char *fnt;
    gn = *(GtGenomeNode***) gt_array_get(nodes, i);

    fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);

    while ((curnode = gt_feature_node_iterator_next(fni))) {
      fnt = gt_feature_node_get_type(curnode);
      if ((g_strcmp0(fnt, "repeat_region") == 0)) {
        char *seqid;
        GtRange range;
        seqid = gt_str_get(gt_genome_node_get_seqid((GtGenomeNode*) curnode));
        range = gt_genome_node_get_range((GtGenomeNode*) curnode);
        gtk_tree_store_append(store, &iter, NULL);
        gtk_tree_store_set(store, &iter,
                           TV_MAIN_SEQID, seqid,
                           TV_MAIN_TYPE, "",
                           TV_MAIN_START, range.start,
                           TV_MAIN_END, range.end,
                           -1);
      } else if (g_strcmp0(fnt, "protein_match") == 0) {
        const char *attr = gt_feature_node_get_attribute(curnode, "pfamname");

        if (gt_hashmap_get(features, attr) != NULL) {
          const char *clid;
          unsigned long cno, cval;
          clid = gt_feature_node_get_attribute(curnode, "clid");
          sscanf(clid, "%lu", &cval);
          cno = (unsigned long) gt_hashmap_get(features, attr);
          gtk_tree_store_set(store, &iter,
                             cno, cval,
                             -1);
        }
      } else if ((gt_hashmap_get(features, fnt)) != NULL) {
        const char *clid;
        unsigned long cno, cval;
        clid = gt_feature_node_get_attribute(curnode, "clid");
        sscanf(clid, "%lu", &cval);
        cno = (unsigned long) gt_hashmap_get(features, fnt);
        gtk_tree_store_set(store, &iter,
                           cno, cval,
                           -1);
      }
    }
    gt_feature_node_iterator_delete(fni);
  }

  gtk_tree_view_set_model(GTK_TREE_VIEW(ltrgui->tv_main), GTK_TREE_MODEL(store));
  gtk_tree_view_set_headers_clickable(GTK_TREE_VIEW(ltrgui->tv_main), TRUE);
  g_object_unref(store);

  gtk_container_add(GTK_CONTAINER(ltrgui->sw_main), ltrgui->tv_main);
  gtk_widget_show(ltrgui->tv_main);

  g_free(types);
}
