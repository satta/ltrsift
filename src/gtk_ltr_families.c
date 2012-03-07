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

#include <string.h>
#include "error.h"
#include "gtk_ltr_families.h"
#include "message_strings.h"
#include "statusbar.h"
#include "support.h"

/* function prototypes start */
static void gtk_ltr_families_nb_fam_refresh_nums(gpointer);

static gint nb_fam_lv_sort_function(GtkTreeModel*,
                                    GtkTreeIter*,
                                    GtkTreeIter*,
                                    gpointer);
/* function prototypes end */

/* get functions start */
GtkNotebook* gtk_ltr_families_get_nb(GtkLTRFamilies *ltrfams)
{
  return GTK_NOTEBOOK(ltrfams->nb_family);
}

GtkTreeView* gtk_ltr_families_get_list_view_families(GtkLTRFamilies *ltrfams)
{
  return GTK_TREE_VIEW(ltrfams->lv_families);
}

GtArray* gtk_ltr_families_get_nodes(GtkLTRFamilies *ltrfams)
{
  return ltrfams->nodes;
}

gboolean gtk_ltr_families_get_modified(GtkLTRFamilies *ltrfams)
{
  return ltrfams->modified;
}

gchar* gtk_ltr_families_get_projectfile(GtkLTRFamilies *ltrfams)
{
  return ltrfams->projectfile;
}

gint gtk_ltr_families_get_position(GtkLTRFamilies *ltrfams)
{
  return gtk_paned_get_position(GTK_PANED(ltrfams));
}

gint gtk_ltr_families_get_hpaned_position(GtkLTRFamilies *ltrfams)
{
  return gtk_paned_get_position(GTK_PANED(ltrfams->hpaned));
}

gint gtk_ltr_families_get_vpaned_position(GtkLTRFamilies *ltrfams)
{
  return gtk_paned_get_position(GTK_PANED(ltrfams->vpaned));
}
/* get functions end */

/* set functions start */
void gtk_ltr_families_set_modified(GtkLTRFamilies *ltrfams, gboolean modified)
{
  ltrfams->modified = modified;

  GtkWidget *toplevel = gtk_widget_get_toplevel(GTK_WIDGET(ltrfams));
  gchar buffer[BUFSIZ], *f;
  f = ltrfams->projectfile;
  if (gtk_widget_is_toplevel(toplevel)) {
    switch (modified) {
      case TRUE:
        g_snprintf(buffer, BUFSIZ, "%s - *%s",
                   GUI_NAME,
                   f ? g_path_get_basename(ltrfams->projectfile) : "UNSAVED");
        gtk_window_set_title(GTK_WINDOW(toplevel), buffer);
        break;
      case FALSE:
        g_snprintf(buffer, BUFSIZ, "%s - %s",
                   GUI_NAME,
                   f ? g_path_get_basename(ltrfams->projectfile) : "UNSAVED");
        gtk_window_set_title(GTK_WINDOW(toplevel), buffer);
        break;
      default:
        break;
    }
  }
}

void gtk_ltr_families_set_projectfile(GtkLTRFamilies *ltrfams,
                                      gchar *projectfile)
{
  if (ltrfams->projectfile)
    g_free(ltrfams->projectfile);
  ltrfams->projectfile = projectfile;
}

void gtk_ltr_families_set_position(GtkLTRFamilies *ltrfams, gint pos)
{
  gtk_paned_set_position(GTK_PANED(ltrfams), pos);
}

void gtk_ltr_families_set_hpaned_position(GtkLTRFamilies *ltrfams, gint pos)
{
  gtk_paned_set_position(GTK_PANED(ltrfams->hpaned), pos);
}

void gtk_ltr_families_set_vpaned_position(GtkLTRFamilies *ltrfams, gint pos)
{
  gtk_paned_set_position(GTK_PANED(ltrfams->vpaned), pos);
}

void gtk_ltr_families_set_filter_widget(GtkLTRFamilies *ltrfams,
                                        GtkWidget *ltrfilt)
{
  ltrfams->ltrfilt = ltrfilt;
}
/* set functions end */

/* "support" functions start */
void update_main_tab_label(GtkLTRFamilies *ltrfams)
{
  GtkWidget *tab_label, *child;
  gfloat percent;
  gchar text[BUFSIZ];
  gint main_tab_no;
  unsigned long size;

  main_tab_no = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(ltrfams->nb_family),
                                                  "main_tab"));
  child = gtk_notebook_get_nth_page(GTK_NOTEBOOK(ltrfams->nb_family),
                                    main_tab_no);
  tab_label = gtk_notebook_get_tab_label(GTK_NOTEBOOK(ltrfams->nb_family),
                                         child);
  size = gt_array_size(ltrfams->nodes);
  if (size == 0)
    size = 1;
  percent = ((gfloat) ltrfams->unclassified_cands /
             (gfloat) size) * 100.0;
  g_snprintf(text, BUFSIZ, "%s (%.1f%%)", MAIN_TAB_LABEL, percent);
  gtk_label_close_set_text(GTKLABELCLOSE(tab_label), text);
}

void gtk_ltr_families_update_unclass_cands(GtkLTRFamilies *ltrfams,
                                         long int amount)
{
  gchar sb_text[BUFSIZ];

  ltrfams->unclassified_cands += amount;
  update_main_tab_label(ltrfams);
  g_snprintf(sb_text, BUFSIZ, STATUSBAR_NUM_OF_CANDS,
             gt_array_size(ltrfams->nodes));
  statusbar_set_status(ltrfams->statusbar, sb_text);
}

static gboolean prefix_in_list_view_fams(GtkTreeModel *model,
                                         const gchar *prefix)
{
  GtkTreeIter iter;
  gchar *name;
  gboolean valid;

  valid = gtk_tree_model_get_iter_first(model, &iter);
  if (valid) {
    gtk_tree_model_get(model, &iter, LTRFAMS_FAM_LV_OLDNAME, &name, -1);
    if (g_str_has_prefix(name, prefix)) {
      g_free(name);
      return TRUE;
    }
    while (gtk_tree_model_iter_next(model, &iter)) {
      gtk_tree_model_get(model, &iter, LTRFAMS_FAM_LV_OLDNAME, &name, -1);
      if (g_str_has_prefix(name, prefix)) {
        g_free(name);
        return TRUE;
      }
      g_free(name);
    }
  }
  return FALSE;
}

static int fill_feature_list(void *key, GT_UNUSED void *value,
                             void *lv, GT_UNUSED GtError *err)
{
  GtkTreeView *list_view;
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *feature = (gchar*) key;

  list_view = (GtkTreeView*) lv;
  model = gtk_tree_view_get_model(list_view);
  gtk_list_store_append(GTK_LIST_STORE(model), &iter);
  gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                     0, feature,
                     -1);
  return 0;
}

static void dialog_selection_changed(GtkTreeSelection *sel, GtkWidget *dialog)
{
  if (gtk_tree_selection_count_selected_rows(sel) == 0)
    gtk_dialog_set_response_sensitive(GTK_DIALOG(dialog), GTK_RESPONSE_OK,
                                      FALSE);
  else
    gtk_dialog_set_response_sensitive(GTK_DIALOG(dialog), GTK_RESPONSE_OK,
                                      TRUE);
}

static gboolean
get_features_and_prefix_for_classification(GtkLTRFamilies *ltrfams,
                                           GtHashmap *sel_features,
                                           gchar **prefix)
{
  GtkWidget *label, *list_view, *vbox, *sw, *dialog, *entry, *toplevel;
  GtkTreeSelection *sel;
  GtkTreeViewColumn *column;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkCellRenderer *renderer;
  GtkListStore *store;
  GList *rows, *tmp;
  gchar *feature_name;

  toplevel = gtk_widget_get_toplevel(GTK_WIDGET(ltrfams));
  dialog = gtk_dialog_new_with_buttons(INFORMATION, GTK_WINDOW(toplevel),
                                        GTK_DIALOG_MODAL,
                                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                        GTK_STOCK_OK, GTK_RESPONSE_OK,
                                        NULL);
  gtk_dialog_set_has_separator(GTK_DIALOG(dialog), FALSE);
  gtk_dialog_set_response_sensitive(GTK_DIALOG(dialog), GTK_RESPONSE_OK, FALSE);
  label = gtk_label_new ("Select features for classification");
  list_view = gtk_tree_view_new();
  sw = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Features",
                                                    renderer,
                                                    "text",
                                                    0,
                                                    NULL);
  gtk_tree_view_column_set_sort_column_id(column, 0);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);
  store = gtk_list_store_new(1, G_TYPE_STRING);
  gtk_tree_view_set_model(GTK_TREE_VIEW(list_view), GTK_TREE_MODEL(store));
  g_object_unref(store);
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_view));
  gtk_tree_selection_set_mode(sel,GTK_SELECTION_MULTIPLE);
  g_signal_connect(G_OBJECT(sel), "changed",
                   G_CALLBACK(dialog_selection_changed), dialog);
  gt_hashmap_foreach(ltrfams->features, fill_feature_list,
                     (void*) list_view, ltrfams->err);
  gtk_container_add(GTK_CONTAINER(sw), list_view);
  vbox = gtk_vbox_new(FALSE, 5);
  gtk_container_set_border_width(GTK_CONTAINER(vbox), 2);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), sw, TRUE, TRUE, 1);
  label = gtk_label_new("Select prefix for new families:");
  entry = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(entry), "newfam_");
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 1);
  gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), vbox);

  gtk_window_resize(GTK_WINDOW(dialog), 200, 350);
  gtk_widget_show_all(dialog);
  if (gtk_dialog_run(GTK_DIALOG(dialog)) != GTK_RESPONSE_OK) {
    gtk_widget_destroy(dialog);
    return FALSE;
  }
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_view));
  if (gtk_tree_selection_count_selected_rows(sel) == 0) {
    gtk_widget_destroy(dialog);
    return FALSE;
  }
  *prefix = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(list_view));
  rows = gtk_tree_selection_get_selected_rows(sel, &model);
  tmp = rows;
  while (tmp != NULL) {
    gtk_tree_model_get_iter(model, &iter, (GtkTreePath*) tmp->data);
    gtk_tree_model_get(model, &iter,
                       0, &feature_name,
                       -1);
    gt_hashmap_add(sel_features, (void*) gt_cstr_dup(feature_name),
                   (void*) 1);
    g_free(feature_name);
    tmp = tmp->next;
  }
  g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
  g_list_free(rows);
  gtk_widget_destroy(dialog);
  return TRUE;
}

void free_tdata(FamilyTransferData *tdata)
{
  g_list_foreach(tdata->references, (GFunc) gtk_tree_row_reference_free, NULL);
  g_list_free(tdata->references);
  gt_array_delete(tdata->nodes);
  tdata->rowref = NULL;
  tdata->list_view = NULL;
  g_slice_free(FamilyTransferData, tdata);
}

void set_cell_colors(GList *columns, GtHashmap *colors)
{
  GtkTreeViewColumn *column;
  GtkCellRenderer *renderer;
  const char *hexcode;
  GList *tmp1, *tmp2;

  tmp1 = columns;
  while (tmp1 != NULL) {
    column = (GtkTreeViewColumn*) tmp1->data;
    hexcode =
           (const char*) gt_hashmap_get(colors,
                                        gtk_tree_view_column_get_title(column));
    if (hexcode != NULL) {
      tmp2 = gtk_cell_layout_get_cells(GTK_CELL_LAYOUT(column));
      renderer = (GtkCellRenderer*) g_list_nth_data(tmp2, 0);
      g_object_set(renderer,
                   "cell-background", hexcode,
                   "cell-background-set", TRUE,
                   NULL);
      g_list_free(tmp2);
    }
    tmp1 = tmp1->next;
  }
}

static void update_list_view_with_flcand(GtkListStore *store,
                                         GtkTreeIter *iter,
                                         GtGenomeNode *gn)
{
  GtFeatureNode *curnode;
  GtFeatureNodeIterator *fni;
  const char *attr;

  fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);
  curnode = gt_feature_node_iterator_next(fni);
  attr = gt_feature_node_get_attribute(curnode, ATTR_FULLLEN);
  gtk_list_store_set(store, iter, LTRFAMS_LV_FLCAND, (attr ? "*" : ""), -1);
  gt_feature_node_iterator_delete(fni);
}

static void remove_candidate(GtkTreeRowReference *rowref)
{
  CandidateData *cdata;
  GtGenomeNode *gn;
  GtkTreeIter iter;
  GtkTreePath *path;
  GtkTreeModel *model;
  gboolean valid;

  model = gtk_tree_row_reference_get_model(rowref);
  path = gtk_tree_row_reference_get_path(rowref);
  valid = gtk_tree_model_get_iter(model, &iter, path);
  if (!valid)
    g_warning("%s", "Remove candidate - Programming error!");
  else {
    gtk_tree_model_get(model, &iter, LTRFAMS_LV_NODE, &gn, -1);
    cdata = (CandidateData*) gt_genome_node_get_user_data(gn, "cdata");
    if (cdata) {
      cdata->cand_ref = NULL;
      cdata->fam_ref = NULL;
    }
    gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
  }
  gtk_tree_path_free(path);
}

static void remove_merged_family(GtkTreeRowReference *rowref,
                                 GtkLTRFamilies *ltrfams)
{
  GtArray *nodes;
  GtkTreePath *path;
  GtkTreeModel *model;
  GtkWidget *tab_label;
  GtkTreeIter iter;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfams->lv_families));
  path = gtk_tree_row_reference_get_path(rowref);
  gtk_tree_model_get_iter(model, &iter, path);
  gtk_tree_model_get(model, &iter,
                     LTRFAMS_FAM_LV_NODE_ARRAY, &nodes,
                     LTRFAMS_FAM_LV_TAB_LABEL, &tab_label,
                     -1);
  gt_array_delete(nodes);
  if (tab_label) {
    gtk_notebook_remove_page(GTK_NOTEBOOK(ltrfams->nb_family),
                GPOINTER_TO_INT(
                 gtk_label_close_get_button_data(GTKLABELCLOSE(tab_label),
                                                 "nbpage")));
    gtk_ltr_families_nb_fam_refresh_nums(ltrfams);
  }
  gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
  gtk_tree_path_free(path);
}

static void remove_family(GtkTreeRowReference *rowref, GtkLTRFamilies *ltrfams)
{
  GtArray *nodes;
  GList *children;
  GtkTreeView *list_view;
  GtkTreePath *path;
  GtkTreeModel *model;
  GtkWidget *main_tab, *tab_label;
  GtkTreeIter iter;
  GtGenomeNode *gn;
  GtFeatureNode *curnode;
  GtFeatureNodeIterator *fni;
  gint main_tab_no;
  const char *attr;
  unsigned long i;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfams->lv_families));
  path = gtk_tree_row_reference_get_path(rowref);
  gtk_tree_model_get_iter(model, &iter, path);
  gtk_tree_model_get(model, &iter,
                     LTRFAMS_FAM_LV_NODE_ARRAY, &nodes,
                     -1);
  ltrfams->unclassified_cands += gt_array_size(nodes);
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(ltrfams->nb_family)) != 0) {

    main_tab_no =
                 GPOINTER_TO_INT(g_object_get_data(G_OBJECT(ltrfams->nb_family),
                                                   "main_tab"));
    main_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(ltrfams->nb_family),
                                         main_tab_no);
    children = gtk_container_get_children(GTK_CONTAINER(main_tab));
    list_view = GTK_TREE_VIEW(g_list_first(children)->data);
    for (i = 0; i < gt_array_size(nodes); i++) {
      gn = *(GtGenomeNode**) gt_array_get(nodes, i);
      fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);
      curnode = gt_feature_node_iterator_next(fni);
      attr = gt_feature_node_get_attribute(curnode, ATTR_LTRFAM);
      if (attr)
        gt_feature_node_remove_attribute(curnode, ATTR_LTRFAM);
      attr = gt_feature_node_get_attribute(curnode, ATTR_FULLLEN);
      if (attr)
        gt_feature_node_remove_attribute(curnode, ATTR_FULLLEN);
      gtk_ltr_families_nb_fam_lv_append_gn(ltrfams, list_view, gn, NULL, NULL,
                                           NULL, NULL);
      gt_feature_node_iterator_delete(fni);
    }
    gt_array_delete(nodes);
    gtk_tree_model_get(model, &iter,
                       LTRFAMS_FAM_LV_TAB_LABEL, &tab_label,
                       -1);
    if (tab_label) {
      gtk_notebook_remove_page(GTK_NOTEBOOK(ltrfams->nb_family),
                  GPOINTER_TO_INT(
                   gtk_label_close_get_button_data(GTKLABELCLOSE(tab_label),
                                                   "nbpage")));
      gtk_ltr_families_nb_fam_refresh_nums(ltrfams);
    }
    g_list_free(children);
  }
  gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
  gtk_tree_path_free(path);
}

static void update_genomenode_famname(GtArray *nodes, const char *newname)
{
  GtGenomeNode *gn;
  GtFeatureNode *curnode;
  GtFeatureNodeIterator *fni;
  unsigned long i;

  for (i = 0; i < gt_array_size(nodes); i++) {
    gn = *(GtGenomeNode**) gt_array_get(nodes, i);
    fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);
    curnode = gt_feature_node_iterator_next(fni);
    gt_feature_node_set_attribute(curnode, ATTR_LTRFAM, newname);
    gt_feature_node_iterator_delete(fni);
  }
}

static void remove_nodes_from_array(GtArray *nodes1, GtArray *nodes2,
                                    gboolean delete_gn)
{
  GtGenomeNode *tmp1, *tmp2;
  unsigned long i, j;

  for (i = 0; i < gt_array_size(nodes2); i++) {
    tmp2 = *(GtGenomeNode**) gt_array_get(nodes2, i);
    for (j = 0; j < gt_array_size(nodes1); j++) {
      tmp1 = *(GtGenomeNode**) gt_array_get(nodes1, j);
      if (gt_genome_node_cmp(tmp1, tmp2) == 0) {
        gt_array_rem(nodes1, j);
        if (delete_gn)
          delete_gt_genome_node(tmp1);
        break;
      }
    }
  }
}

char* double_underscores(const char *str)
{
  char **arr;
  char *ret;

  arr = g_strsplit(str, "_", 0);
  ret = g_strjoinv("__", arr);
  g_strfreev(arr);

  return ret;
}

