#include "gtk_ltr_family.h"
#include "unused.h"

static char* double_underscores(const char *str)
{
  char **arr;
  char *ret;

  arr = g_strsplit(str, "_", 0);
  ret = g_strjoinv("__", arr);
  g_strfreev(arr);

  return ret;
}

static int add_feature_columns(void *key, void *value, void *ltrfam,
                               GT_UNUSED GtError *err)
{
  GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
  GtkTreeViewColumn *column;
  const char *caption = double_underscores((const char*) key);
  unsigned long num = (unsigned long) value;
  GtkLTRFamily *fam = (GtkLTRFamily*) ltrfam;

  g_object_set(renderer,
               "cell-background", "Orange",
               "cell-background-set", TRUE,
               "xalign", 1.0,
               NULL);

  column = gtk_tree_view_column_new_with_attributes(caption, renderer, "text",
                                                    num, NULL);
  gtk_tree_view_column_set_clickable(column, true);
  gtk_tree_view_append_column(GTK_TREE_VIEW(fam->list_view), column);

  return 0;
}

static void append_child(GtkTreeStore *store,
                                 GtkTreeIter *child,
                                 GtkTreeIter *iter,
                                 GtFeatureNode *curnode,
                                 const char *fnt)
{
  const char *clid;
  GtRange range;
  range = gt_genome_node_get_range((GtGenomeNode*) curnode);
  clid = gt_feature_node_get_attribute(curnode, ATTR_CLUSTID);
  gtk_tree_store_append(store, child, iter);
  gtk_tree_store_set(store, child,
                     LTRFAM_TV_TYPE, fnt,
                     LTRFAM_TV_START, range.start,
                     LTRFAM_TV_END, range.end,
                     LTRFAM_TV_INFO, clid,
                     -1);
}

GtkWidget* gtk_ltr_family_get_list_view(GtkLTRFamily *ltrfam)
{
  return ltrfam->list_view;
}

static void list_view_row_activated(GtkTreeView *list_view,
                                    GtkTreePath *path,
                                    G_UNUSED GtkTreeViewColumn *column,
                                    GtkLTRFamily *ltrfam)
{
  GtkTreeModel *list_model, *tree_model;
  GtkTreeIter iter, tmp, child;
  GtkTreeStore *store;
  GtGenomeNode **gn;
  GtFeatureNodeIterator *fni;
  GtFeatureNode *curnode;
  const char *fnt;

  list_model = gtk_tree_view_get_model(list_view);
  /* get GtGenomeNode */
  gtk_tree_model_get_iter(list_model, &tmp, path);
  gtk_tree_model_get(list_model, &tmp, LTRFAM_LV_NODE, &gn, -1);

  tree_model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfam->tree_view));
  if (!tree_model) {
    GType *types = g_new0(GType, LTRFAM_TV_N_COLUMS);

    types[0] = G_TYPE_STRING;
    types[1] = G_TYPE_ULONG;
    types[2] = G_TYPE_ULONG;
    types[3] = G_TYPE_STRING;

    store = gtk_tree_store_newv(LTRFAM_TV_N_COLUMS, types);

    gtk_tree_view_set_model(GTK_TREE_VIEW(ltrfam->tree_view),
                            GTK_TREE_MODEL(store));
    g_object_unref(store);

    g_free(types);
  } else {
    store = GTK_TREE_STORE(tree_model);
    gtk_tree_store_clear(store);
  }

  tree_model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfam->tree_view));
  fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);

  while ((curnode = gt_feature_node_iterator_next(fni))) {
    fnt = gt_feature_node_get_type(curnode);
    if (g_strcmp0(fnt, FNT_REPEATR) == 0) {
      GtRange range;
      range = gt_genome_node_get_range((GtGenomeNode*) curnode);
      gtk_tree_store_append(store, &iter, NULL);
      gtk_tree_store_set(store, &iter,
                         LTRFAM_TV_TYPE, fnt,
                         LTRFAM_TV_START, range.start,
                         LTRFAM_TV_END, range.end,
                         -1);
    } else
      append_child(store, &child, &iter, curnode, fnt);
  }
  gtk_tree_view_expand_all(GTK_TREE_VIEW(ltrfam->tree_view));
  gt_feature_node_iterator_delete(fni);
}

