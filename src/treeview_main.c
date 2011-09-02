#include "treeview_main.h"
#include "unused.h"

static char *double_underscores(const char *str)
{
  char **arr;
  char *ret;

  arr = g_strsplit (str, "_", 0);
  ret = g_strjoinv ("__", arr);
  g_strfreev (arr);

  return ret;
}


static int add_feature_columns(void *key, void *value, void *ltrgui,
                               GT_UNUSED GtError *err)
{
  GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
  GtkTreeViewColumn *column;
  const char *caption = double_underscores((const char*) key);
  unsigned long num = (unsigned long) value;
  GUIData *gui = (GUIData*) ltrgui;

  g_object_set(renderer,
                "cell-background", "Orange",
                "cell-background-set", TRUE,
                "xalign", 1.0,
                NULL);

  column = gtk_tree_view_column_new_with_attributes(caption, renderer, "text",
                                                    num, NULL);
  gtk_tree_view_column_set_clickable(column, true);
  gtk_tree_view_append_column(GTK_TREE_VIEW(gui->tv_main), column);

  return 0;
}

static void tw_main_append_child(GUIData *ltrgui,
                                 GtkTreeModel *model,
                                 GtkTreeIter *child,
                                 GtkTreeIter *iter,
                                 GtFeatureNode *curnode,
                                 const char *type,
                                 const char *fnt)
{
  const char *clid;/*, *seqid;*/
  unsigned long cno;
  GtRange range;
/*seqid = gt_str_get(gt_genome_node_get_seqid((GtGenomeNode*) curnode));*/
  range = gt_genome_node_get_range((GtGenomeNode*) curnode);
  clid = gt_feature_node_get_attribute(curnode, ATTR_CLUSTID);
  cno = (unsigned long) gt_hashmap_get(ltrgui->features, type);
  gtk_tree_store_append(GTK_TREE_STORE(model), child, iter);
  gtk_tree_store_set(GTK_TREE_STORE(model), child,
                   /*  TV_MAIN_SEQID, seqid,*/
                     TV_MAIN_TYPE, fnt,
                     TV_MAIN_START, range.start,
                     TV_MAIN_END, range.end,
                     cno, clid,
                     -1);
}


void tw_main_row_collapsed(GtkTreeView *tree_view, GtkTreeIter *iter,
                          G_UNUSED GtkTreePath *path, G_UNUSED GUIData *ltrgui)
{
  GtkTreeIter child;
  GtkTreeModel *model;

  model = gtk_tree_view_get_model(tree_view);
  gtk_tree_model_iter_children(model, &child, iter);
  while (gtk_tree_store_remove(GTK_TREE_STORE(model), &child))
    continue;
  gtk_tree_store_append(GTK_TREE_STORE(model), &child, iter);
}

void tw_main_row_expanded(GtkTreeView *tree_view, GtkTreeIter *iter,
                          GtkTreePath *path, G_UNUSED GUIData *ltrgui)
{
  GtkTreeModel *model;
  GtkTreeIter child, tmp1, tmp2;
  GtGenomeNode **gn;
  GtFeatureNodeIterator *fni;
  GtFeatureNode *curnode;
  const char *fnt, *tmp = NULL;
  gboolean first_ltr = true;

  model = gtk_tree_view_get_model(tree_view);
  /* get iter for first row, will be needed to remove empty row (see below) */
  gtk_tree_model_get_iter(model, &tmp1, path);
  /* get GtGenomeNode */
  gtk_tree_model_get(model, iter,
                     TV_MAIN_NODE, &gn, -1);
  fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);

  while ((curnode = gt_feature_node_iterator_next(fni))) {
    fnt = gt_feature_node_get_type(curnode);
    if (g_strcmp0(fnt, FNT_PROTEINM) == 0) {
      const char *attr = gt_feature_node_get_attribute(curnode, ATTR_PFAMN);
      if (gt_hashmap_get(ltrgui->features, attr) != NULL)
        tw_main_append_child(ltrgui, model, &child, iter, curnode, attr, fnt);
    } else if (g_strcmp0(fnt, FNT_LTR) == 0) {
      if (first_ltr) {
        tmp = FNT_LLTR;
        first_ltr = false;
      } else
        tmp = FNT_RLTR;
      if (gt_hashmap_get(ltrgui->features, tmp) != NULL)
        tw_main_append_child(ltrgui, model, &child, iter, curnode, tmp, fnt);
    } else {
      if (gt_hashmap_get(ltrgui->features, fnt) != NULL)
        tw_main_append_child(ltrgui, model, &child, iter, curnode, fnt, fnt);
    }
  }
  gt_feature_node_iterator_delete(fni);
  /* remove empty row */
  gtk_tree_model_iter_children(model, &tmp2, &tmp1);
  gtk_tree_store_remove(GTK_TREE_STORE(model), &tmp2);
}