void free_str_hash(void *elem)
{
  gt_free(elem);
}

static void free_iter_hash(void *elem)
{
  gtk_tree_iter_free((GtkTreeIter*) elem);
}

static void
gtk_ltr_families_clear_lv_det_on_equal_nodes(GtkLTRFamilies *ltrfams,
                                             GtGenomeNode *gn)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtGenomeNode *gn_tmp;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfams->tv_details));
  if (model) {
    gboolean valid = gtk_tree_model_get_iter_first(model, &iter);
    if (!valid)
      return;
    gtk_tree_model_get(model, &iter,
                       LTRFAMS_DETAIL_TV_NODE, &gn_tmp,
                       -1);
    if (gt_genome_node_cmp(gn_tmp, gn) == 0) {
      gtk_tree_store_clear(GTK_TREE_STORE(model));
      gt_diagram_delete(ltrfams->diagram);
      ltrfams->diagram = NULL;
      gtk_widget_queue_draw(ltrfams->image_area);
      gtk_layout_set_size(GTK_LAYOUT(ltrfams->image_area), 100, 100);
    }
  }
}
/* "support" functions end */

/* thread related functions start */
static gboolean classify_nodes_finished(gpointer data)
{
    ThreadData *threaddata = (ThreadData*) data;

    g_source_remove(GPOINTER_TO_INT(
                                 g_object_get_data(G_OBJECT(threaddata->window),
                                                   "source_id")));
    gtk_widget_destroy(threaddata->window);
    reset_progressbar(threaddata->progressbar);

    if (!threaddata->had_err) {
      g_list_foreach(threaddata->references, (GFunc) remove_candidate, NULL);
      threaddata->ltrfams->unclassified_cands -=
                                           gt_array_size(threaddata->new_nodes);
      gtk_ltr_families_nb_fam_lv_append_array(threaddata->ltrfams,
                                              threaddata->list_view,
                                              threaddata->new_nodes,
                                              NULL);
      gtk_ltr_families_set_modified(threaddata->ltrfams, TRUE);
      update_main_tab_label(threaddata->ltrfams);
    } else {
      g_set_error(&threaddata->ltrfams->gerr,
                  G_FILE_ERROR,
                  0,
                  "Could not classify the selected data: %s",
                  gt_error_get(threaddata->err));
      error_handle(gtk_widget_get_toplevel(GTK_WIDGET(threaddata->ltrfams)),
                   threaddata->ltrfams->gerr);
    }
    threaddata_delete(threaddata);
    return FALSE;
}

static gpointer classify_nodes_start(gpointer data)
{
  ThreadData *threaddata = (ThreadData*) data;
  GtNodeStream *classify_stream = NULL,
               *array_in_stream = NULL,
               *array_out_stream = NULL;

  array_in_stream = gt_array_in_stream_new(threaddata->old_nodes,
                                           NULL,
                                           threaddata->err);
  classify_stream = gt_ltr_classify_stream_new(array_in_stream,
                                               threaddata->sel_features,
                                               threaddata->fam_prefix,
                                               &threaddata->current_state,
                                               &threaddata->progress,
                                               threaddata->err);
  array_out_stream = gt_array_out_stream_new(classify_stream,
                                             threaddata->new_nodes,
                                             threaddata->err);
  threaddata->had_err = gt_node_stream_pull(array_out_stream, threaddata->err);

  gt_node_stream_delete(classify_stream);
  gt_node_stream_delete(array_in_stream);
  gt_node_stream_delete(array_out_stream);
  g_idle_add(classify_nodes_finished, data);

  return NULL;
}
/* thread related functions end */

/* <image_area> related functions start */
static void draw_image(GtkLTRFamilies *ltrfams, GtGenomeNode *gn)
{
  GtkWidget *a = GTK_WIDGET(ltrfams->image_area);
  GtFeatureIndex *features;
  GtRange range;
  const char *seqid;

  features = gt_feature_index_memory_new();
  gt_feature_index_add_feature_node(features, (GtFeatureNode*) gn,
                                    ltrfams->err);
  seqid = gt_feature_index_get_first_seqid(features, ltrfams->err);
  gt_feature_index_get_range_for_seqid(features, &range, seqid, ltrfams->err);
  gt_diagram_delete(ltrfams->diagram);
  ltrfams->diagram = gt_diagram_new(features, seqid, &range,
                                    ltrfams->style, ltrfams->err);
  gtk_widget_queue_draw_area(a, 0, 0, a->allocation.width,
                             a->allocation.height);
  gt_feature_index_delete(features);
}

static gboolean image_area_expose_event(GtkWidget *widget,
                                        GdkEventExpose *event,
                                        GtkLTRFamilies *ltrfams)
{
  cairo_t *cr;
  GtCanvas *canvas = NULL;
  GtLayout *l;
  GtError *err;
  err = gt_error_new();
  unsigned long height;
  int rval;
  if (!ltrfams->diagram || widget->allocation.width <= 30) return FALSE;

  /* render image */
  l = gt_layout_new(ltrfams->diagram, widget->allocation.width, ltrfams->style,
                    err);
  if (!l) return FALSE;
  rval = gt_layout_get_height(l, &height, err);
  gt_assert(rval == 0);
  gtk_layout_set_size(GTK_LAYOUT(widget),
                      widget->allocation.width,
                      height);
  cr = gdk_cairo_create(GTK_LAYOUT(widget)->bin_window);
  cairo_rectangle(cr, event->area.x, event->area.y, event->area.width,
                  event->area.height);
  cairo_clip(cr);
  canvas = gt_canvas_cairo_context_new(ltrfams->style, cr, 0,
                                       widget->allocation.width, height, NULL,
                                       err);
  gt_assert(canvas);
  gt_layout_sketch(l, canvas, err);
  gt_layout_delete(l);
  gt_canvas_delete(canvas);
  cairo_destroy(cr);
  gt_error_delete(err);
  return FALSE;
}
/* <image_area> related functions end */

/* drag'n'drop related functions start */
static void on_drag_data_get(GtkWidget *widget,
                             GT_UNUSED GdkDragContext *drag_context,
                             GtkSelectionData *sdata,
                             GT_UNUSED guint info,
                             GT_UNUSED guint time,
                             GT_UNUSED gpointer user_data)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  GtkTreeRowReference *rowref;
  FamilyTransferData *tdata;
  GtGenomeNode *gn;
  GList *rows, *tmp;

  /* retrieve data from selected row, do nothing if no row is selected */
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(widget));
  if (gtk_tree_selection_count_selected_rows(sel) < 1)
    return;
  tdata = g_slice_new(FamilyTransferData);
  tdata->nodes = gt_array_new(sizeof (GtGenomeNode*));
  tdata->references = NULL;
  tdata->list_view = GTK_TREE_VIEW(widget);

  rows = gtk_tree_selection_get_selected_rows(sel, &model);
  tmp = rows;

  while (tmp != NULL) {
    rowref = gtk_tree_row_reference_new(model, (GtkTreePath*) tmp->data);
    tdata->references = g_list_prepend(tdata->references,
                                       gtk_tree_row_reference_copy(rowref));
    gtk_tree_row_reference_free(rowref);
    gtk_tree_model_get_iter(model, &iter, (GtkTreePath*) tmp->data);
    gtk_tree_model_get(model, &iter,
                       LTRFAMS_LV_NODE, &gn,
                       LTRFAMS_LV_ROWREF, &tdata->rowref,
                       -1);
    gt_array_add(tdata->nodes, gn);
    tmp = tmp->next;
  }
  g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
  g_list_free(rows);

  /* add retrieved data to GtkSelectionData object */
  gtk_selection_data_set(sdata,
                         gdk_atom_intern("FamilyTransferData pointer", FALSE),
                         8,
                         (void*) &tdata,
                         sizeof (tdata));
}

static gboolean on_drag_motion(GtkWidget *widget,
                               GT_UNUSED GdkDragContext *context,
                               gint x,
                               gint y,
                               GT_UNUSED guint time,
                               GT_UNUSED gpointer user_data)
{
  GtkTreeSelection *selection;
  GtkTreePath *path;
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
  gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
  if (gtk_tree_view_get_dest_row_at_pos(GTK_TREE_VIEW(widget), x, y, &path,
                                        NULL)) {
    GtkTreeIter iter;
    GtkTreeModel *model;
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(widget));
    gtk_tree_model_get_iter(model, &iter, path);
    gtk_tree_selection_unselect_all(selection);
    gtk_tree_selection_select_path(selection, path);
    gtk_tree_path_free(path);
  } else {
    gtk_tree_selection_unselect_all(selection);
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_MULTIPLE);
    return FALSE;
  }
  gtk_tree_selection_set_mode(selection, GTK_SELECTION_MULTIPLE);
  return TRUE;
}

static void on_drag_data_received(GtkWidget *widget,
                                  GT_UNUSED GdkDragContext *drag_context,
                                  GT_UNUSED gint x,
                                  GT_UNUSED gint y,
                                  GtkSelectionData *sdata,
                                  GT_UNUSED guint info,
                                  GT_UNUSED guint time,
                                  GtkLTRFamilies *ltrfams)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreeSelection *sel;
  GtkTreePath *path;
  GtkWidget *tab_child;
  FamilyTransferData *tdata = NULL;
  GtFeatureNode *curnode;
  GtFeatureNodeIterator *fni;
  GtGenomeNode *gn;
  GtArray *nodes;
  gchar *oldname,
        curname[BUFSIZ];
  unsigned long i;

  /* add <gn> to selected family, do nothing if no family is selected
     <widget> == <lv_families> */
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
  gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);
  if (!gtk_tree_selection_get_selected(sel, &model, &iter)) {
    gtk_tree_selection_set_mode(sel, GTK_SELECTION_MULTIPLE);
    return;
  }
  /* copy recieved data into new struct */
  memcpy(&tdata, sdata->data, sizeof (tdata));

  path = gtk_tree_model_get_path(model, &iter);
  if (tdata->rowref &&
      (gtk_tree_path_compare(gtk_tree_row_reference_get_path(tdata->rowref),
                            path)) == 0) {
    free_tdata(tdata);
    gtk_tree_path_free(path);
    return;
  }
  gtk_tree_path_free(path);

  gtk_tree_model_get(model, &iter,
                     LTRFAMS_FAM_LV_NODE_ARRAY, &nodes,
                     LTRFAMS_FAM_LV_TAB_CHILD, &tab_child,
                     LTRFAMS_FAM_LV_OLDNAME, &oldname,
                     -1);

  /* tdata->rowref points to family in <lv_families>. If <nodes> was
     drag'n'dropped from one family to another, remove <gn> from node
     array of old family */
  if (tdata->rowref) {
    GtkTreePath *tv_path;
    GtkTreeIter tv_iter;
    GtArray *tmp_nodes;
    char *tmp_oldname,
         tmp_curname[BUFSIZ];

    tv_path = gtk_tree_row_reference_get_path(tdata->rowref);
    gtk_tree_model_get_iter(model, &tv_iter, tv_path);
    gtk_tree_model_get(model, &tv_iter,
                       LTRFAMS_FAM_LV_NODE_ARRAY, &tmp_nodes,
                       LTRFAMS_FAM_LV_OLDNAME, &tmp_oldname,
                       -1);
    remove_nodes_from_array(tmp_nodes, tdata->nodes, FALSE);
    g_snprintf(tmp_curname, BUFSIZ, "%s (%lu)",
               tmp_oldname, gt_array_size(tmp_nodes));
    gtk_list_store_set(GTK_LIST_STORE(model), &tv_iter,
                       LTRFAMS_FAM_LV_CURNAME, tmp_curname,
                       -1);
    gtk_tree_path_free(tv_path);
    g_free(tmp_oldname);
  } else {
    ltrfams->unclassified_cands -= gt_array_size(tdata->nodes);
    update_main_tab_label(ltrfams);
  }

  /* remove rows from drag source */
  g_list_foreach(tdata->references, (GFunc) remove_candidate, NULL);

  for (i = 0; i < gt_array_size(tdata->nodes); i++) {
    CandidateData *cdata;
    GtkTreePath *path;
    GtkTreeRowReference  *tv_ref2;
    const char *attr;

    gn = *(GtGenomeNode**) gt_array_get(tdata->nodes, i);
    gtk_ltr_families_clear_lv_det_on_equal_nodes(ltrfams, gn);
    fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);
    curnode = gt_feature_node_iterator_next(fni);
    gt_feature_node_set_attribute(curnode, ATTR_LTRFAM, oldname);
    attr = gt_feature_node_get_attribute(curnode, ATTR_FULLLEN);
    if (attr)
      gt_feature_node_remove_attribute(curnode, ATTR_FULLLEN);
    gt_feature_node_iterator_delete(fni);
    path = gtk_tree_model_get_path(model, &iter);
    tv_ref2 = gtk_tree_row_reference_new(model, path);
    cdata = (CandidateData*) gt_genome_node_get_user_data(gn, "cdata");
    cdata->fam_ref = tv_ref2;
    cdata->cand_ref = NULL;
    gt_array_add(nodes, gn);
    if (tab_child)
      gtk_ltr_families_nb_fam_lv_append_gn(ltrfams, GTK_TREE_VIEW(tab_child),
                                           gn, tv_ref2, NULL, NULL, NULL);
    gtk_tree_path_free(path);
  }

  g_snprintf(curname, BUFSIZ, "%s (%lu)", oldname, gt_array_size(nodes));
  gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                     LTRFAMS_FAM_LV_CURNAME, curname,
                     -1);
  gtk_tree_selection_set_mode(sel, GTK_SELECTION_MULTIPLE);
  g_free(oldname);
  free_tdata(tdata);
  gtk_ltr_families_set_modified(ltrfams, TRUE);
}
/* drag'n'drop related functions end */

/* popupmenu related functions start*/
static void
gtk_ltr_families_notebook_list_view_pmenu_filter_clicked(
                                                  GT_UNUSED GtkWidget *menuitem,
                                                        GtkLTRFamilies *ltrfams)
{
  gtk_ltr_filter_set_range(GTK_LTR_FILTER(ltrfams->ltrfilt),
                           LTR_FILTER_RANGE_CANDIDATES);
  gtk_widget_show(ltrfams->ltrfilt);
}

static void
gtk_ltr_families_nb_fam_lv_pmenu_remove_clicked(GT_UNUSED GtkWidget *menuitem,
                                                GtkLTRFamilies *ltrfams)
{
  GtkWidget *dialog,
            *main_tab,
            *tab_child;
  GtkTreeView *list_view,
              *tmp_view;
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  GtkTreeIter iter;
  GtkTreeRowReference *rowref, *tmp_rowref;
  GtArray *nodes;
  GList *children, *rows, *tmp, *tmp_children, *references = NULL;
  GtGenomeNode *gn;
  gchar tmp_curname[BUFSIZ];
  gint main_tab_no,
       tab_no;

  tab_no = gtk_notebook_get_current_page(GTK_NOTEBOOK(ltrfams->nb_family));
  tab_child = gtk_notebook_get_nth_page(GTK_NOTEBOOK(ltrfams->nb_family),
                                        tab_no);
  main_tab_no = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(ltrfams->nb_family),
                                                  "main_tab"));
  children = gtk_container_get_children(GTK_CONTAINER(tab_child));
  list_view = GTK_TREE_VIEW(g_list_first(children)->data);
  model = gtk_tree_view_get_model(list_view);
  sel = gtk_tree_view_get_selection(list_view);
  if (gtk_tree_selection_count_selected_rows(sel) == 0)
    return;

  dialog = gtk_message_dialog_new(NULL,
                                  GTK_DIALOG_MODAL |
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
                                  "%s",
                                  (tab_no == main_tab_no) ? CAND_RM_DIALOG :
                                                            CAND_UC_DIALOG);
  gtk_window_set_title(GTK_WINDOW(dialog), "Attention!");
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
  if (gtk_dialog_run(GTK_DIALOG(dialog)) != GTK_RESPONSE_YES) {
    gtk_widget_destroy(dialog);
    return;
  } else {
    nodes = gt_array_new(sizeof(GtGenomeNode*));
    main_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(ltrfams->nb_family),
                                         main_tab_no);
    tmp_children = gtk_container_get_children(GTK_CONTAINER(main_tab));
    tmp_view = GTK_TREE_VIEW(g_list_first(tmp_children)->data);
    rows = gtk_tree_selection_get_selected_rows(sel, &model);
    tmp = rows;
    while (tmp != NULL) {
      rowref = gtk_tree_row_reference_new(model, (GtkTreePath*) tmp->data);
      gtk_tree_model_get_iter(model, &iter, (GtkTreePath*) tmp->data);
      gtk_tree_model_get(model, &iter,
                         LTRFAMS_LV_NODE, &gn,
                         LTRFAMS_LV_ROWREF, &tmp_rowref,
                         -1);
      gt_array_add(nodes, gn);
      gtk_ltr_families_clear_lv_det_on_equal_nodes(ltrfams, gn);
      if (tab_no != main_tab_no) {
        GtFeatureNode *curnode;
        GtFeatureNodeIterator *fni;
        const char *attr;

        fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);
        curnode = gt_feature_node_iterator_next(fni);
        gt_feature_node_remove_attribute(curnode, ATTR_LTRFAM);
        if ((attr = gt_feature_node_get_attribute(curnode, ATTR_FULLLEN)))
          gt_feature_node_remove_attribute(curnode, ATTR_FULLLEN);
        gtk_ltr_families_nb_fam_lv_append_gn(ltrfams, tmp_view, gn, NULL, NULL,
                                             NULL, NULL);
        gt_feature_node_iterator_delete(fni);
      }
      references = g_list_prepend(references,
                                  gtk_tree_row_reference_copy(rowref));
      gtk_tree_row_reference_free(rowref);
      tmp = tmp->next;
    }
    if (tmp_rowref) {
      GtkTreeModel *model2;
      GtkTreePath *tv_path;
      GtkTreeIter tv_iter;
      GtArray *tmp_nodes;
      gchar *tmp_oldname;
      model2 = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfams->lv_families));
      tv_path = gtk_tree_row_reference_get_path(tmp_rowref);
      gtk_tree_model_get_iter(model2, &tv_iter, tv_path);
      gtk_tree_model_get(model2, &tv_iter,
                         LTRFAMS_FAM_LV_NODE_ARRAY, &tmp_nodes,
                         LTRFAMS_FAM_LV_OLDNAME, &tmp_oldname,
                         -1);
      remove_nodes_from_array(tmp_nodes, nodes, FALSE);
      ltrfams->unclassified_cands += gt_array_size(nodes);
      update_main_tab_label(ltrfams);
      g_snprintf(tmp_curname, BUFSIZ, "%s (%lu)",
                 tmp_oldname, gt_array_size(tmp_nodes));
      gtk_list_store_set(GTK_LIST_STORE(model2), &tv_iter,
                         LTRFAMS_FAM_LV_CURNAME, tmp_curname,
                         -1);
      gtk_tree_path_free(tv_path);
      g_free(tmp_oldname);
    } else {
      remove_nodes_from_array(ltrfams->nodes, nodes, TRUE);
      gtk_ltr_families_update_unclass_cands(ltrfams,
                                            (-1) * gt_array_size(nodes));
    }
    g_list_foreach(references, (GFunc) remove_row, NULL);
    g_list_foreach(references, (GFunc) gtk_tree_row_reference_free, NULL);
    g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
    g_list_free(references);
    g_list_free(rows);
    g_list_free(tmp_children);
    gt_array_delete(nodes);
    gtk_ltr_families_set_modified(ltrfams, TRUE);
  }
  g_list_free(children);
  gtk_widget_destroy(dialog);
}