static void gtk_ltr_family_list_view_new(GtkLTRFamily *ltrfam,
                                         GtArray *nodes,
                                         GtHashmap *features,
                                         unsigned long noc)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkTreeIter iter;
  GtkListStore *store;
  GType *types;
  GtError *err = NULL;
  unsigned long i;

  types = g_new0(GType, noc);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(LTRFAM_LV_CAPTION_SEQID,
                                                    renderer, "text",
                                                    LTRFAM_LV_SEQID, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrfam->list_view), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(LTRFAM_LV_CAPTION_TYPE,
                                                    renderer, "text",
                                                    LTRFAM_LV_TYPE, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrfam->list_view), column);

  renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "xalign", 1.0, NULL);
  column = gtk_tree_view_column_new_with_attributes(LTRFAM_LV_CAPTION_START,
                                                    renderer, "text",
                                                    LTRFAM_LV_START, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrfam->list_view), column);

  renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "xalign", 1.0, NULL);
  column = gtk_tree_view_column_new_with_attributes(LTRFAM_LV_CAPTION_END,
                                                    renderer, "text",
                                                    LTRFAM_LV_END, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrfam->list_view), column);

  gt_hashmap_foreach(features, add_feature_columns, (void*) ltrfam, err);

  column = gtk_tree_view_column_new();
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrfam->list_view), column);

  types[0] = G_TYPE_POINTER;
  types[1] = G_TYPE_STRING;
  types[2] = G_TYPE_STRING;
  types[3] = G_TYPE_ULONG;
  types[4] = G_TYPE_ULONG;

  for (i = LTRFAM_LV_N_COLUMS; i < noc; i++) {
    types[i] = G_TYPE_STRING;
  }

  store = gtk_list_store_newv(noc, types);

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
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           LTRFAM_LV_NODE, gn,
                           LTRFAM_LV_SEQID, seqid,
                           LTRFAM_LV_START, range.start,
                           LTRFAM_LV_END, range.end,
                           -1);
      } else if (g_strcmp0(fnt, FNT_PROTEINM) == 0) {
        const char *attr = gt_feature_node_get_attribute(curnode, ATTR_PFAMN);

        if (gt_hashmap_get(features, attr) != NULL) {
          const char *clid;
          unsigned long cno;
          clid = gt_feature_node_get_attribute(curnode, ATTR_CLUSTID);
          cno = (unsigned long) gt_hashmap_get(features, attr);
          gtk_list_store_set(store, &iter,
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
        if (gt_hashmap_get(features, tmp) != NULL) {
          const char *clid;
          unsigned long cno;
          clid = gt_feature_node_get_attribute(curnode, ATTR_CLUSTID);
          cno = (unsigned long) gt_hashmap_get(features, tmp);
          gtk_list_store_set(store, &iter,
                             cno, clid,
                             -1);
        }
      } else if ((gt_hashmap_get(features, fnt)) != NULL) {
        const char *clid;
        unsigned long cno;
        clid = gt_feature_node_get_attribute(curnode, ATTR_CLUSTID);
        cno = (unsigned long) gt_hashmap_get(features, fnt);
        gtk_list_store_set(store, &iter,
                           cno, clid,
                           -1);
      }
    }
    gt_feature_node_iterator_delete(fni);
  }
  gtk_tree_view_set_model(GTK_TREE_VIEW(ltrfam->list_view),
                          GTK_TREE_MODEL(store));
  g_object_unref(store);
  g_signal_connect(G_OBJECT(ltrfam->list_view), "row-activated",
                   G_CALLBACK(list_view_row_activated), ltrfam);
  g_free(types);
}

void gtk_ltr_family_tree_view_new(GtkLTRFamily *ltrfam)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(LTRFAM_LV_CAPTION_TYPE,
                                                    renderer, "text",
                                                    LTRFAM_TV_TYPE, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrfam->tree_view), column);

  renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "xalign", 1.0, NULL);
  column = gtk_tree_view_column_new_with_attributes(LTRFAM_LV_CAPTION_START,
                                                    renderer, "text",
                                                    LTRFAM_TV_START, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrfam->tree_view), column);

  renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "xalign", 1.0, NULL);
  column = gtk_tree_view_column_new_with_attributes(LTRFAM_LV_CAPTION_END,
                                                    renderer, "text",
                                                    LTRFAM_TV_END, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrfam->tree_view), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(LTRFAM_TV_CAPTION_INFO,
                                                    renderer, "text",
                                                    LTRFAM_TV_INFO, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrfam->tree_view), column);
}