void tv_main_init(GUIData *ltrgui, GtArray *nodes, unsigned long noc)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkTreeIter iter, child;
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
  g_object_set(renderer, "xalign", 1.0, NULL);
  column = gtk_tree_view_column_new_with_attributes(TV_MAIN_CAPTION_START,
                                                    renderer, "text",
                                                    TV_MAIN_START, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrgui->tv_main), column);

  renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "xalign", 1.0, NULL);
  column = gtk_tree_view_column_new_with_attributes(TV_MAIN_CAPTION_END,
                                                    renderer, "text",
                                                    TV_MAIN_END, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrgui->tv_main), column);

  gt_hashmap_foreach(ltrgui->features, add_feature_columns, (void*) ltrgui,
                     err);

  column = gtk_tree_view_column_new();
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrgui->tv_main), column);

  types[0] = G_TYPE_POINTER;
  types[1] = G_TYPE_STRING;
  types[2] = G_TYPE_STRING;
  types[3] = G_TYPE_ULONG;
  types[4] = G_TYPE_ULONG;
  types[5] = G_TYPE_STRING;

  for (i = TV_MAIN_N_COLUMS; i < noc; i++) {
    types[i] = G_TYPE_STRING;
  }

  store = gtk_tree_store_newv(noc, types);

  for (i = 0; i < gt_array_size(nodes); i++) {
    GtGenomeNode **gn;
    GtFeatureNode *curnode;
    GtFeatureNodeIterator *fni;
    const char *fnt;
    gboolean first_ltr = true;
    gn = *(GtGenomeNode***) gt_array_get(nodes, i);

    fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);

    while ((curnode = gt_feature_node_iterator_next(fni))) {
      fnt = gt_feature_node_get_type(curnode);
      if ((g_strcmp0(fnt, FNT_REPEATR) == 0)) {
        char *seqid;
        GtRange range;
        seqid = gt_str_get(gt_genome_node_get_seqid((GtGenomeNode*) curnode));
        range = gt_genome_node_get_range((GtGenomeNode*) curnode);
        gtk_tree_store_append(store, &iter, NULL);
        gtk_tree_store_set(store, &iter,
                           TV_MAIN_NODE, gn,
                           TV_MAIN_SEQID, seqid,
                           TV_MAIN_START, range.start,
                           TV_MAIN_END, range.end,
                           -1);
        /* append an empty row for row-expand functionality */
        gtk_tree_store_append(store, &child, &iter);
      } else if (g_strcmp0(fnt, FNT_PROTEINM) == 0) {
        const char *attr = gt_feature_node_get_attribute(curnode, ATTR_PFAMN);

        if (gt_hashmap_get(ltrgui->features, attr) != NULL) {
          const char *clid;
          unsigned long cno;
          clid = gt_feature_node_get_attribute(curnode, ATTR_CLUSTID);
          cno = (unsigned long) gt_hashmap_get(ltrgui->features, attr);
          gtk_tree_store_set(store, &iter,
                             cno, clid,
                             -1);
        }
      } else if (g_strcmp0(fnt, FNT_LTR) == 0) {
        const char *tmp;
        if (first_ltr) {
          tmp = FNT_LLTR;
          first_ltr = false;
        } else
          tmp = FNT_RLTR;
        if (gt_hashmap_get(ltrgui->features, tmp) != NULL) {
          const char *clid;
          unsigned long cno;
          clid = gt_feature_node_get_attribute(curnode, ATTR_CLUSTID);
          cno = (unsigned long) gt_hashmap_get(ltrgui->features, tmp);
          gtk_tree_store_set(store, &iter,
                             cno, clid,
                             -1);
        }
      } else if ((gt_hashmap_get(ltrgui->features, fnt)) != NULL) {
        const char *clid;
        unsigned long cno;
        clid = gt_feature_node_get_attribute(curnode, ATTR_CLUSTID);
        cno = (unsigned long) gt_hashmap_get(ltrgui->features, fnt);
        gtk_tree_store_set(store, &iter,
                           cno, clid,
                           -1);
      }
    }
    gt_feature_node_iterator_delete(fni);
  }

  gtk_tree_view_set_model(GTK_TREE_VIEW(ltrgui->tv_main),
                          GTK_TREE_MODEL(store));
  g_object_unref(store);

  gtk_container_add(GTK_CONTAINER(ltrgui->sw_main), ltrgui->tv_main);
  g_signal_connect(G_OBJECT(ltrgui->tv_main), "row-expanded",
                   G_CALLBACK(tw_main_row_expanded), ltrgui);
  g_signal_connect(G_OBJECT(ltrgui->tv_main), "row-collapsed",
                   G_CALLBACK(tw_main_row_collapsed), ltrgui);
  gtk_widget_show(ltrgui->tv_main);

  g_free(types);
}