static void gtk_ltr_families_nb_fam_lv_pmenu(GT_UNUSED GtkWidget *tree_view,
                                             GdkEventButton *event,
                                             GtkLTRFamilies *ltrfams)
{
  GtkWidget *menu,
            *menuitem;
  gint cur_tab_no, main_tab_no;

  cur_tab_no = gtk_notebook_get_current_page(GTK_NOTEBOOK(ltrfams->nb_family));
  main_tab_no = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(ltrfams->nb_family),
                                                  "main_tab"));

  menu = gtk_menu_new();

  menuitem = gtk_menu_item_new_with_label((cur_tab_no == main_tab_no)
                                          ? DELETE_SELECTED : UNCLASS_SELECTED);
  g_signal_connect(menuitem, "activate",
                   G_CALLBACK(gtk_ltr_families_nb_fam_lv_pmenu_remove_clicked),
                   ltrfams);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  menuitem = gtk_menu_item_new_with_label(LTR_FAMILIES_FILTER_SELECTION);
  g_signal_connect(menuitem, "activate",
           G_CALLBACK(gtk_ltr_families_notebook_list_view_pmenu_filter_clicked),
                   ltrfams);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  gtk_widget_show_all(menu);
  gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,
                 (event != NULL) ? event->button : 0,
                 gdk_event_get_time((GdkEvent*)event));
}

static gboolean gtk_ltr_families_nb_fam_lv_button_pressed(GtkWidget *tree_view,
                                                          GdkEventButton *event,
                                                        GtkLTRFamilies *ltrfams)
{
  if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3)
  {
    GtkTreeSelection *selection;
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
    if (gtk_tree_selection_count_selected_rows(selection)  <= 1)
    {
      GtkTreePath *path;
      if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(tree_view),
                                        (gint) event->x,
                                        (gint) event->y,
                                        &path, NULL, NULL, NULL)) {
        gtk_tree_selection_unselect_all(selection);
        gtk_tree_selection_select_path(selection, path);
        gtk_tree_path_free(path);
      } else
        return TRUE;
    }
    gtk_ltr_families_nb_fam_lv_pmenu(tree_view, event, ltrfams);
    return TRUE;
  }
  return FALSE;
}

static gboolean gtk_ltr_families_nb_fam_lv_on_pmenu(GtkWidget *tree_view,
                                                    GtkLTRFamilies *ltrfams)
{
  gtk_ltr_families_nb_fam_lv_pmenu(tree_view, NULL, ltrfams);
  return TRUE;
}

static void
gtk_ltr_families_lv_fams_pmenu_edit_clicked(GT_UNUSED GtkWidget *menuitem,
                                            gpointer userdata)
{
  GtkTreeView *tree_view;
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  GtkTreeViewColumn *column;
  GtkTreeIter iter;
  GtkTreePath *path;
  GtkCellRenderer *renderer;
  GList *tmp;

  tree_view = GTK_TREE_VIEW(userdata);
  model = gtk_tree_view_get_model(tree_view);
  sel = gtk_tree_view_get_selection(tree_view);
  gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);
  column = gtk_tree_view_get_column(tree_view, 0);
  tmp = gtk_cell_layout_get_cells(GTK_CELL_LAYOUT(column));
  renderer = (GtkCellRenderer*) g_list_nth_data(tmp, 0);
  g_object_set(renderer, "editable", TRUE, NULL);
  gtk_tree_selection_get_selected(sel, &model, &iter);
  path = gtk_tree_model_get_path(model, &iter);
  gtk_tree_view_set_cursor(tree_view, path, column, TRUE);
  gtk_tree_path_free(path);
}

static gboolean match_cands_finished(gpointer data)
{
  ThreadData *threaddata = (ThreadData*) data;

  g_source_remove(GPOINTER_TO_INT(
                               g_object_get_data(G_OBJECT(threaddata->window),
                                                 "source_id")));
  gtk_widget_destroy(threaddata->window);
  reset_progressbar(threaddata->progressbar);

  if (threaddata->had_err) {
    g_set_error(&threaddata->ltrfams->gerr,
                G_FILE_ERROR,
                0,
                "Could not classify the selected data: %s",
                gt_error_get(threaddata->err));
    error_handle(gtk_widget_get_toplevel(GTK_WIDGET(threaddata->ltrfams)),
                 threaddata->ltrfams->gerr);
  }
  gt_array_delete(threaddata->nodes);
  threaddata_delete(threaddata);
  return FALSE;
}

static gpointer match_cands_start(gpointer data)
{
  ThreadData *threaddata = (ThreadData*) data;
  GtkWidget *blastn_params = threaddata->blastn_refseq;
  GtNodeStream *array_in_stream = NULL,
               *refseq_match_stream = NULL;
  gdouble evalue, xdrop, seqid, match_len;
  gboolean dust, flcands;
  gint gapopen, gapextend, penalty, reward, threads, wordsize;
  const gchar *moreblast, *refseq_file;

  evalue = gtk_blastn_params_get_evalue(GTK_BLASTN_PARAMS(blastn_params));
  dust = gtk_blastn_params_get_dust(GTK_BLASTN_PARAMS(blastn_params));
  gapopen = gtk_blastn_params_get_gapopen(GTK_BLASTN_PARAMS(blastn_params));
  gapextend = gtk_blastn_params_get_gapextend(GTK_BLASTN_PARAMS(blastn_params));
  xdrop = gtk_blastn_params_get_xdrop(GTK_BLASTN_PARAMS(blastn_params));
  penalty = gtk_blastn_params_get_penalty(GTK_BLASTN_PARAMS(blastn_params));
  reward = gtk_blastn_params_get_reward(GTK_BLASTN_PARAMS(blastn_params));
  threads = gtk_blastn_params_get_threads(GTK_BLASTN_PARAMS(blastn_params));
  wordsize = gtk_blastn_params_get_wordsize(GTK_BLASTN_PARAMS(blastn_params));
  seqid = gtk_blastn_params_get_seqid(GTK_BLASTN_PARAMS(blastn_params));
  moreblast = gtk_blastn_params_get_moreblast(GTK_BLASTN_PARAMS(blastn_params));
  refseq_file = gtk_blastn_params_refseq_get_refseq_file(
                                       GTK_BLASTN_PARAMS_REFSEQ(blastn_params));
  match_len = gtk_blastn_params_refseq_get_match_len(
                                       GTK_BLASTN_PARAMS_REFSEQ(blastn_params));
  flcands = gtk_blastn_params_refseq_get_flcands(
                                       GTK_BLASTN_PARAMS_REFSEQ(blastn_params));
  gtk_widget_destroy(threaddata->dialog);

  /* TODO: get indexname & tmpdir */
  array_in_stream = gt_array_in_stream_new(threaddata->nodes,
                                           NULL, threaddata->err);
  refseq_match_stream = ltrgui_refseq_match_stream_new(array_in_stream,
                                                       NULL,
                                                       refseq_file,
                                                       NULL,
                                                       evalue, dust, wordsize,
                                                       gapopen, gapextend,
                                                       penalty, reward, threads,
                                                       xdrop, seqid, moreblast,
                                                       flcands, match_len,
                                                       threaddata->err);
  threaddata->had_err = gt_node_stream_pull(refseq_match_stream,
                                            threaddata->err);
  gt_node_stream_delete(refseq_match_stream);
  gt_node_stream_delete(array_in_stream);
  g_idle_add(match_cands_finished, data);
  return NULL;
}

static void
gtk_ltr_families_list_view_families_pmenu_match_clicked(GT_UNUSED GtkWidget *m,
                                                        GtkLTRFamilies *ltrfams)
{
  ThreadData *threaddata;
  GtkWidget *dialog,
            *blastn_params;
  GtkTreeSelection *sel;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtArray *nodes, *tmpnodes;
  GList *rows, *tmp;

  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(ltrfams->lv_families));
  if (gtk_tree_selection_count_selected_rows(sel) == 0)
    return;

  blastn_params = gtk_blastn_params_refseq_new();
  dialog = gtk_dialog_new_with_buttons(MATCH_DIALOG,
                       GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(ltrfams))),
                                       GTK_DIALOG_MODAL |
                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                       GTK_STOCK_APPLY,
                                       GTK_RESPONSE_ACCEPT,
                                       GTK_STOCK_CANCEL,
                                       GTK_RESPONSE_REJECT,
                                       NULL);

  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), blastn_params);
  gtk_widget_show_all(dialog);
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_REJECT) {
    gtk_widget_destroy(dialog);
    return;
  }
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfams->lv_families));
  nodes = gt_array_new(sizeof (GtGenomeNode*));
  rows = gtk_tree_selection_get_selected_rows(sel, &model);
  tmp = rows;
  while (tmp != NULL) {
    gtk_tree_model_get_iter(model, &iter, (GtkTreePath*) tmp->data);
    gtk_tree_model_get(model, &iter,
                       LTRFAMS_FAM_LV_NODE_ARRAY, &tmpnodes,
                       -1);
    gt_array_add_array(nodes, tmpnodes);
    tmp = tmp->next;
  }
  gt_genome_nodes_sort_stable(nodes);

  /* create thread for matching */
  threaddata = threaddata_new();
  threaddata->ltrfams = ltrfams;
  threaddata->progressbar = ltrfams->progressbar;
  threaddata->dialog = dialog;
  threaddata->blastn_refseq = blastn_params;
  threaddata->nodes = nodes;
  threaddata->current_state = gt_cstr_dup("Blaaaa");

  progress_dialog_init(threaddata,
                       gtk_widget_get_toplevel(GTK_WIDGET(ltrfams)));

  if (!g_thread_create(match_cands_start, (gpointer) threaddata, FALSE,
                       &ltrfams->gerr)) {
    error_handle(gtk_widget_get_toplevel(GTK_WIDGET(ltrfams)), ltrfams->gerr);
  }
  g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
  g_list_free(rows);
}

static void
gtk_ltr_families_list_view_families_pmenu_filter_clicked(GT_UNUSED GtkWidget *m,
                                                        GtkLTRFamilies *ltrfams)
{
  gtk_ltr_filter_set_range(GTK_LTR_FILTER(ltrfams->ltrfilt),
                           LTR_FILTER_RANGE_FAMILIES);
  gtk_widget_show(ltrfams->ltrfilt);
}

static void
gtk_ltr_families_list_view_families_pmenu_merge_clicked(
                                                  GT_UNUSED GtkWidget *menuitem,
                                                        GtkLTRFamilies *ltrfams)
{
  CandidateData *cdata;
  GtkWidget *dialog, *entry, *label;
  GList *rows, *tmp, *references = NULL;
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  GtkTreePath *path;
  GtkTreeRowReference *rowref;
  GtArray *nodes, *tmp_nodes;
  gchar cur_name[BUFSIZ];
  GtFeatureNode *curnode;
  GtFeatureNodeIterator *fni;
  unsigned long i;
  GtGenomeNode *gn;
  const gchar *attr;

  dialog = gtk_dialog_new_with_buttons(MERGE_FAMILIES_DIALOG,
                       GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(ltrfams))),
                                       GTK_DIALOG_MODAL |
                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                       GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                       GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
  label = gtk_label_new(ENTER_NEW_FAMILIE_NAME);
  entry = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(entry), 25);
  gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), label);
  gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), entry);
  gtk_widget_show_all(dialog);
  if (gtk_dialog_run(GTK_DIALOG(dialog)) != GTK_RESPONSE_OK) {
    gtk_widget_destroy(dialog);
    return;
  }
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfams->lv_families));
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(ltrfams->lv_families));
  nodes = gt_array_new(sizeof (GtGenomeNode*));
  rows = gtk_tree_selection_get_selected_rows(sel, &model);
  tmp = rows;
  while (tmp != NULL) {
    gtk_tree_model_get_iter(model, &iter, (GtkTreePath*) tmp->data);
    rowref = gtk_tree_row_reference_new(model, (GtkTreePath*) tmp->data);
    references = g_list_prepend(references,
                                gtk_tree_row_reference_copy(rowref));
    gtk_tree_row_reference_free(rowref);
    gtk_tree_model_get(model, &iter, LTRFAMS_FAM_LV_NODE_ARRAY, &tmp_nodes, -1);
    gt_array_add_array(nodes, tmp_nodes);
    tmp = tmp->next;
  }
  gtk_list_store_append(GTK_LIST_STORE(model), &iter);
  path = gtk_tree_model_get_path(model, &iter);
  rowref = gtk_tree_row_reference_new(model, path);
  gtk_tree_path_free(path);
  for (i = 0; i < gt_array_size(nodes); i++) {
    gn = *(GtGenomeNode**) gt_array_get(nodes, i);
    cdata = (CandidateData*) gt_genome_node_get_user_data(gn, "cdata");
    cdata->fam_ref = rowref;
    cdata->cand_ref = NULL;
    fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);
    curnode = gt_feature_node_iterator_next(fni);
    gt_feature_node_set_attribute(curnode, ATTR_LTRFAM,
                                  gtk_entry_get_text(GTK_ENTRY(entry)));
    attr = gt_feature_node_get_attribute(curnode, ATTR_FULLLEN);
    if (attr)
      gt_feature_node_remove_attribute(curnode, ATTR_FULLLEN);
    gt_feature_node_iterator_delete(fni);

  }
  g_snprintf(cur_name, BUFSIZ, "%s (%lu)", gtk_entry_get_text(GTK_ENTRY(entry)),
             gt_array_size(nodes));
  gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                     LTRFAMS_FAM_LV_NODE_ARRAY, nodes,
                     LTRFAMS_FAM_LV_TAB_CHILD, NULL,
                     LTRFAMS_FAM_LV_TAB_LABEL, NULL,
                     LTRFAMS_FAM_LV_CURNAME, cur_name,
                     LTRFAMS_FAM_LV_OLDNAME,
                     gtk_entry_get_text(GTK_ENTRY(entry)),
                     LTRFAMS_FAM_LV_ROWREF, rowref,
                     -1);
  gtk_widget_destroy(dialog);
  g_list_foreach(references, (GFunc) remove_merged_family, ltrfams);
  g_list_foreach(references, (GFunc) gtk_tree_row_reference_free, NULL);
  g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
  g_list_free(references);
  g_list_free(rows);
  gtk_ltr_families_set_modified(ltrfams, TRUE);
}

static void
gtk_ltr_families_lv_fams_pmenu_remove_clicked(GT_UNUSED GtkWidget *menuitem,
                                              GtkLTRFamilies *ltrfams)
{
  GtkWidget *dialog;
  GtkTreeView *tree_view;
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  GtkTreeRowReference *rowref;
  gchar text[BUFSIZ];
  GList *rows, *tmp, *references = NULL;

  tree_view = GTK_TREE_VIEW(ltrfams->lv_families);
  model = gtk_tree_view_get_model(tree_view);
  sel = gtk_tree_view_get_selection(tree_view);

  g_snprintf(text, BUFSIZ, FAMS_RM_DIALOG,
             gtk_tree_selection_count_selected_rows(sel));
  dialog = gtk_message_dialog_new(NULL,
                                  GTK_DIALOG_MODAL |
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
                                  "%s",
                                  text);
  gtk_window_set_title(GTK_WINDOW(dialog), ATTENTION);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
  if (gtk_dialog_run(GTK_DIALOG(dialog)) != GTK_RESPONSE_YES) {
    gtk_widget_destroy(dialog);
    return;
  } else {
    rows = gtk_tree_selection_get_selected_rows(sel, &model);
    tmp = rows;
    while (tmp != NULL) {
      rowref = gtk_tree_row_reference_new(model, (GtkTreePath*) tmp->data);
      references = g_list_prepend(references,
                                  gtk_tree_row_reference_copy(rowref));
      gtk_tree_row_reference_free(rowref);
      tmp = tmp->next;
    }
    g_list_foreach(references, (GFunc) remove_family, ltrfams);
    g_list_foreach(references, (GFunc) gtk_tree_row_reference_free, NULL);
    g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
    g_list_free(references);
    g_list_free(rows);
    gtk_ltr_families_set_modified(ltrfams, TRUE);
    update_main_tab_label(ltrfams);
  }
  gtk_widget_destroy(dialog);
}