/* maybe add some signal-stuff here */
/* static void gtk_ltr_family_class_init(GtkLTRFamilyClass *class)
{
  GtkObjectClass *object_class;

  object_class = (GtkObjectClass*) class;

  class->gtk_ltr_family = NULL;
}*/

static void gtk_ltr_family_init(GtkLTRFamily *ltrfam)
{
  GtkWidget *sw1, *sw2, *hbox, *hsep, *vsep, *label1, *label2, *vbox1, *vbox2;

  gtk_box_set_homogeneous(GTK_BOX(ltrfam), FALSE);
  gtk_box_set_spacing(GTK_BOX(ltrfam), 0);

  hbox = gtk_hbox_new(FALSE, 0);
  hsep = gtk_hseparator_new();
  vsep = gtk_vseparator_new();

  /* ltrfam->label is just for tests */
  ltrfam->label = gtk_label_new("TEST IMAGE-SECTION");
  label1 = gtk_label_new(LTRFAM_DETINFO);
  label2 = gtk_label_new(LTRFAM_IMAGE);

  vbox1 = gtk_vbox_new(FALSE, 0);
  vbox2 = gtk_vbox_new(FALSE, 0);

  sw1 = gtk_scrolled_window_new(NULL, NULL);
  sw2 = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw1),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw2),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);

  ltrfam->list_view = gtk_tree_view_new();
  gtk_container_add(GTK_CONTAINER(sw1), ltrfam->list_view);
  gtk_box_pack_start(GTK_BOX(ltrfam), sw1, TRUE, TRUE, 0);

  gtk_box_pack_start(GTK_BOX(ltrfam), hsep, FALSE, TRUE, 5);

  gtk_box_pack_start(GTK_BOX(vbox1), label1, FALSE, TRUE, 0);
  ltrfam->tree_view = gtk_tree_view_new();
  gtk_container_add(GTK_CONTAINER(sw2), ltrfam->tree_view);
  gtk_box_pack_start(GTK_BOX(vbox1), sw2, TRUE, TRUE, 0);

  gtk_box_pack_start(GTK_BOX(vbox2), label2, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox2), ltrfam->label, TRUE, TRUE, 0);

  gtk_box_pack_start(GTK_BOX(hbox), vbox1, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), vsep, FALSE, TRUE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), vbox2, TRUE, TRUE, 0);

  gtk_box_pack_start(GTK_BOX(ltrfam), hbox, TRUE, TRUE, 0);
  gtk_widget_show_all(GTK_WIDGET(ltrfam));
}

GType gtk_ltr_family_get_type(void)
{
  static GType ltrfam_type = 0;

  if (!ltrfam_type) {
    const GTypeInfo ltrfam_info =
    {
      sizeof (GtkLTRFamilyClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      NULL, /* (GClassInitFunc) gtk_ltr_family_class_init, */
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (GtkLTRFamily),
      0,    /* n_preallocs */
      (GInstanceInitFunc) gtk_ltr_family_init,
    };
    ltrfam_type = g_type_register_static(GTK_TYPE_VBOX, "GtkLTRFamily",
                                         &ltrfam_info, 0);
  }
  return ltrfam_type;
}

GtkWidget* gtk_ltr_family_new(GtHashmap *features,
                              GtArray *nodes,
                              unsigned long noc)
{
  GtkLTRFamily *ltrfam;
  gt_assert(features);
  /* no gt_assert(nodes); due to tests */
  ltrfam = gtk_type_new(GTKLTRFAMILY_TYPE);

  gtk_ltr_family_list_view_new(ltrfam, nodes, features, noc);
  gtk_ltr_family_tree_view_new(ltrfam);

  return GTK_WIDGET(ltrfam);
}