static void
gtk_ltr_families_lv_fams_pmenu_export_anno(GtkLTRFamilies *ltrfams,
                                           gboolean multi)
{
  GtkWidget *filechooser = NULL,
            *toplevel,
            *checkb,
            *align;
  GtkTreeView *tree_view;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreeSelection *sel;
  GtArray *nodes,
          *tmpnodes;
  GList *rows,
        *tmp;
  gboolean flcands;
  gchar *filename;
  const gchar *projectfile;

  toplevel = gtk_widget_get_toplevel(GTK_WIDGET(ltrfams));
  projectfile = gtk_ltr_families_get_projectfile(ltrfams);
  tree_view = GTK_TREE_VIEW(ltrfams->lv_families);
  model = gtk_tree_view_get_model(tree_view);
  sel = gtk_tree_view_get_selection(tree_view);

  filechooser =
     gtk_file_chooser_dialog_new(multi ? CHOOSE_PREFIX_ANNO : CHOOSE_FILEN_ANNO,
                                 GTK_WINDOW(toplevel),
                                 GTK_FILE_CHOOSER_ACTION_SAVE,
                                 GTK_STOCK_CANCEL,
                                 GTK_RESPONSE_CANCEL, "Export",
                                 GTK_RESPONSE_ACCEPT, NULL);
    checkb = gtk_check_button_new_with_mnemonic(FAMS_EXPORT_FLCANDS);
    align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
    gtk_container_add(GTK_CONTAINER(align), checkb);
    gtk_widget_show_all(align);
    gtk_file_chooser_set_extra_widget(GTK_FILE_CHOOSER(filechooser), align);
  if (projectfile != NULL)
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(filechooser),
                                        g_path_get_dirname(projectfile));
  else
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(filechooser),
                                        g_get_home_dir());
  if (gtk_dialog_run(GTK_DIALOG(filechooser)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser));
    flcands = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkb));
    gtk_widget_destroy(filechooser);
  } else {
    gtk_widget_destroy(filechooser);
    return;
  }

  if (!multi) {
    nodes = gt_array_new(sizeof (GtGenomeNode*));
    rows = gtk_tree_selection_get_selected_rows(sel, &model);
    tmp = rows;
    while (tmp != NULL) {
      gtk_tree_model_get_iter(model, &iter, (GtkTreePath*) tmp->data);
      gtk_tree_model_get(model, &iter,
                         LTRFAMS_FAM_LV_NODE_ARRAY, &tmpnodes,
                         -1);
      gt_array_add_array(nodes, tmpnodes);
      tmp = tmp->next;
    }
    g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
    gt_genome_nodes_sort_stable(nodes);
    export_annotation(nodes, filename, flcands,
                      gtk_widget_get_toplevel(GTK_WIDGET(ltrfams)),
                      ltrfams->gerr);
    gt_array_delete(nodes);
    g_free(filename);
  } else {
    rows = gtk_tree_selection_get_selected_rows(sel, &model);
    tmp = rows;
    while (tmp != NULL) {
      gchar *famname,
            *filen;

      gtk_tree_model_get_iter(model, &iter, (GtkTreePath*) tmp->data);
      gtk_tree_model_get(model, &iter,
                         LTRFAMS_FAM_LV_OLDNAME, &famname,
                         LTRFAMS_FAM_LV_NODE_ARRAY, &nodes,
                         -1);
      gt_genome_nodes_sort_stable(nodes);
      filen = g_strdup_printf("%s_%s", filename, famname);
      export_annotation(nodes, filename, flcands,
                        gtk_widget_get_toplevel(GTK_WIDGET(ltrfams)),
                        ltrfams->gerr);
      g_free(famname);
      g_free(filen);
      tmp = tmp->next;
    }
    g_free(filename);
    g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
  }
}

static void
gtk_ltr_families_lv_fams_pmenu_export_seqs(GtkLTRFamilies *ltrfams,
                                           gboolean multi)
{
  GtkWidget *dialog = NULL,
            *filechooser,
            *toplevel,
            *projset,
            *checkb,
            *align;
  GtkTreeView *tree_view;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreeSelection *sel;
  GtArray *nodes,
          *tmpnodes;
  GList *rows,
        *tmp;
  gboolean flcands;
  gchar *filename,
        tmp_index[BUFSIZ];
  const gchar *projectfile,
              *indexname;

  toplevel = gtk_widget_get_toplevel(GTK_WIDGET(ltrfams));
  projectfile = gtk_ltr_families_get_projectfile(ltrfams);
  tree_view = GTK_TREE_VIEW(ltrfams->lv_families);
  model = gtk_tree_view_get_model(tree_view);
  sel = gtk_tree_view_get_selection(tree_view);

  projset = ltrfams->projset;
  indexname =
          gtk_project_settings_get_indexname(GTK_PROJECT_SETTINGS(projset));
  g_snprintf(tmp_index, BUFSIZ, "%s%s", indexname, ESQ_PATTERN);
  if ((g_strcmp0(indexname, "") == 0) ||
      !g_file_test(tmp_index, G_FILE_TEST_EXISTS)) {
    dialog = gtk_message_dialog_new(GTK_WINDOW(toplevel),
                                    GTK_DIALOG_MODAL ||
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_QUESTION,
                                    GTK_BUTTONS_YES_NO,
                                    "%s",
                                    NO_INDEX_DIALOG);
    gtk_window_set_title(GTK_WINDOW(dialog), INFORMATION);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) != GTK_RESPONSE_YES) {
      gtk_widget_destroy(dialog);
      return;
    } else {
      gtk_widget_destroy(dialog);

      filechooser = gtk_file_chooser_dialog_new(SELECT_INDEX,
                                                GTK_WINDOW(toplevel),
                                                GTK_FILE_CHOOSER_ACTION_SAVE,
                                                GTK_STOCK_CANCEL,
                                                GTK_RESPONSE_CANCEL, "Sele_ct",
                                                GTK_RESPONSE_ACCEPT, NULL);
      if (projectfile != NULL)
        gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(filechooser),
                                           g_path_get_dirname(projectfile));
      else
        gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(filechooser),
                                            g_get_home_dir());
      GtkFileFilter *esq_file_filter;
      esq_file_filter = gtk_file_filter_new();
      gtk_file_filter_set_name(esq_file_filter, ESQ_FILTER_PATTERN);
      gtk_file_filter_add_pattern(esq_file_filter, ESQ_FILTER_PATTERN);
      gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filechooser),
                                  esq_file_filter);
      if (gtk_dialog_run(GTK_DIALOG(filechooser)) == GTK_RESPONSE_ACCEPT) {
        gchar *tmpname, *tmp;
        tmpname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser));
        tmp = g_strndup(tmpname, strlen(tmpname) - 4);

        gtk_widget_destroy(filechooser);
        gtk_project_settings_update_indexname(GTK_PROJECT_SETTINGS(projset),
                                              tmp);
        gtk_ltr_families_set_modified(ltrfams, TRUE);
        g_free(tmp);
        g_free(tmpname);
        indexname =
          gtk_project_settings_get_indexname(GTK_PROJECT_SETTINGS(projset));
      } else {
        gtk_widget_destroy(filechooser);
        return;
      }
    }
  }

  filechooser =
     gtk_file_chooser_dialog_new(multi ? CHOOSE_PREFIX_SEQS : CHOOSE_FILEN_SEQS,
                                 GTK_WINDOW(toplevel),
                                 GTK_FILE_CHOOSER_ACTION_SAVE,
                                 GTK_STOCK_CANCEL,
                                 GTK_RESPONSE_CANCEL, "Export",
                                 GTK_RESPONSE_ACCEPT, NULL);
  checkb = gtk_check_button_new_with_mnemonic(FAMS_EXPORT_FLCANDS);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), checkb);
  gtk_widget_show_all(align);
  gtk_file_chooser_set_extra_widget(GTK_FILE_CHOOSER(filechooser), align);

  if (projectfile != NULL)
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(filechooser),
                                        g_path_get_dirname(projectfile));
  else
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(filechooser),
                                        g_get_home_dir());
  if (gtk_dialog_run(GTK_DIALOG(filechooser)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser));
    flcands = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkb));
    gtk_widget_destroy(filechooser);
  } else {
    gtk_widget_destroy(filechooser);
    return;
  }

  if (!multi) {
    nodes = gt_array_new(sizeof(GtGenomeNode*));
    rows = gtk_tree_selection_get_selected_rows(sel, &model);
    tmp = rows;
    while (tmp != NULL) {
      gtk_tree_model_get_iter(model, &iter, (GtkTreePath*) tmp->data);
      gtk_tree_model_get(model, &iter,
                         LTRFAMS_FAM_LV_NODE_ARRAY, &tmpnodes,
                         -1);
      gt_array_add_array(nodes, tmpnodes);
      tmp = tmp->next;
    }
    g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
    gt_genome_nodes_sort_stable(nodes);
    export_sequences(nodes, filename, indexname, flcands,
                     gtk_widget_get_toplevel(GTK_WIDGET(ltrfams)),
                     ltrfams->gerr);
    gt_array_delete(nodes);
    g_free(filename);
  } else {
    rows = gtk_tree_selection_get_selected_rows(sel, &model);
    tmp = rows;
    while (tmp != NULL) {
      gchar *famname,
            *filen;

      gtk_tree_model_get_iter(model, &iter, (GtkTreePath*) tmp->data);
      gtk_tree_model_get(model, &iter,
                         LTRFAMS_FAM_LV_OLDNAME, &famname,
                         LTRFAMS_FAM_LV_NODE_ARRAY, &nodes,
                         -1);
      gt_genome_nodes_sort_stable(nodes);
      filen = g_strdup_printf("%s_%s", filename, famname);
      export_sequences(nodes, filen, indexname, flcands,
                       gtk_widget_get_toplevel(GTK_WIDGET(ltrfams)),
                       ltrfams->gerr);
      g_free(famname);
      g_free(filen);
      tmp = tmp->next;
    }
    g_free(filename);
    g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
  }
}

static void
gtk_ltr_families_lv_fams_pmenu_export_seqs_one(GT_UNUSED GtkWidget *menuitem,
                                               GtkLTRFamilies *ltrfams)
{
  gtk_ltr_families_lv_fams_pmenu_export_seqs(ltrfams, FALSE);
}

static void
gtk_ltr_families_lv_fams_pmenu_export_seqs_mult(GT_UNUSED GtkWidget *menuitem,
                                                GtkLTRFamilies *ltrfams)
{
  gtk_ltr_families_lv_fams_pmenu_export_seqs(ltrfams, TRUE);
}

static void
gtk_ltr_families_lv_fams_pmenu_export_anno_one(GT_UNUSED GtkWidget *menuitem,
                                               GtkLTRFamilies *ltrfams)
{
  gtk_ltr_families_lv_fams_pmenu_export_anno(ltrfams, FALSE);
}

static void
gtk_ltr_families_lv_fams_pmenu_export_anno_mult(GT_UNUSED GtkWidget *menuitem,
                                                GtkLTRFamilies *ltrfams)
{
  gtk_ltr_families_lv_fams_pmenu_export_anno(ltrfams, TRUE);
}

static void gtk_ltr_families_tv_fams_pmenu(GtkWidget *tree_view,
                                           GdkEventButton *event,
                                           GtkLTRFamilies *ltrfams)
{
  GtkWidget *menu,
            *menuitem;
  GtkTreeSelection *sel;

  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
  if (gtk_tree_selection_count_selected_rows(sel) == 0)
    return;

  menu = gtk_menu_new();

  menuitem = gtk_menu_item_new_with_label(FAMS_EDIT_NAME);
  g_signal_connect(menuitem, "activate",
                   G_CALLBACK(gtk_ltr_families_lv_fams_pmenu_edit_clicked),
                   tree_view);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  menuitem = gtk_menu_item_new_with_label(LTR_FAMILIES_FILTER_SELECTION);
  g_signal_connect(menuitem, "activate",
           G_CALLBACK(gtk_ltr_families_list_view_families_pmenu_filter_clicked),
                   ltrfams);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  /* gtk_ltr_families_lv_fams_tb_refseqmatch_clicked */
  menuitem = gtk_menu_item_new_with_label(LTR_FAMILIES_MATCH_SELECTION);
  g_signal_connect(menuitem, "activate",
           G_CALLBACK(gtk_ltr_families_list_view_families_pmenu_match_clicked),
                   ltrfams);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  menuitem = gtk_menu_item_new_with_label(FAMS_REMOVE_SEL);
  g_signal_connect(menuitem, "activate",
                   G_CALLBACK(gtk_ltr_families_lv_fams_pmenu_remove_clicked),
                   ltrfams);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  menuitem = gtk_menu_item_new_with_label(LTR_FAMILIES_MERGE_SELECTION);
  g_signal_connect(menuitem, "activate",
           G_CALLBACK(gtk_ltr_families_list_view_families_pmenu_merge_clicked),
                   ltrfams);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
  if (gtk_tree_selection_count_selected_rows(sel) == 1)
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), FALSE);

  menuitem = gtk_menu_item_new_with_label(FAMS_EXPORT_SEQS_ONE);
  g_signal_connect(menuitem, "activate",
                 G_CALLBACK(gtk_ltr_families_lv_fams_pmenu_export_seqs_one),
                   ltrfams);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  menuitem = gtk_menu_item_new_with_label(FAMS_EXPORT_SEQS_MULT);
  g_signal_connect(menuitem, "activate",
                 G_CALLBACK(gtk_ltr_families_lv_fams_pmenu_export_seqs_mult),
                   ltrfams);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
  if (gtk_tree_selection_count_selected_rows(sel) == 1)
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), FALSE);

  menuitem = gtk_menu_item_new_with_label(FAMS_EXPORT_ANNO_ONE);
  g_signal_connect(menuitem, "activate",
                 G_CALLBACK(gtk_ltr_families_lv_fams_pmenu_export_anno_one),
                   ltrfams);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  menuitem = gtk_menu_item_new_with_label(FAMS_EXPORT_ANNO_MULT);
  g_signal_connect(menuitem, "activate",
                 G_CALLBACK(gtk_ltr_families_lv_fams_pmenu_export_anno_mult),
                   ltrfams);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
  if (gtk_tree_selection_count_selected_rows(sel) == 1)
    gtk_widget_set_sensitive(GTK_WIDGET(menuitem), FALSE);

  gtk_widget_show_all(menu);
  gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,
                 (event != NULL) ? event->button : 0,
                 gdk_event_get_time((GdkEvent*)event));
}

static gboolean gtk_ltr_families_tv_fams_button_pressed(GtkWidget *tree_view,
                                                        GdkEventButton *event,
                                                        GtkLTRFamilies *ltrfams)
{
  if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3)
  {
    GtkTreeSelection *selection;
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
    if (gtk_tree_selection_count_selected_rows(selection)  <= 1)
    {
      GtkTreePath *path;
      if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(tree_view),
                                        (gint) event->x,
                                        (gint) event->y,
                                        &path, NULL, NULL, NULL)) {
        gtk_tree_selection_unselect_all(selection);
        gtk_tree_selection_select_path(selection, path);
        gtk_tree_path_free(path);
      } else
        return TRUE;
    }
    gtk_ltr_families_tv_fams_pmenu(tree_view, event, ltrfams);
    return TRUE;
  }
  return FALSE;
}

static gboolean gtk_ltr_families_tv_fams_on_pmenu(GtkWidget *tree_view,
                                                  GtkLTRFamilies *ltrfams)
{
  gtk_ltr_families_tv_fams_pmenu(tree_view, NULL, ltrfams);
  return TRUE;
}
/* popupmenu related functions end*/

/* <nb_family> related functions start */
void gtk_ltr_families_nb_fam_lv_append_array(GtkLTRFamilies *ltrfams,
                                             GtkTreeView *list_view,
                                             GtArray *nodes,
                                             GtkListStore *store)
{
  GtkTreeIter iter, *tmp_iter;
  GtkTreeModel *model;
  GtGenomeNode *gn;
  GtFeatureNode *curnode;
  GtFeatureNodeIterator *fni;
  GtHashmap *iter_hash;
  gchar *tmp_oldname;
  const gchar *fam;
  unsigned long i;

  if (!store && list_view)
    store = GTK_LIST_STORE(gtk_tree_view_get_model(list_view));

  iter_hash = gt_hashmap_new(GT_HASH_STRING, NULL, free_iter_hash);
  for (i = 0; i < gt_array_size(nodes); i++) {
    gn = *(GtGenomeNode**) gt_array_get(nodes, i);
    fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);
    curnode = gt_feature_node_iterator_next(fni);
    if ((fam = gt_feature_node_get_attribute(curnode, ATTR_LTRFAM)) == NULL) {
      gtk_ltr_families_nb_fam_lv_append_gn(ltrfams, list_view, gn,
                                           NULL, store, ltrfams->style,
                                           ltrfams->colors);
      ltrfams->unclassified_cands++;
    } else {
      GtArray *fam_nodes;
      char tmp_curname[BUFSIZ];

      model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfams->lv_families));
      tmp_iter = (GtkTreeIter*) gt_hashmap_get(iter_hash, (void*) fam);
      if (tmp_iter) {
        CandidateData *cdata;
        GtkTreeRowReference *fam_ref;

        gtk_tree_model_get(model, tmp_iter,
                           LTRFAMS_FAM_LV_NODE_ARRAY, &fam_nodes,
                           LTRFAMS_FAM_LV_OLDNAME, &tmp_oldname,
                           LTRFAMS_FAM_LV_ROWREF, &fam_ref,
                           -1);

        cdata = (CandidateData*) gt_genome_node_get_user_data(gn, "cdata");
        if (!cdata) {
          cdata = g_slice_new(CandidateData);
          cdata->fam_ref = fam_ref;
          cdata->cand_ref = NULL;
          gt_genome_node_add_user_data(gn, "cdata", (gpointer) cdata, NULL);
        } else {
          cdata->fam_ref = fam_ref;
          cdata->cand_ref = NULL;
        }

        gt_array_add(fam_nodes, gn);
        g_snprintf(tmp_curname, BUFSIZ, "%s (%lu)",
                   tmp_oldname, gt_array_size(fam_nodes));
        gtk_list_store_set(GTK_LIST_STORE(model), tmp_iter,
                           LTRFAMS_FAM_LV_CURNAME, tmp_curname,
                           -1);
        g_free(tmp_oldname);
      } else {
        CandidateData *cdata;
        GtkTreeRowReference *fam_ref;
        GtkTreePath *path;

        fam_nodes = gt_array_new(sizeof(GtGenomeNode*));
        gt_array_add(fam_nodes, gn);
        g_snprintf(tmp_curname, BUFSIZ, "%s (%lu)",
                   fam, gt_array_size(fam_nodes));
        gtk_list_store_append(GTK_LIST_STORE(model), &iter);
        path = gtk_tree_model_get_path(model, &iter);
        fam_ref = gtk_tree_row_reference_new(model, path);

        cdata = (CandidateData*) gt_genome_node_get_user_data(gn, "cdata");
        if (!cdata) {
          cdata = g_slice_new(CandidateData);
          cdata->fam_ref = fam_ref;
          cdata->cand_ref = NULL;
          gt_genome_node_add_user_data(gn, "cdata", (gpointer) cdata, NULL);
        } else {
          cdata->fam_ref = fam_ref;
          cdata->cand_ref = NULL;
        }

        gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                           LTRFAMS_FAM_LV_NODE_ARRAY, fam_nodes,
                           LTRFAMS_FAM_LV_TAB_CHILD, NULL,
                           LTRFAMS_FAM_LV_TAB_LABEL, NULL,
                           LTRFAMS_FAM_LV_CURNAME, tmp_curname,
                           LTRFAMS_FAM_LV_OLDNAME, fam,
                           LTRFAMS_FAM_LV_ROWREF, fam_ref,
                           -1);
        gt_hashmap_add(iter_hash,
                       (void*) fam,
                       (void*) gtk_tree_iter_copy(&iter));
        gtk_tree_path_free(path);
      }
    }
    gt_feature_node_iterator_delete(fni);
  }
  gt_hashmap_delete(iter_hash);
}

static void gtk_ltr_families_nb_fam_tb_fl_clicked(GT_UNUSED GtkWidget *button,
                                                  GtkLTRFamilies *ltrfams)
{
  GtkNotebook *notebook;
  GtkTreeView *list_view;
  GtkTreeModel *model1,
               *model2;
  GtkTreeIter iter1,
              iter2;
  GtkTreeRowReference *rowref;
  GtkTreePath *path;
  GtkWidget *tab,
            *dialog,
            *label,
            *vbox,
            *spinb1,
            *spinb2,
            *align,
            *hbox,
            *toplevel;
  GtkObject *adjust;
  GtArray *nodes;
  GtGenomeNode *gn;
  GList *children;
  gboolean valid;
  gchar buffer[BUFSIZ];
  gint curtab_no;
  gfloat ltrtolerance,
         lentolerance;
  unsigned long flcands;

  notebook = GTK_NOTEBOOK(ltrfams->nb_family);
  curtab_no = gtk_notebook_get_current_page(notebook);
  tab = gtk_notebook_get_nth_page(notebook, curtab_no);
  children = gtk_container_get_children(GTK_CONTAINER(tab));
  list_view = GTK_TREE_VIEW(g_list_first(children)->data);
  model1 = gtk_tree_view_get_model(list_view);
  valid = gtk_tree_model_get_iter_first(model1, &iter1);
  if (!valid)
    return; /* TODO: show dialog */
  gtk_tree_model_get(model1, &iter1, LTRFAMS_LV_ROWREF, &rowref, -1);
  model2 = gtk_tree_row_reference_get_model(rowref);
  path = gtk_tree_row_reference_get_path(rowref);
  if (!gtk_tree_model_get_iter(model2, &iter2, path)) {
    /* report programming error */
    return;
  }
  gtk_tree_model_get(model2, &iter2, LTRFAMS_FAM_LV_NODE_ARRAY, &nodes, -1);
  gtk_tree_path_free(path);

  toplevel = gtk_widget_get_toplevel(GTK_WIDGET(ltrfams));
  dialog = gtk_dialog_new_with_buttons(INFORMATION,
                                       GTK_WINDOW(toplevel),
                                       GTK_DIALOG_MODAL, GTK_STOCK_CANCEL,
                                       GTK_RESPONSE_CANCEL, GTK_STOCK_OK,
                                       GTK_RESPONSE_OK, NULL);
  gtk_dialog_set_has_separator(GTK_DIALOG (dialog), FALSE);

  hbox = gtk_hbox_new(FALSE, 5);
  vbox = gtk_vbox_new(TRUE, 1);
  label = gtk_label_new(LTR_TOLERANCE);
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new(LEN_TOLERANCE);
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 1);

  vbox = gtk_vbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(0.0, 0.0, 1000.0, 1.0, 10.0, 0.0);
  spinb1 = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 0.01, 1);
  adjust = gtk_adjustment_new(0.0, 0.0, 1000.0, 1.0, 10.0, 0.0);
  spinb2 = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 0.01, 1);
  gtk_box_pack_start(GTK_BOX(vbox), spinb1, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox), spinb2, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 1);
  gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), hbox);
  gtk_widget_show_all(dialog);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) != GTK_RESPONSE_OK) {
    gtk_widget_destroy(dialog);
    return;
  }

  ltrtolerance = (gfloat) gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinb1));
  lentolerance = (gfloat) gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinb2));
  gtk_widget_destroy(dialog);
  flcands = determine_full_length_candidates(nodes, ltrtolerance, lentolerance);

  g_snprintf(buffer, BUFSIZ, FLCAND_RESULT, flcands);
  dialog = gtk_message_dialog_new(GTK_WINDOW(toplevel),
                                  GTK_DIALOG_MODAL |
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                  "%s", buffer);
  gtk_window_set_title(GTK_WINDOW(dialog), "Results");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);

  valid = gtk_tree_model_get_iter_first(model1, &iter1);
  if (!valid) {
    /* report programming error */
    return;
  }
  gtk_tree_model_get(model1, &iter1, LTRFAMS_LV_NODE, &gn, -1);
  update_list_view_with_flcand(GTK_LIST_STORE(model1), &iter1, gn);

  while (gtk_tree_model_iter_next(model1, &iter1)) {
    gtk_tree_model_get(model1, &iter1, LTRFAMS_LV_NODE, &gn, -1);
    update_list_view_with_flcand(GTK_LIST_STORE(model1), &iter1, gn);
  }
  g_list_free(children);

  gtk_ltr_families_set_modified(ltrfams, TRUE);
}

static void gtk_ltr_families_nb_fam_tb_nf_clicked(GT_UNUSED GtkWidget *button,
                                                  GtkLTRFamilies *ltrfams)
{
  GtkNotebook *notebook;
  GtkTreeView *list_view;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreeSelection *sel;
  GtkTreeRowReference *rowref;
  GtkWidget *tab, *dialog, *toplevel;
  GtGenomeNode *gn;
  GtArray *nodes;
  GtHashmap *sel_features = NULL;
  ThreadData *threaddata;
  GList *rows,
        *tmp,
        *references = NULL,
        *children;
  gchar *fam_prefix = NULL;
  gint curtab_no;

  toplevel = gtk_widget_get_toplevel(GTK_WIDGET(ltrfams));
  notebook = GTK_NOTEBOOK(ltrfams->nb_family);
  curtab_no = gtk_notebook_get_current_page(notebook);
  tab = gtk_notebook_get_nth_page(notebook, curtab_no);
  children = gtk_container_get_children(GTK_CONTAINER(tab));
  list_view = GTK_TREE_VIEW(g_list_first(children)->data);
  sel = gtk_tree_view_get_selection(list_view);
  if (gtk_tree_selection_count_selected_rows(sel) < 3) {
    dialog = gtk_message_dialog_new(GTK_WINDOW(toplevel),
                                    GTK_DIALOG_MODAL |
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_QUESTION, GTK_BUTTONS_OK,
                                    "%s",
                                    NEW_FAM_DIALOG);
    gtk_window_set_title(GTK_WINDOW(dialog), INFORMATION);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }

  sel_features = gt_hashmap_new(GT_HASH_STRING, free_gt_hash_elem, NULL);
  if (!get_features_and_prefix_for_classification(ltrfams, sel_features,
                                                  &fam_prefix)) {
    g_list_free(children);
    gt_hashmap_delete(sel_features);
    return;
  } else {
    GtkTreeModel *tmp_model;
    tmp_model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfams->lv_families));
    while (prefix_in_list_view_fams(tmp_model, fam_prefix)) {
      GtkWidget *label,
                *entry,
                *vbox;
      dialog = gtk_dialog_new_with_buttons(INFORMATION, GTK_WINDOW(toplevel),
                                           GTK_DIALOG_MODAL, GTK_STOCK_CANCEL,
                                           GTK_RESPONSE_CANCEL, GTK_STOCK_OK,
                                           GTK_RESPONSE_OK, NULL);
      gtk_dialog_set_has_separator(GTK_DIALOG (dialog), FALSE);
      label = gtk_label_new(PREFIX_EXISTS);
      entry = gtk_entry_new();
      gtk_entry_set_text(GTK_ENTRY(entry), fam_prefix);
      vbox = gtk_vbox_new(FALSE, 5);
      gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);
      gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 1);
      gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), vbox);
      gtk_widget_show_all(dialog);
      g_free(fam_prefix);
      if (gtk_dialog_run(GTK_DIALOG(dialog)) != GTK_RESPONSE_OK) {
        gt_hashmap_delete(sel_features);
        gtk_widget_destroy(dialog);
        return;
      }
      fam_prefix = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
      gtk_widget_destroy(dialog);
    }
  }

  nodes = gt_array_new(sizeof(GtGenomeNode*));
  model = gtk_tree_view_get_model(list_view);
  rows = gtk_tree_selection_get_selected_rows(sel, &model);
  tmp = rows;
  while (tmp != NULL) {
    rowref = gtk_tree_row_reference_new(model, (GtkTreePath*) tmp->data);
    gtk_tree_model_get_iter(model, &iter, (GtkTreePath*) tmp->data);
    gtk_tree_model_get(model, &iter, LTRFAMS_LV_NODE, &gn, -1);
    gt_array_add(nodes, gn);
    references = g_list_prepend(references,
                                gtk_tree_row_reference_copy(rowref));
    gtk_tree_row_reference_free(rowref);
    tmp = tmp->next;
  }
  gt_genome_nodes_sort_stable(nodes);

  threaddata = threaddata_new();
  threaddata->ltrfams = ltrfams;
  threaddata->progressbar = ltrfams->progressbar;
  threaddata->old_nodes = nodes;
  threaddata->new_nodes = gt_array_new(sizeof(GtGenomeNode*));
  threaddata->err = gt_error_new();
  threaddata->classification = TRUE;
  threaddata->current_state = gt_cstr_dup(START_CLASSIF);
  threaddata->references = references;
  threaddata->list_view = list_view;
  threaddata->sel_features = sel_features;
  threaddata->fam_prefix = fam_prefix;

  progress_dialog_init(threaddata,
                       gtk_widget_get_toplevel(GTK_WIDGET(ltrfams)));

  if (!g_thread_create(classify_nodes_start, (gpointer) threaddata, FALSE,
                       &ltrfams->gerr)) {
    error_handle(gtk_widget_get_toplevel(GTK_WIDGET(ltrfams)), ltrfams->gerr);
  }

  g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
  g_list_free(rows);
  g_list_free(children);
}

static int add_feature_columns(void *key, void *value, void *lv,
                               GT_UNUSED GtError *err)
{
  GtkCellRenderer *renderer;
  GtkTreeView *list_view;
  GtkTreeViewColumn *column;
  GtkTreeSortable *sortable;
  const char *caption;
  unsigned long num;

  list_view = (GtkTreeView*) lv;
  caption = double_underscores((const char*) key);
  num = (unsigned long) value;
  renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "xalign", 1.0, NULL);
  sortable =
          GTK_TREE_SORTABLE(GTK_LIST_STORE(gtk_tree_view_get_model(list_view)));
  column = gtk_tree_view_column_new_with_attributes(caption, renderer, "text",
                                                    num, NULL);
  gtk_tree_view_column_set_resizable(column, true);
  gtk_tree_sortable_set_sort_func(sortable, num,
                                  nb_fam_lv_sort_function,
                                  GINT_TO_POINTER(num), NULL);
  gtk_tree_view_column_set_sort_column_id(column, num);
  if (g_strcmp0((const char*) key, FNT_LLTR) == 0)
    gtk_tree_view_insert_column(list_view, column, 7);
  else if (g_strcmp0((const char*) key, FNT_RLTR) == 0)
    gtk_tree_view_insert_column(list_view, column, 8);
  else
    gtk_tree_view_append_column(list_view, column);
  return 0;
}

static void
gtk_ltr_families_nb_fam_lv_set_visible_columns(GtkNotebook *notebook,
                                               GtkTreeView *list_view1)
{
  GtkWidget *sw;
  GtkTreeView *list_view2;
  GtkTreeViewColumn *column1,
                    *column2;
  GList *children,
        *columns;
  gint main_tab_no;
  guint i, noc;

  main_tab_no =
             GPOINTER_TO_INT(g_object_get_data(G_OBJECT(notebook), "main_tab"));
  sw = gtk_notebook_get_nth_page(notebook, main_tab_no);
  children = gtk_container_get_children(GTK_CONTAINER(sw));
  list_view2 = GTK_TREE_VIEW(g_list_first(children)->data);
  columns = gtk_tree_view_get_columns(list_view2);
  noc = g_list_length(columns);
  /* start with i = 1 because column 0 needs to be visible for families but not
     on general tab */
  for (i = 1; i < noc; i++) {
    column1 = gtk_tree_view_get_column(list_view1, i);
    column2 = gtk_tree_view_get_column(list_view2, i);
    gtk_tree_view_column_set_visible(column1,
                                     gtk_tree_view_column_get_visible(column2));
  }
  g_list_free(children);
  g_list_free(columns);
}

static void gtk_ltr_families_nb_fam_lv_changed(GtkTreeView *list_view,
                                               GtkLTRFamilies *ltrfams)
{
  GtkTreeModel *list_model, *tree_model;
  GtkTreeIter iter, tmp;
  GtkTreeStore *store;
  GtkTreeSelection *selection;
  GtkTreePath *path;
  GtGenomeNode *gn;
  GtFeatureNodeIterator *fni;
  GtFeatureNode *curnode;
  GtRange range;
  GtHashmap *iter_hash;
  GList *rows;
  const char *fnt;

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_view));
  if (gtk_tree_selection_count_selected_rows(selection) != 1)
    return;
  else {
    list_model = gtk_tree_view_get_model(list_view);
    rows = gtk_tree_selection_get_selected_rows(selection, &list_model);
    path = (GtkTreePath*) g_list_first(rows)->data;
    gtk_tree_model_get_iter(list_model, &tmp, path);
    /* get GtGenomeNode */
    gtk_tree_model_get(list_model, &tmp,
                       LTRFAMS_LV_NODE, &gn,
                       -1);
    tree_model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfams->tv_details));
    if (!tree_model) {
      GType *types = g_new0(GType, LTRFAMS_DETAIL_TV_N_COLUMS);

      types[0] = G_TYPE_POINTER;
      types[1] = G_TYPE_STRING;
      types[2] = G_TYPE_ULONG;
      types[3] = G_TYPE_ULONG;

      store = gtk_tree_store_newv(LTRFAMS_DETAIL_TV_N_COLUMS, types);

      gtk_tree_view_set_model(GTK_TREE_VIEW(ltrfams->tv_details),
                              GTK_TREE_MODEL(store));
      g_object_unref(store);
      g_free(types);
    } else {
      store = GTK_TREE_STORE(tree_model);
      gtk_tree_store_clear(store);
    }

    iter_hash = gt_hashmap_new(GT_HASH_STRING, NULL, free_iter_hash);
    fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);
    while ((curnode = gt_feature_node_iterator_next(fni))) {
      fnt = gt_feature_node_get_type(curnode);
      if (g_strcmp0(fnt, FNT_REPEATR) == 0) {
        range = gt_genome_node_get_range((GtGenomeNode*) curnode);
        gtk_tree_store_append(store, &iter, NULL);
        gtk_tree_store_set(store, &iter,
                           LTRFAMS_DETAIL_TV_NODE, gn,
                           LTRFAMS_DETAIL_TV_TYPE, fnt,
                           LTRFAMS_DETAIL_TV_START, range.start,
                           LTRFAMS_DETAIL_TV_END, range.end,
                           -1);
        gt_hashmap_add(iter_hash,
                       (void*) gt_feature_node_get_attribute(curnode, ATTR_RID),
                       (void*) gtk_tree_iter_copy(&iter));
      } else {
        GtkTreeIter *tmp_iter;
        tmp_iter = (GtkTreeIter*) gt_hashmap_get(iter_hash,
                      (void*) gt_feature_node_get_attribute(curnode,
                                                            ATTR_PARENT));
        if (tmp_iter) {
          GtkTreeIter child;
          const char *id;
          range = gt_genome_node_get_range((GtGenomeNode*) curnode);
          gtk_tree_store_append(store, &child, tmp_iter);
          gtk_tree_store_set(store, &child,
                             LTRFAMS_DETAIL_TV_NODE, curnode,
                             LTRFAMS_DETAIL_TV_TYPE, fnt,
                             LTRFAMS_DETAIL_TV_START, range.start,
                             LTRFAMS_DETAIL_TV_END, range.end,
                             -1);
          if ((id = gt_feature_node_get_attribute(curnode, ATTR_RID)))
            gt_hashmap_add(iter_hash, (void*) id,
                           (void*) gtk_tree_iter_copy(&child));
        }
      }
    }
    gtk_tree_view_expand_all(GTK_TREE_VIEW(ltrfams->tv_details));
    gt_feature_node_iterator_delete(fni);
    draw_image(ltrfams, gn);
    gt_hashmap_delete(iter_hash);
    g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
    g_list_free(rows);
  }
}

void gtk_ltr_families_nb_fam_lv_append_gn(GtkLTRFamilies *ltrfams,
                                          GtkTreeView *list_view,
                                          GtGenomeNode *gn,
                                          GtkTreeRowReference *rowref,
                                          GtkListStore *tmp,
                                          GtStyle *style,
                                          GtHashmap *colors)
{
  GtHashmap *features;
  GtFeatureNodeIterator *fni;
  GtFeatureNode *curnode;
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkListStore *store;
  GtError *err = gt_error_new();
  const char *fnt;
  unsigned long cno = 0;
  gboolean first_ltr = TRUE;

  features = ltrfams->features;

  if (!tmp) {
    model = gtk_tree_view_get_model(list_view);
    store = GTK_LIST_STORE(model);
  } else {
    store = tmp;
    model = GTK_TREE_MODEL(store);
  }

  fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);

  while ((curnode = gt_feature_node_iterator_next(fni))) {
    fnt = gt_feature_node_get_type(curnode);
    if ((g_strcmp0(fnt, FNT_REPEATR) == 0)) {
      CandidateData *cdata;
      GtkTreePath *path;
      GtkTreeRowReference *cand_ref;
      char *seqid;
      const char *flcand;

      seqid = gt_str_get(gt_genome_node_get_seqid((GtGenomeNode*) curnode));
      flcand = gt_feature_node_get_attribute(curnode, ATTR_FULLLEN);
      gtk_list_store_append(store, &iter);
      path = gtk_tree_model_get_path(model, &iter);
      cand_ref = gtk_tree_row_reference_new(model, path);
      gtk_list_store_set(store, &iter,
                         LTRFAMS_LV_NODE, gn,
                         LTRFAMS_LV_ROWREF, rowref,
                         LTRFAMS_LV_SEQID, seqid,
                         LTRFAMS_LV_FLCAND, (flcand ? "*" : ""),
                         -1);
      cdata = (CandidateData*) gt_genome_node_get_user_data(gn, "cdata");
      if (!cdata) {
        cdata = g_slice_new(CandidateData);
        cdata->fam_ref = rowref;
        cdata->cand_ref = cand_ref;
        gt_genome_node_add_user_data(gn, "cdata", (gpointer) cdata, NULL);
      } else {
        cdata->fam_ref = rowref;
        gtk_tree_row_reference_free(cdata->cand_ref);
        cdata->cand_ref = cand_ref;
      }
      gtk_tree_path_free(path);
    } else if (g_strcmp0(fnt, FNT_PROTEINM) == 0) {
      fnt = gt_feature_node_get_attribute(curnode, ATTR_PFAMN);
      if (!fnt)
        continue;
      if (gt_hashmap_get(features, fnt) != NULL) {
        const char *clid;
        clid = gt_feature_node_get_attribute(curnode, ATTR_CLUSTID);
        cno = (unsigned long) gt_hashmap_get(features, fnt);
        gtk_list_store_set(store, &iter, cno, clid, -1);
      }
    } else if (g_strcmp0(fnt, FNT_LTRRETRO) == 0) {
      GtRange range;
      GtStrand strand = gt_feature_node_get_strand(curnode);
      gchar c[2];

      g_snprintf(c, 2, "%c", GT_STRAND_CHARS[strand]);
      range = gt_genome_node_get_range((GtGenomeNode*) curnode);
      gtk_list_store_set(store, &iter,
                         LTRFAMS_LV_STRAND, c,
                         LTRFAMS_LV_START, range.start,
                         LTRFAMS_LV_END, range.end,
                         LTRFAMS_LV_ELEMLEN, gt_range_length(&range),
                         -1);
    } else if (g_strcmp0(fnt, FNT_LTR) == 0) {
      GtRange range;
      if (first_ltr) {
        range = gt_genome_node_get_range((GtGenomeNode*) curnode);
        fnt = FNT_LLTR;
        first_ltr = FALSE;
        gtk_list_store_set(store, &iter,
                           LTRFAMS_LV_LLTRLEN, gt_range_length(&range),
                           -1);
      } else
        fnt = FNT_RLTR;
      if (gt_hashmap_get(features, fnt) != NULL) {
        const char *clid;
        clid = gt_feature_node_get_attribute(curnode, ATTR_CLUSTID);
        cno = (unsigned long) gt_hashmap_get(features, fnt);
        gtk_list_store_set(store, &iter, cno, clid, -1);
      }
    } else if ((gt_hashmap_get(features, fnt)) != NULL) {
      const char *clid;
      clid = gt_feature_node_get_attribute(curnode, ATTR_CLUSTID);
      cno = (unsigned long) gt_hashmap_get(features, fnt);
      gtk_list_store_set(store, &iter, cno, clid, -1);
    }
    if (style) {
      const char *caption, *hexcode;

      caption = double_underscores(fnt);
      hexcode = (const char*) gt_hashmap_get(colors, caption);
      if (hexcode == NULL) {
        gint r, g, b;
        char hex[8];
        GtColor *color = gt_color_new(0.0, 0.0, 0.0, 0.0);
        gt_style_get_color(style, gt_feature_node_get_type(curnode),
                           "fill", color, curnode, err);
        r = 256 * color->red;
        g = 256 * color->green;
        b = 256 * color->blue;
        g_snprintf(hex, 8, "#%x%x%x", r, g, b);
        gt_hashmap_add(colors, gt_cstr_dup(caption), gt_cstr_dup(hex));
      }
    }
  }
  gt_feature_node_iterator_delete(fni);
  gt_error_delete(err);
  return;
}

static gint nb_fam_lv_sort_function(GtkTreeModel *model, GtkTreeIter *a,
                                    GtkTreeIter *b, gpointer userdata)
{
  GtkTreeSortable *sortable;
  GtkSortType order;
  gchar *val1, *val2;
  gint ret = 0,
       sortcol = GPOINTER_TO_INT(userdata),
       tmp;
  unsigned long x, y;

  sortable = GTK_TREE_SORTABLE(model);

  gtk_tree_sortable_get_sort_column_id(sortable, &tmp, &order);

  switch (sortcol) {
    case LTRFAMS_LV_FLCAND:
    case LTRFAMS_LV_SEQID:
    case LTRFAMS_LV_STRAND:
      gtk_tree_model_get(model, a,
                         sortcol, &val1,
                         -1);
      gtk_tree_model_get(model, b,
                         sortcol, &val2,
                         -1);
      if (val1 == NULL || val2 == NULL) {
        if (val1 == NULL && val2 == NULL)
           break; /* both equal => ret = 0 */
        ret = (val1 == NULL) ? -1 : 1;
      }
      else
        ret = g_utf8_collate(val1, val2);
      g_free(val1);
      g_free(val2);
      break;
    case LTRFAMS_LV_START:
    case LTRFAMS_LV_END:
    case LTRFAMS_LV_LLTRLEN:
    case LTRFAMS_LV_ELEMLEN:
      gtk_tree_model_get(model, a,
                         sortcol, &x,
                         -1);
      gtk_tree_model_get(model, b,
                         sortcol, &y,
                         -1);
      if (x != y)
        ret = (x > y) ? 1 : -1;
      /* else both equal => ret = 0 */
      break;
    default:
      gtk_tree_model_get(model, a,
                         sortcol, &val1,
                         -1);
      gtk_tree_model_get(model, b,
                         sortcol, &val2,
                         -1);
      if (val1 == NULL || val2 == NULL) {
        if (val1 == NULL && val2 == NULL) {
          g_free(val1);
          g_free(val2);
          break; /* both equal => ret = 0 */
        }
        /* NULL should always be less than 0 */
        if (order == GTK_SORT_DESCENDING)
          ret = (val1 == NULL) ? -1 : 1;
        else
          ret = (val1 == NULL) ? 1 : -1;
      } else {
        sscanf(val1, "%lu", &x);
        sscanf(val2, "%lu", &y);
        if (x != y)
          ret = (x > y) ? 1 : -1;
        /* else both equal => ret = 0 */
        g_free(val1);
        g_free(val2);
      }
  }
  return ret;
}

static void gtk_ltr_families_nb_fam_lv_create(GtkTreeView *list_view,
                                              GtArray *nodes,
                                              GtkLTRFamilies *ltrfams)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkTreeSortable *sortable;
  GtkListStore *store;
  GList *columns;
  GType *types;
  GtError *err = NULL;
  unsigned long i;

  types = g_new0(GType, ltrfams->n_features);

  types[0] = G_TYPE_POINTER;
  types[1] = G_TYPE_POINTER;
  types[2] = G_TYPE_STRING;
  types[3] = G_TYPE_STRING;
  types[4] = G_TYPE_STRING;
  types[5] = G_TYPE_ULONG;
  types[6] = G_TYPE_ULONG;
  types[7] = G_TYPE_ULONG;
  types[8] = G_TYPE_ULONG;

  for (i = LTRFAMS_LV_N_COLUMS; i < ltrfams->n_features; i++) {
    types[i] = G_TYPE_STRING;
  }

  GtkTreeSelection *sel = gtk_tree_view_get_selection(list_view);
  gtk_tree_selection_set_mode(sel, GTK_SELECTION_MULTIPLE);

  store = gtk_list_store_newv(ltrfams->n_features, types);

  sortable = GTK_TREE_SORTABLE(store);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("",
                                                    renderer, "text",
                                                    LTRFAMS_LV_FLCAND, NULL);
  gtk_tree_sortable_set_sort_func(sortable, LTRFAMS_LV_FLCAND,
                                  nb_fam_lv_sort_function,
                                  GINT_TO_POINTER(LTRFAMS_LV_FLCAND), NULL);
  gtk_tree_view_column_set_sort_column_id(column, LTRFAMS_LV_FLCAND);
  gtk_tree_view_append_column(list_view, column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(LTRFAMS_LV_CAPTION_SEQID,
                                                    renderer, "text",
                                                    LTRFAMS_LV_SEQID, NULL);
  gtk_tree_sortable_set_sort_func(sortable, LTRFAMS_LV_SEQID,
                                  nb_fam_lv_sort_function,
                                  GINT_TO_POINTER(LTRFAMS_LV_SEQID), NULL);
  gtk_tree_view_column_set_sort_column_id(column, LTRFAMS_LV_SEQID);
  gtk_tree_view_append_column(list_view, column);

  renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "xalign", 1.0, NULL);
  column = gtk_tree_view_column_new_with_attributes(LTRFAMS_LV_CAPTION_STRAND,
                                                    renderer, "text",
                                                    LTRFAMS_LV_STRAND, NULL);
  gtk_tree_sortable_set_sort_func(sortable, LTRFAMS_LV_STRAND,
                                  nb_fam_lv_sort_function,
                                  GINT_TO_POINTER(LTRFAMS_LV_STRAND), NULL);
  gtk_tree_view_column_set_sort_column_id(column, LTRFAMS_LV_STRAND);
  gtk_tree_view_append_column(list_view, column);

  renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "xalign", 1.0, NULL);
  column = gtk_tree_view_column_new_with_attributes(LTRFAMS_LV_CAPTION_START,
                                                    renderer, "text",
                                                    LTRFAMS_LV_START, NULL);
  gtk_tree_sortable_set_sort_func(sortable, LTRFAMS_LV_START,
                                  nb_fam_lv_sort_function,
                                  GINT_TO_POINTER(LTRFAMS_LV_START), NULL);
  gtk_tree_view_column_set_sort_column_id(column, LTRFAMS_LV_START);
  gtk_tree_view_append_column(list_view, column);

  renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "xalign", 1.0, NULL);
  column = gtk_tree_view_column_new_with_attributes(LTRFAMS_LV_CAPTION_END,
                                                    renderer, "text",
                                                    LTRFAMS_LV_END, NULL);
  gtk_tree_sortable_set_sort_func(sortable, LTRFAMS_LV_END,
                                  nb_fam_lv_sort_function,
                                  GINT_TO_POINTER(LTRFAMS_LV_END), NULL);
  gtk_tree_view_column_set_sort_column_id(column, LTRFAMS_LV_END);
  gtk_tree_view_append_column(list_view, column);

  renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "xalign", 1.0, NULL);
  column = gtk_tree_view_column_new_with_attributes(LTRFAMS_LV_CAPTION_LLTRLEN,
                                                    renderer, "text",
                                                    LTRFAMS_LV_LLTRLEN, NULL);
  gtk_tree_sortable_set_sort_func(sortable, LTRFAMS_LV_LLTRLEN,
                                  nb_fam_lv_sort_function,
                                  GINT_TO_POINTER(LTRFAMS_LV_LLTRLEN), NULL);
  gtk_tree_view_column_set_sort_column_id(column, LTRFAMS_LV_LLTRLEN);
  gtk_tree_view_append_column(list_view, column);

  renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "xalign", 1.0, NULL);
  column = gtk_tree_view_column_new_with_attributes(LTRFAMS_LV_CAPTION_ELEMLEN,
                                                    renderer, "text",
                                                    LTRFAMS_LV_ELEMLEN, NULL);
  gtk_tree_sortable_set_sort_func(sortable, LTRFAMS_LV_ELEMLEN,
                                  nb_fam_lv_sort_function,
                                  GINT_TO_POINTER(LTRFAMS_LV_ELEMLEN), NULL);
  gtk_tree_view_column_set_sort_column_id(column, LTRFAMS_LV_ELEMLEN);
  gtk_tree_view_append_column(list_view, column);

  /* set model for <list_view> because it is needed in function
     add_feature_columns */
  gtk_tree_view_set_model(list_view, GTK_TREE_MODEL(store));

  gt_hashmap_foreach(ltrfams->features, add_feature_columns,
                     (void*) list_view, err);

  gtk_tree_view_set_model(list_view, NULL);

  column = gtk_tree_view_column_new();
  gtk_tree_view_append_column(list_view, column);

  if (nodes)
    gtk_ltr_families_nb_fam_lv_append_array(ltrfams, list_view, nodes, store);

  columns = gtk_tree_view_get_columns(list_view);
  set_cell_colors(columns, ltrfams->colors);

  gtk_tree_view_set_model(list_view, GTK_TREE_MODEL(store));
  g_object_unref(store);

  g_signal_connect(G_OBJECT(list_view), "cursor-changed",
                   G_CALLBACK(gtk_ltr_families_nb_fam_lv_changed), ltrfams);
  g_free(types);
  g_list_free(columns);
}

static void gtk_ltr_families_nb_fam_refresh_nums(gpointer user_data)
{
  GtkLTRFamilies *ltrfams = (GtkLTRFamilies*) user_data;
  GtRDB *rdb = NULL;
  GtRDBStmt *stmt = NULL;
  GtkNotebook *notebook;
  GtkWidget *tab_child,
            *tab_label;
  gchar *file,
        query[BUFSIZ];
  gint n_pages,
       i,
       old_pagenum,
       had_err = 0;

  notebook = GTK_NOTEBOOK(ltrfams->nb_family);
  n_pages = gtk_notebook_get_n_pages(notebook);
  file = gtk_ltr_families_get_projectfile(ltrfams);
  if (file)
    rdb = gt_rdb_sqlite_new(file, ltrfams->err);
  for (i = 0; i < n_pages; i++) {
    tab_child = gtk_notebook_get_nth_page(notebook, i);
    tab_label = gtk_notebook_get_tab_label(notebook, tab_child);
    old_pagenum =
       GPOINTER_TO_INT(gtk_label_close_get_button_data(GTKLABELCLOSE(tab_label),
                                                       "nbpage"));
    if (old_pagenum != i) {
      gtk_label_close_set_button_data(GTKLABELCLOSE(tab_label),
                                      "nbpage",
                                      GINT_TO_POINTER(i));
      if (g_strcmp0(gtk_label_close_get_text(GTKLABELCLOSE(tab_label)),
                    MAIN_TAB_LABEL) == 0) {
        g_object_set_data(G_OBJECT(notebook),
                          "main_tab",
                          GINT_TO_POINTER(i));
      }
      if (rdb) {
        g_snprintf(query, BUFSIZ,
                   "UPDATE notebook_tabs SET position = %d WHERE name = \"%s\"",
                   i, gtk_label_close_get_text(GTKLABELCLOSE(tab_label)));
        had_err = gt_rdb_prepare(rdb, query, -1, &stmt, ltrfams->err);
        if (!had_err)
          had_err = gt_rdb_stmt_exec(stmt, ltrfams->err);
        gt_rdb_stmt_delete(stmt);
      }
    }
  }
  if (rdb)
    gt_rdb_delete(rdb);
}

static void gtk_ltr_families_nb_fam_close_tab_clicked(GtkButton *button,
                                                      GtkLTRFamilies *ltrfams)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkWidget *tab_label;
  gint nbpage,
       tmp = -1;

  nbpage = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "nbpage"));
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfams->lv_families));
  gtk_tree_model_get_iter_first(model, &iter);
  gtk_tree_model_get(model, &iter,
                     LTRFAMS_FAM_LV_TAB_LABEL, &tab_label,
                     -1);
  if (tab_label)
    tmp = GPOINTER_TO_INT(
           gtk_label_close_get_button_data(GTKLABELCLOSE(tab_label), "nbpage"));
  if (nbpage == tmp) {
    gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                       LTRFAMS_FAM_LV_TAB_CHILD, NULL,
                       LTRFAMS_FAM_LV_TAB_LABEL, NULL,
                       -1);
  } else {
    while (gtk_tree_model_iter_next(model, &iter)) {
      gtk_tree_model_get(model, &iter,
                         LTRFAMS_FAM_LV_TAB_LABEL, &tab_label,
                         -1);
      if (tab_label)
        tmp = GPOINTER_TO_INT(
               gtk_label_close_get_button_data(GTKLABELCLOSE(tab_label),
                                               "nbpage"));
      if (nbpage == tmp) {
        gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                           LTRFAMS_FAM_LV_TAB_CHILD, NULL,
                           LTRFAMS_FAM_LV_TAB_LABEL, NULL,
                           -1);
        break;
      }
    }
  }

  gchar *file;
  file = gtk_ltr_families_get_projectfile(ltrfams);
  if (file) {
    GtRDB *rdb = NULL;
    GtRDBStmt *stmt;
    gchar query[BUFSIZ];
    gint had_err = 0;

    rdb = gt_rdb_sqlite_new(file, ltrfams->err);
    if (!rdb)
      return;
    g_snprintf(query, BUFSIZ,
               "DELETE FROM notebook_tabs WHERE name = \"%s\"",
               gtk_label_close_get_text(GTKLABELCLOSE(tab_label)));
    had_err = gt_rdb_prepare(rdb, query, -1, &stmt, ltrfams->err);
    if (!had_err)
      had_err = gt_rdb_stmt_exec(stmt, ltrfams->err);
    gt_rdb_stmt_delete(stmt);
    gt_rdb_delete(rdb);
  }
  gtk_notebook_remove_page(GTK_NOTEBOOK(ltrfams->nb_family), nbpage);
  gtk_ltr_families_nb_fam_refresh_nums((gpointer) ltrfams);
}

gboolean gtk_ltr_families_nb_fam_key_pressed(GT_UNUSED GtkWidget *list_view,
                                             GdkEventKey *event,
                                             GtkLTRFamilies *ltrfams)
{
  switch (event->keyval) {
    case GDK_Delete:
      gtk_ltr_families_nb_fam_lv_pmenu_remove_clicked(NULL, ltrfams);
      return TRUE;
      break;
    default:
      break;
  }
  return FALSE;
}

void gtk_ltr_families_nb_fam_add_tab(GtkTreeModel *model,
                                     GtkTreeIter *iter,
                                     GtArray *nodes,
                                     gboolean load,
                                     GtkLTRFamilies *ltrfams)
{
  GtGenomeNode *gn;
  GtkTreeRowReference *rowref;
  GtkTreePath *path;
  GtkWidget *child,
            *label,
            *list_view;
  gint nbpage;
  unsigned long i;
  gchar *name,
        *file;

  gtk_tree_model_get(model, iter,
                     LTRFAMS_FAM_LV_OLDNAME, &name,
                     -1);
  child = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(child),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);

  list_view = gtk_tree_view_new();
  g_signal_connect(G_OBJECT(list_view),
                   "key-press-event",
                   G_CALLBACK(gtk_ltr_families_nb_fam_key_pressed), ltrfams);
  gtk_ltr_families_nb_fam_lv_create(GTK_TREE_VIEW(list_view), NULL, ltrfams);

  gtk_drag_source_set(list_view,
                      GDK_BUTTON1_MASK, family_drag_targets,
                      G_N_ELEMENTS(family_drag_targets),
                      GDK_ACTION_MOVE);
  /* Attach a "drag-data-get" signal to send out the dragged data */
  g_signal_connect(G_OBJECT(list_view),
                   "drag-data-get",
                   G_CALLBACK(on_drag_data_get), NULL);
  g_signal_connect(G_OBJECT(list_view),
                   "button-press-event",
                   G_CALLBACK(gtk_ltr_families_nb_fam_lv_button_pressed),
                   ltrfams);
  g_signal_connect(G_OBJECT(list_view), "popup-menu",
                   G_CALLBACK(gtk_ltr_families_nb_fam_lv_on_pmenu), ltrfams);
  gtk_container_add(GTK_CONTAINER(child), list_view);
  gtk_widget_show_all(child);

  path = gtk_tree_model_get_path(model, iter);
  rowref = gtk_tree_row_reference_new(model, path);
  for (i = 0; i < gt_array_size(nodes); i++) {
    gn = *(GtGenomeNode**) gt_array_get(nodes, i);
    gtk_ltr_families_nb_fam_lv_append_gn(ltrfams, GTK_TREE_VIEW(list_view), gn,
                                         rowref, NULL, ltrfams->style,
                                         ltrfams->colors);
  }
  gtk_tree_path_free(path);

  label = gtk_label_close_new(name,
                          G_CALLBACK(gtk_ltr_families_nb_fam_close_tab_clicked),
                             ltrfams);
  nbpage = gtk_notebook_append_page(GTK_NOTEBOOK(ltrfams->nb_family),
                                    child,
                                    label);
  gtk_label_close_set_button_data(GTKLABELCLOSE(label),
                                  "nbpage",
                                  GINT_TO_POINTER(nbpage));
  gtk_notebook_set_tab_reorderable(GTK_NOTEBOOK(ltrfams->nb_family),
                                   child,
                                   TRUE);
  gtk_list_store_set(GTK_LIST_STORE(model), iter,
                     LTRFAMS_FAM_LV_TAB_CHILD, list_view,
                     LTRFAMS_FAM_LV_TAB_LABEL, label,
                     -1);
  gtk_ltr_families_nb_fam_lv_set_visible_columns(
                                               GTK_NOTEBOOK(ltrfams->nb_family),
                                                 GTK_TREE_VIEW(list_view));
  gtk_notebook_set_current_page(GTK_NOTEBOOK(ltrfams->nb_family), nbpage);
  file = gtk_ltr_families_get_projectfile(ltrfams);
  if (file && !load) {
    GtRDB *rdb = NULL;
    GtRDBStmt *stmt = NULL;
    gchar query[BUFSIZ];
    gint had_err = 0;

    rdb = gt_rdb_sqlite_new(file, ltrfams->err);
    if (!rdb) {
      g_free(name);
      return;
    }
    g_snprintf(query, BUFSIZ,
               "INSERT OR IGNORE INTO notebook_tabs "
               " (name, position) values (\"%s\", %d)",
               name, nbpage);
    had_err = gt_rdb_prepare(rdb, query, -1, &stmt, ltrfams->err);
    if (!had_err)
      had_err = gt_rdb_stmt_exec(stmt, ltrfams->err);
    gt_rdb_stmt_delete(stmt);
    gt_rdb_delete(rdb);
  }
  g_free(name);
}

static void
gtk_ltr_families_nb_fam_page_reordered(GT_UNUSED GtkNotebook *notebook,
                                       GT_UNUSED GtkWidget *child,
                                       GT_UNUSED guint page_num,
                                       gpointer user_data)
{
  gtk_ltr_families_nb_fam_refresh_nums(user_data);
}

static gboolean gtk_ltr_families_nb_fam_switch_page(GtkNotebook *notebook,
                                                    GT_UNUSED gpointer arg1,
                                                    guint arg2,
                                                    GtkLTRFamilies *ltrfams)
{
  gint main_tab;
  main_tab = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(notebook), "main_tab"));
  if (main_tab != arg2) {
    gtk_widget_set_sensitive(GTK_WIDGET(ltrfams->new_fam), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(ltrfams->fl_cands), TRUE);
  } else {
    gtk_widget_set_sensitive(GTK_WIDGET(ltrfams->new_fam), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(ltrfams->fl_cands), FALSE);
  }
  return FALSE;
}

static void gtk_ltr_families_nb_fam_create(GtkLTRFamilies *ltrfams)
{
  GtkWidget *child,
            *label,
            *list_view;
  GtkTreeViewColumn *column;
  gint nbpage;

  gtk_notebook_set_scrollable(GTK_NOTEBOOK(ltrfams->nb_family), TRUE);

  g_signal_connect(G_OBJECT(ltrfams->nb_family), "page-reordered",
                   G_CALLBACK(gtk_ltr_families_nb_fam_page_reordered), ltrfams);

  child = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(child),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
  list_view = gtk_tree_view_new();
  g_signal_connect(G_OBJECT(list_view), "key-press-event",
                   G_CALLBACK(gtk_ltr_families_nb_fam_key_pressed), ltrfams);
  gtk_ltr_families_nb_fam_lv_create(GTK_TREE_VIEW(list_view), ltrfams->nodes,
                                    ltrfams);
  column = gtk_tree_view_get_column(GTK_TREE_VIEW(list_view), 0);
  gtk_tree_view_column_set_visible(column, FALSE);
  gtk_drag_source_set(list_view,
                      GDK_BUTTON1_MASK,
                      family_drag_targets,
                      G_N_ELEMENTS(family_drag_targets),
                      GDK_ACTION_MOVE);
  /* Attach a "drag-data-get" signal to send out the dragged data */
  g_signal_connect(G_OBJECT(list_view),
                   "drag-data-get",
                   G_CALLBACK(on_drag_data_get), NULL);
  g_signal_connect(G_OBJECT(list_view),
                   "button-press-event",
                   G_CALLBACK(gtk_ltr_families_nb_fam_lv_button_pressed),
                   ltrfams);

  gtk_container_add(GTK_CONTAINER(child), list_view);
  gtk_widget_show_all(child);

  label = gtk_label_close_new(MAIN_TAB_LABEL, NULL, NULL);
  gtk_label_close_hide_close(GTKLABELCLOSE(label));
  gtk_widget_show(label);

  /* Attach a "drag-data-received" signal to pull in the dragged data */
  g_signal_connect(G_OBJECT(ltrfams->lv_families), "drag-data-received",
                   G_CALLBACK(on_drag_data_received),
                   ltrfams);

  nbpage = gtk_notebook_append_page(GTK_NOTEBOOK(ltrfams->nb_family),
                                    child, label);
  gtk_notebook_set_tab_reorderable(GTK_NOTEBOOK(ltrfams->nb_family),
                                   child,
                                   TRUE);
  gtk_label_close_set_button_data(GTKLABELCLOSE(label),
                                  "nbpage",
                                  GINT_TO_POINTER(nbpage));
  g_object_set_data(G_OBJECT(ltrfams->nb_family),
                    "main_tab",
                    GINT_TO_POINTER(nbpage));
  g_signal_connect(G_OBJECT(ltrfams->nb_family), "switch-page",
                   G_CALLBACK(gtk_ltr_families_nb_fam_switch_page),
                   (gpointer) ltrfams);
}
/* <nb_family> related functions end */

/* <tv_details> related functions start */
static void gtk_ltr_families_tv_det_changed(GtkTreeView *tree_view,
                                            GtkLTRFamilies *ltrfams)
{
  GtkTreeSelection *selection;
  GtkTreeModel *tree_model;
  GtkTreeIter iter, child;
  GtGenomeNode *gn;
  GtFeatureNode *fn, *curnode;
  GtFeatureNodeIterator *fni;

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
  if (gtk_tree_selection_count_selected_rows(selection) != 1)
    return;
  tree_model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));
  gtk_tree_selection_get_selected(selection, &tree_model, &child);
  gtk_tree_model_get_iter_first(tree_model, &iter);
  gtk_tree_model_get(tree_model, &iter,
                     LTRFAMS_DETAIL_TV_NODE, &gn,
                     -1);
  fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);
  while ((curnode = gt_feature_node_iterator_next(fni))) {
    if (gt_feature_node_is_marked(curnode))
      gt_feature_node_unmark(curnode);
  }
  gt_feature_node_iterator_delete(fni);
  gtk_tree_model_get(tree_model, &child,
                     LTRFAMS_DETAIL_TV_NODE, &fn,
                     -1);
  gt_feature_node_mark(fn);
  draw_image(ltrfams, gn);
}

static void gtk_ltr_families_tv_det_create(GtkTreeView *tree_view)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(LTRFAMS_TV_CAPTION_TYPE,
                                                    renderer, "text",
                                                    LTRFAMS_DETAIL_TV_TYPE,
                                                    NULL);
  gtk_tree_view_append_column(tree_view, column);

  renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "xalign", 1.0, NULL);
  column = gtk_tree_view_column_new_with_attributes(LTRFAMS_LV_CAPTION_START,
                                                    renderer, "text",
                                                    LTRFAMS_DETAIL_TV_START,
                                                    NULL);
  gtk_tree_view_append_column(tree_view, column);

  renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "xalign", 1.0, NULL);
  column = gtk_tree_view_column_new_with_attributes(LTRFAMS_LV_CAPTION_END,
                                                    renderer, "text",
                                                    LTRFAMS_DETAIL_TV_END,
                                                    NULL);
  gtk_tree_view_append_column(tree_view, column);

  column = gtk_tree_view_column_new();
  gtk_tree_view_append_column(tree_view, column);

  gtk_widget_show_all(GTK_WIDGET(tree_view));
}
/* <tv_details> related functions end */

/* <lv_families> related functions start */
static gboolean
gtk_ltr_families_lv_fams_check_if_name_exists(GtkTreeView *list_view,
                                              const gchar *new_name,
                                              const gchar *tmp)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  gchar *famname,
        *iter_str;

  model = gtk_tree_view_get_model(list_view);
  gtk_tree_model_get_iter_first(model, &iter);
  gtk_tree_model_get(model, &iter,
                     LTRFAMS_FAM_LV_OLDNAME, &famname,
                     -1);
  iter_str = gtk_tree_model_get_string_from_iter(model, &iter);
  if ((g_strcmp0(famname, new_name) == 0) && (g_strcmp0(iter_str, tmp) != 0)) {
    g_free(famname);
    g_free(iter_str);
    return TRUE;
  }
  while (gtk_tree_model_iter_next(model, &iter)) {
    iter_str = gtk_tree_model_get_string_from_iter(model, &iter);
    gtk_tree_model_get(model, &iter,
                       LTRFAMS_FAM_LV_OLDNAME, &famname,
                       -1);
    if ((g_strcmp0(famname, new_name) == 0) &&
        (g_strcmp0(iter_str, tmp) != 0)) {
      g_free(famname);
      g_free(iter_str);
      return TRUE;
    }
    g_free(famname);
    g_free(iter_str);
  }
  return FALSE;
}

static void
gtk_ltr_families_lv_fams_tb_add_clicked(GT_UNUSED GtkWidget *button,
                                        GtkLTRFamilies *ltrfams)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkListStore *store;
  GtkTreeViewColumn *column;
  GtkTreePath *path;
  GtkTreeSelection *selection;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfams->lv_families));
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(ltrfams->lv_families));
  column = gtk_tree_view_get_column(GTK_TREE_VIEW(ltrfams->lv_families), 0);
  gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
  store = GTK_LIST_STORE(model);
  g_object_set(ltrfams->lv_fams_renderer, "editable", TRUE, NULL);

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter,
                     LTRFAMS_FAM_LV_NODE_ARRAY, NULL,
                     LTRFAMS_FAM_LV_TAB_CHILD, NULL,
                     LTRFAMS_FAM_LV_TAB_LABEL, NULL,
                     -1);

  path = gtk_tree_model_get_path(model, &iter);
  gtk_tree_view_set_cursor(GTK_TREE_VIEW(ltrfams->lv_families),
                           path, column, TRUE);
  gtk_tree_path_free(path);
}

static void gtk_ltr_families_lv_fams_tb_rm_clicked(GT_UNUSED GtkWidget *button,
                                                   GtkLTRFamilies *ltrfams)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  GtkTreeRowReference *rowref;
  GtkWidget *dialog;
  GList *rows, *tmp, *references = NULL;
  GtArray *nodes;
  gchar text[BUFSIZ];
  gboolean valid;

  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(ltrfams->lv_families));
  gtk_tree_selection_unselect_all(sel);
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfams->lv_families));

  valid = gtk_tree_model_get_iter_first(model, &iter);

  if (!valid)
    return;

  gtk_tree_model_get(model, &iter,
                     LTRFAMS_FAM_LV_NODE_ARRAY, &nodes,
                     -1);
  if (nodes && gt_array_size(nodes) < 3)
    gtk_tree_selection_select_iter(sel, &iter);
  while (gtk_tree_model_iter_next(model, &iter)) {
    gtk_tree_model_get(model, &iter,
                       LTRFAMS_FAM_LV_NODE_ARRAY, &nodes,
                       -1);
    if (nodes && gt_array_size(nodes) < 3)
      gtk_tree_selection_select_iter(sel, &iter);
  }
  if (gtk_tree_selection_count_selected_rows(sel) == 0)
    return;

  g_snprintf(text, BUFSIZ, FAMS_RM_DIALOG,
             gtk_tree_selection_count_selected_rows(sel));
  dialog = gtk_message_dialog_new(NULL,
                                  GTK_DIALOG_MODAL |
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
                                  "%s",
                                  text);
  gtk_window_set_title(GTK_WINDOW(dialog), ATTENTION);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);

  rows = gtk_tree_selection_get_selected_rows(sel, &model);
  if (gtk_dialog_run(GTK_DIALOG(dialog)) != GTK_RESPONSE_YES) {
    gtk_widget_destroy(dialog);
    gtk_tree_selection_unselect_all(sel);
    return;
  } else {
    gtk_widget_destroy(dialog);
    tmp = rows;

    while (tmp != NULL) {
      rowref = gtk_tree_row_reference_new(model, (GtkTreePath*) tmp->data);
      references = g_list_prepend(references,
                                  gtk_tree_row_reference_copy(rowref));
      gtk_tree_row_reference_free(rowref);
      tmp = tmp->next;
    }

    g_list_foreach(references, (GFunc) remove_family, ltrfams);
    g_list_foreach(references, (GFunc) gtk_tree_row_reference_free, NULL);
    g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
    g_list_free(references);
    g_list_free(rows);
    gtk_ltr_families_set_modified(ltrfams, TRUE);
    update_main_tab_label(ltrfams);
  }
}

static void
gtk_ltr_families_lv_fams_cell_edited(GT_UNUSED GtkCellRendererText *cell,
                                     gchar *path_string, gchar *new_name,
                                     GtkLTRFamilies *ltrfams)
{
  GtkTreeView *list_view;
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  gchar *old_name;

  list_view = GTK_TREE_VIEW(ltrfams->lv_families);
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(list_view));
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_view));
  gtk_tree_model_get_iter_from_string(model, &iter, path_string);
  gtk_tree_model_get(model, &iter,
                     LTRFAMS_FAM_LV_OLDNAME, &old_name,
                     -1);

  if (!old_name && (g_strcmp0(new_name, "") == 0))
    gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
  else if (g_strcmp0(new_name, "") == 0) {
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(NULL,
                                    GTK_DIALOG_MODAL |
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                    "%s",
                                    FAMS_EMPTY_DIALOG);
    gtk_window_set_title(GTK_WINDOW(dialog), INFORMATION);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  } else {
    GtkWidget *label;
    GtArray *nodes;
    gchar *iter_str;
    gboolean exists;
    iter_str = gtk_tree_model_get_string_from_iter(model, &iter);
    exists =
         gtk_ltr_families_lv_fams_check_if_name_exists(GTK_TREE_VIEW(list_view),
                                                       new_name, iter_str);
    if (exists) {
      GtkWidget *dialog;
      dialog = gtk_message_dialog_new(NULL,
                                      GTK_DIALOG_MODAL |
                                      GTK_DIALOG_DESTROY_WITH_PARENT,
                                      GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                      "%s",
                                      FAMS_EXIST_DIALOG);
      gtk_window_set_title(GTK_WINDOW(dialog), INFORMATION);
      gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
      gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
      g_signal_emit_by_name(ltrfams->lv_fams_renderer, "editing-canceled");
    } else {
      char tmp_name[BUFSIZ];
      gtk_tree_model_get(model, &iter,
                         LTRFAMS_FAM_LV_NODE_ARRAY, &nodes,
                         -1);
      if (!nodes) {
        nodes = gt_array_new(sizeof(GtGenomeNode*));
        gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                           LTRFAMS_FAM_LV_NODE_ARRAY, nodes,
                           -1);
      } else
        update_genomenode_famname(nodes, new_name);
      g_snprintf(tmp_name, BUFSIZ, "%s (%lu)", new_name, gt_array_size(nodes));
      gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                         LTRFAMS_FAM_LV_CURNAME, tmp_name,
                         LTRFAMS_FAM_LV_OLDNAME, new_name,
                         -1);
      gtk_tree_model_get(model, &iter,
                         LTRFAMS_FAM_LV_TAB_LABEL, &label,
                         -1);
      if (label)
        gtk_label_close_set_text(GTKLABELCLOSE(label), new_name);

      g_free(iter_str);
      gtk_ltr_families_set_modified(ltrfams, TRUE);
    }
  }
  g_object_set(ltrfams->lv_fams_renderer, "editable", FALSE, NULL);
  gtk_tree_selection_set_mode(sel, GTK_SELECTION_MULTIPLE);
  g_free(old_name);
}

static void
gtk_ltr_families_lv_fams_cell_edit_canceled(GT_UNUSED GtkCellRenderer *renderer,
                                            GtkLTRFamilies *ltrfams)
{
  GtkTreeView *list_view;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreeSelection *sel;
  gchar *name;

  list_view = GTK_TREE_VIEW(ltrfams->lv_families);
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(list_view));
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_view));
  gtk_tree_selection_get_selected(sel, &model, &iter);
  gtk_tree_model_get(model, &iter,
                     LTRFAMS_FAM_LV_CURNAME, &name,
                     -1);

  if (!name)
    gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
  g_free(name);
  g_object_set(ltrfams->lv_fams_renderer, "editable", FALSE, NULL);
  gtk_tree_selection_set_mode(sel, GTK_SELECTION_MULTIPLE);
}

static void
gtk_ltr_families_lv_fams_cell_edit_started(GT_UNUSED GtkCellRenderer *renderer,
                                           GtkCellEditable *editable,
                                           gchar *path_str,
                                           gpointer list_view)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreePath *path = NULL;
  gchar *text = NULL;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(list_view));

  path = gtk_tree_path_new_from_string (path_str);
  gtk_tree_model_get_iter (model, &iter, path);
  gtk_tree_path_free (path);
  gtk_tree_model_get(model, &iter,
                     LTRFAMS_FAM_LV_OLDNAME, &text,
                     -1);
  if (GTK_IS_ENTRY(editable) && (text != NULL)) {
    GtkEntry *entry = GTK_ENTRY (editable);
    gtk_entry_set_text(entry, text);
  }
  g_free(text);
}

void
gtk_ltr_families_lv_fams_row_activated(GtkTreeView *tree_view,
                                       GtkTreePath *path,
                                       GT_UNUSED GtkTreeViewColumn *column,
                                       GtkLTRFamilies *ltrfams)
{
  GtArray *nodes;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkNotebook *notebook;
  GtkWidget *tab_label;
  gint nbpage;

  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(ltrfams->nb_family)) == 0)
    return;

  model = gtk_tree_view_get_model(tree_view);
  gtk_tree_model_get_iter(model, &iter, path);
  gtk_tree_model_get(model, &iter,
                     LTRFAMS_FAM_LV_TAB_LABEL, &tab_label,
                     LTRFAMS_FAM_LV_NODE_ARRAY, &nodes,
                     -1);
  notebook = GTK_NOTEBOOK(ltrfams->nb_family);

  if (tab_label) {
    nbpage =
       GPOINTER_TO_INT(gtk_label_close_get_button_data(GTKLABELCLOSE(tab_label),
                                                       "nbpage"));
    gtk_notebook_set_current_page(notebook, nbpage);
  } else {
    gtk_ltr_families_nb_fam_add_tab(model, &iter, nodes, FALSE, ltrfams);
  }
}

static void gtk_ltr_families_lv_fams_create(GtkLTRFamilies *ltrfams)
{
  GtkTreeViewColumn *column;
  GtkListStore *store;

  ltrfams->lv_fams_renderer = gtk_cell_renderer_text_new();
  g_signal_connect(G_OBJECT(ltrfams->lv_fams_renderer), "edited",
                   G_CALLBACK(gtk_ltr_families_lv_fams_cell_edited),
                   ltrfams);
  g_signal_connect(G_OBJECT(ltrfams->lv_fams_renderer), "editing-canceled",
                   G_CALLBACK(gtk_ltr_families_lv_fams_cell_edit_canceled),
                   ltrfams);
  g_signal_connect(G_OBJECT(ltrfams->lv_fams_renderer), "editing-started",
                   G_CALLBACK(gtk_ltr_families_lv_fams_cell_edit_started),
                   ltrfams->lv_families);
  g_signal_connect(G_OBJECT(ltrfams->lv_families), "button-press-event",
                   G_CALLBACK(gtk_ltr_families_tv_fams_button_pressed),
                   ltrfams);
  g_signal_connect(G_OBJECT(ltrfams->lv_families), "popup-menu",
                   G_CALLBACK(gtk_ltr_families_tv_fams_on_pmenu), ltrfams);
  g_signal_connect(G_OBJECT(ltrfams->lv_families), "row-activated",
                   G_CALLBACK(gtk_ltr_families_lv_fams_row_activated), ltrfams);
  column  = gtk_tree_view_column_new_with_attributes("LTR Families",
                                                     ltrfams->lv_fams_renderer,
                                                     "text",
                                                     LTRFAMS_FAM_LV_CURNAME,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id(column, LTRFAMS_FAM_LV_CURNAME);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrfams->lv_families), column);

  store = gtk_list_store_new(LTRFAMS_FAM_LV_N_COLUMS,
                             G_TYPE_POINTER,
                             G_TYPE_POINTER,
                             G_TYPE_POINTER,
                             G_TYPE_STRING,
                             G_TYPE_STRING,
                             G_TYPE_POINTER);

  gtk_tree_view_set_model(GTK_TREE_VIEW(ltrfams->lv_families),
                          GTK_TREE_MODEL(store));
  g_object_unref(store);

  /* Set <lv_families> as the destination of the Drag-N-Drop operation */
  gtk_drag_dest_set(ltrfams->lv_families,
                    GTK_DEST_DEFAULT_ALL,
                    family_drag_targets,
                    G_N_ELEMENTS(family_drag_targets),
                    GDK_ACTION_MOVE);
  g_signal_connect(G_OBJECT(ltrfams->lv_families), "drag-motion",
                   G_CALLBACK(on_drag_motion), NULL);
}
/* <lv_families> related functions end */

void gtk_ltr_families_determine_fl_cands(GtkLTRFamilies *ltrfams,
                                         gfloat ltrtolerance,
                                         gfloat lentolerance)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtArray *nodes;
  gboolean valid;
  unsigned long flcands;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfams->lv_families));
  valid = gtk_tree_model_get_iter_first(model, &iter);
  if (valid) {
    gtk_tree_model_get(model, &iter, LTRFAMS_FAM_LV_NODE_ARRAY, &nodes, -1);
    flcands = determine_full_length_candidates(nodes, ltrtolerance,
                                               lentolerance);
    while (gtk_tree_model_iter_next(model, &iter)) {
      gtk_tree_model_get(model, &iter, LTRFAMS_FAM_LV_NODE_ARRAY, &nodes, -1);
      flcands = determine_full_length_candidates(nodes, ltrtolerance,
                                                 lentolerance);
    }
  }
}

void gtk_ltr_families_fill_with_data(GtkLTRFamilies *ltrfams,
                                     GtArray *nodes,
                                     GtHashmap *features,
                                     unsigned long noc)
{
  gchar sb_text[BUFSIZ];
  gint had_err;

  ltrfams->nodes = nodes;
  ltrfams->features = features;
  ltrfams->colors = gt_hashmap_new(GT_HASH_STRING,
                                   free_str_hash, free_str_hash);
  ltrfams->n_features = noc;
  ltrfams->err = gt_error_new();
  ltrfams->style = gt_style_new(ltrfams->err);

  had_err = gt_style_load_file(ltrfams->style, DEFAULT_STYLE, ltrfams->err);
  if (had_err) {
    g_warning("%s", gt_error_get(ltrfams->err));
    gt_error_unset(ltrfams->err);
  }
  gtk_ltr_families_nb_fam_create(ltrfams);
  update_main_tab_label(ltrfams);
  gtk_widget_set_sensitive(GTK_WIDGET(ltrfams->new_fam), TRUE);
  gtk_widget_set_sensitive(GTK_WIDGET(ltrfams->tb_lv_families), TRUE);
  g_snprintf(sb_text, BUFSIZ, STATUSBAR_NUM_OF_CANDS,
             gt_array_size(ltrfams->nodes));
  statusbar_set_status(ltrfams->statusbar, sb_text);
}

static gboolean gtk_ltr_families_destroy(GtkWidget *widget,
                                         GT_UNUSED GdkEvent *event,
                                         GT_UNUSED gpointer user_data)
{
  GtkLTRFamilies *ltrfams;
  GtGenomeNode *gn;
  unsigned long i;
  ltrfams = GTK_LTR_FAMILIES(widget);

  gt_error_delete(ltrfams->err);
  gt_style_delete(ltrfams->style);
  gt_diagram_delete(ltrfams->diagram);
  gt_hashmap_delete(ltrfams->features);
  gt_hashmap_delete(ltrfams->colors);
  for (i = 0; i < gt_array_size(ltrfams->nodes); i++) {
    gn = *(GtGenomeNode**) gt_array_get(ltrfams->nodes, i);
    delete_gt_genome_node(gn);
  }
  gt_array_delete(ltrfams->nodes);
  g_free(ltrfams->projectfile);

  return FALSE;
}

gboolean gtk_ltr_families_lv_fams_key_pressed(GT_UNUSED GtkWidget *list_view,
                                              GdkEventKey *event,
                                              GtkLTRFamilies *ltrfams)
{
  switch (event->keyval) {
    case GDK_Delete:
      gtk_ltr_families_lv_fams_pmenu_remove_clicked(NULL, ltrfams);
      return TRUE;
      break;
    case GDK_F2:
      gtk_ltr_families_lv_fams_pmenu_edit_clicked(NULL,
                                               (gpointer) ltrfams->lv_families);
      return TRUE;
      break;
    default:
      break;
  }
  return FALSE;
}

static void gtk_ltr_families_init(GtkLTRFamilies *ltrfams)
{
  GtkWidget *vbox1,
            *vbox2,
            *vbox3,
            *label,
            *sw1,
            *sw2,
            *sw3,
            *eventb,
            *align1,
            *align2,
            *hsep1,
            *hsep2;

  GtkAdjustment *vadj = NULL;
  GtkToolItem *add,
              *remove;
  GdkColor color;
  GtkTreeSelection *selection;

  gdk_color_parse("white", &color);

  vbox1 = gtk_vbox_new(FALSE, 0);
  ltrfams->tb_lv_families = gtk_toolbar_new();
  gtk_widget_set_sensitive(GTK_WIDGET(ltrfams->tb_lv_families), FALSE);
  gtk_toolbar_set_show_arrow(GTK_TOOLBAR(ltrfams->tb_lv_families), TRUE);
  gtk_toolbar_set_style(GTK_TOOLBAR(ltrfams->tb_lv_families),
                        GTK_TOOLBAR_ICONS);
  add = gtk_tool_button_new_from_stock(GTK_STOCK_ADD);
  gtk_tool_item_set_tooltip_text(add, TB_FAMS_ADD);
  gtk_toolbar_insert(GTK_TOOLBAR(ltrfams->tb_lv_families), add, 0);
  remove = gtk_tool_button_new_from_stock(GTK_STOCK_CLEAR);
  gtk_tool_item_set_tooltip_text(remove, TB_FAMS_REMOVE);
  gtk_toolbar_insert(GTK_TOOLBAR(ltrfams->tb_lv_families), remove, 1);
  sw1 = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw1),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
  ltrfams->lv_families = gtk_tree_view_new();
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(ltrfams->lv_families));
  gtk_tree_selection_set_mode(selection, GTK_SELECTION_MULTIPLE);
  g_signal_connect(G_OBJECT(ltrfams->lv_families), "key-press-event",
                   G_CALLBACK(gtk_ltr_families_lv_fams_key_pressed), ltrfams);
  g_signal_connect(G_OBJECT(add), "clicked",
                   G_CALLBACK(gtk_ltr_families_lv_fams_tb_add_clicked),
                   ltrfams);
  g_signal_connect(G_OBJECT(remove), "clicked",
                   G_CALLBACK(gtk_ltr_families_lv_fams_tb_rm_clicked),
                   ltrfams);
  gtk_container_add(GTK_CONTAINER(sw1), ltrfams->lv_families);
  hsep1 = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(vbox1), ltrfams->tb_lv_families, FALSE, TRUE, 1);
  gtk_box_pack_start(GTK_BOX(vbox1), hsep1, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox1), sw1, TRUE, TRUE, 1);

  gtk_paned_add1(GTK_PANED(ltrfams), vbox1);

  ltrfams->vpaned = gtk_vpaned_new();
  vbox2 = gtk_vbox_new(FALSE, 0);
  ltrfams->tb_nb_family = gtk_toolbar_new();
  gtk_toolbar_set_show_arrow(GTK_TOOLBAR(ltrfams->tb_nb_family), TRUE);
  gtk_toolbar_set_style(GTK_TOOLBAR(ltrfams->tb_nb_family), GTK_TOOLBAR_ICONS);
  ltrfams->new_fam = gtk_tool_button_new_from_stock(GTK_STOCK_FIND);
  gtk_tool_item_set_tooltip_text(ltrfams->new_fam, TB_NB_NEW_FAM);
  gtk_toolbar_insert(GTK_TOOLBAR(ltrfams->tb_nb_family), ltrfams->new_fam, 0);
  g_signal_connect(G_OBJECT(ltrfams->new_fam), "clicked",
                   G_CALLBACK(gtk_ltr_families_nb_fam_tb_nf_clicked), ltrfams);
  ltrfams->fl_cands =
                     gtk_tool_button_new_from_stock(GTK_STOCK_FIND_AND_REPLACE);
  gtk_tool_item_set_tooltip_text(ltrfams->fl_cands, TB_NB_FL_CANDS);
  gtk_toolbar_insert(GTK_TOOLBAR(ltrfams->tb_nb_family), ltrfams->fl_cands, 1);
  g_signal_connect(G_OBJECT(ltrfams->fl_cands), "clicked",
                   G_CALLBACK(gtk_ltr_families_nb_fam_tb_fl_clicked), ltrfams);
  gtk_widget_set_sensitive(GTK_WIDGET(ltrfams->new_fam), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(ltrfams->fl_cands), FALSE);
  ltrfams->nb_family = gtk_notebook_new();
  hsep2 = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(vbox2), ltrfams->tb_nb_family, FALSE, TRUE, 1);
  gtk_box_pack_start(GTK_BOX(vbox2), hsep2, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox2), ltrfams->nb_family, TRUE, TRUE, 1);

  gtk_paned_add1(GTK_PANED(ltrfams->vpaned), vbox2);

  ltrfams->hpaned = gtk_hpaned_new();
  vbox3 = gtk_vbox_new(FALSE, 0);
  align1 = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  align2 = gtk_alignment_new(0.0, 0.5, 1.0, 1.0);
  eventb = gtk_event_box_new();
  label = gtk_label_new(LTRFAM_DETINFO);
  gtk_widget_modify_bg(eventb, GTK_STATE_NORMAL, &color);
  gtk_container_add(GTK_CONTAINER(align1), label);
  gtk_container_add(GTK_CONTAINER(eventb), align1);
  gtk_container_add(GTK_CONTAINER(align2), eventb);
  sw2 = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw2),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
  ltrfams->tv_details = gtk_tree_view_new();
  gtk_container_add(GTK_CONTAINER(sw2), ltrfams->tv_details);
  gtk_box_pack_start(GTK_BOX(vbox3), align2, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox3), sw2, TRUE, TRUE, 0);

  gtk_paned_add1(GTK_PANED(ltrfams->hpaned), vbox3);

  sw3 = gtk_scrolled_window_new(NULL, vadj);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw3),
                                 GTK_POLICY_NEVER,
                                 GTK_POLICY_AUTOMATIC);
  ltrfams->image_area = gtk_layout_new(NULL, vadj);
  gtk_widget_modify_bg(ltrfams->image_area, GTK_STATE_NORMAL, &color);
  g_signal_connect(ltrfams->image_area, "expose-event",
                   G_CALLBACK(image_area_expose_event), ltrfams);
  gtk_layout_set_size(GTK_LAYOUT(ltrfams->image_area), 100, 100);
  gtk_container_add(GTK_CONTAINER(sw3), ltrfams->image_area);

  gtk_paned_add2(GTK_PANED(ltrfams->hpaned), sw3);

  gtk_paned_add2(GTK_PANED(ltrfams->vpaned), ltrfams->hpaned);

  gtk_paned_add2(GTK_PANED(ltrfams), ltrfams->vpaned);

  gtk_paned_set_position(GTK_PANED(ltrfams->hpaned), 400);
  gtk_paned_set_position(GTK_PANED(ltrfams->vpaned), 400);
  gtk_paned_set_position(GTK_PANED(ltrfams), 200);

  gtk_widget_show_all(GTK_WIDGET(ltrfams));
}

GType gtk_ltr_families_get_type(void)
{
  static GType ltrfams_type = 0;

  if (!ltrfams_type) {
    const GTypeInfo ltrfams_info =
    {
      sizeof (GtkLTRFamiliesClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      NULL, /* (GClassInitFunc) gtk_ltr_family_class_init, */
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (GtkLTRFamilies),
      0,    /* n_preallocs */
      (GInstanceInitFunc) gtk_ltr_families_init,
    };
    ltrfams_type = g_type_register_static(GTK_TYPE_HPANED, "GtkLTRFamilies",
                                         &ltrfams_info, 0);
  }
  return ltrfams_type;
}

GtkWidget* gtk_ltr_families_new(GtkWidget *statusbar,
                                GtkWidget *progressbar,
                                GtkWidget *projset)
{
  GtkLTRFamilies *ltrfams;
  ltrfams = gtk_type_new(GTK_LTR_FAMILIES_TYPE);
  gtk_ltr_families_lv_fams_create(ltrfams);
  gtk_ltr_families_tv_det_create(GTK_TREE_VIEW(ltrfams->tv_details));
  g_signal_connect(G_OBJECT(ltrfams->tv_details), "cursor-changed",
                   G_CALLBACK(gtk_ltr_families_tv_det_changed), ltrfams);
  g_signal_connect(G_OBJECT(ltrfams), "destroy",
                   G_CALLBACK(gtk_ltr_families_destroy), NULL);
  ltrfams->projectfile = NULL;
  ltrfams->unclassified_cands = 0;
  ltrfams->modified = FALSE;
  ltrfams->statusbar = statusbar;
  ltrfams->progressbar = progressbar;
  ltrfams->projset = projset;

  return GTK_WIDGET(ltrfams);
}
