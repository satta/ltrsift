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
#include "gtk_ltr_families.h"

/* "support" functions start */
static void remove_node_from_array(GtArray *nodes, GtGenomeNode **gn)
{
  GtGenomeNode **tmp;
  unsigned long i;

  for (i = 0; i < gt_array_size(nodes); i++) {
    tmp = *(GtGenomeNode***) gt_array_get(nodes, i);
    if (gn == tmp) {
      gt_array_rem(nodes, i);
      return;
    }
  }
}

static char* double_underscores(const char *str)
{
  char **arr;
  char *ret;

  arr = g_strsplit(str, "_", 0);
  ret = g_strjoinv("__", arr);
  g_strfreev(arr);

  return ret;
}

static void free_iter_hash(void *elem)
{
  gtk_tree_iter_free((GtkTreeIter*) elem);
}

static void gtk_ltr_families_clear_lv_det_on_equal_nodes(GtkTreeView *tree_view,
                                                         GtGenomeNode **gn)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtGenomeNode **gn_tmp;

  model = gtk_tree_view_get_model(tree_view);
  if (model) {
    gboolean valid = gtk_tree_model_get_iter_first(model, &iter);
    if (!valid)
      return;
    gtk_tree_model_get(model, &iter,
                       LTRFAMS_DETAIL_TV_NODE, &gn_tmp,
                       -1);
    if (gn_tmp == gn) {
      gtk_tree_store_clear(GTK_TREE_STORE(model));
    }
  }
}
/* "support" functions end */

/* <image_area> related functions start */
static void draw_image(GtkLTRFamilies *ltrfams, GtGenomeNode **gn)
{
  GtkWidget *a = GTK_WIDGET(ltrfams->image_area);
  GtFeatureIndex *features;
  GtRange range;
  GtError *err;
  const char *seqid;

  err = gt_error_new();
  ltrfams->style = gt_style_new(err);
  gt_style_load_file(ltrfams->style,
   "/local/skastens/masterarbeit/ltrgui/tmp/ltr_colors_pdoms_uncollapsed.style",
                     err);
  features = gt_feature_index_memory_new();
  gt_feature_index_add_feature_node(features, (GtFeatureNode*) gn);
  seqid = gt_feature_index_get_first_seqid(features);
  gt_feature_index_get_range_for_seqid(features, &range, seqid);
  gt_diagram_delete(ltrfams->diagram);
  ltrfams->diagram = gt_diagram_new(features, seqid, &range,
                                    ltrfams->style, err);
  gtk_widget_queue_draw_area(a, 0, 0, a->allocation.width,
                             a->allocation.height);
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
  FamilyTransferData *tdata;

  /* retrieve data from selected row, do nothing if no row is selected */
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
  if (!gtk_tree_selection_get_selected(sel, &model, &iter))
    return;
  tdata = g_slice_new(FamilyTransferData);
  gtk_tree_model_get(model, &iter,
                     LTRFAMS_LV_NODE, &tdata->gn,
                     LTRFAMS_LV_FEAT, &tdata->feat,
                     -1);
  /* add retrieved data to GtkSelectionData object */
  gtk_selection_data_set(sdata,
                         gdk_atom_intern("FamilyTransferData pointer", FALSE),
                         8,
                         (void*) &tdata,
                         sizeof(tdata));
}

static gboolean on_drag_motion(GtkWidget *widget,
                               GT_UNUSED GdkDragContext *context,
                               gint x,
                               gint y,
                               GT_UNUSED guint time,
                               GT_UNUSED gpointer user_data)
{
  GtkTreeSelection *selection;
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
  if (gtk_tree_selection_count_selected_rows(selection)  <= 1) {
    GtkTreePath *path;
    /* in case mouse cursor points at a child select its parent
       as drag destination */
    if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(widget),
                                      x,
                                      y,
                                      &path, NULL, NULL, NULL)) {
      GtkTreeIter iter, parent;
      GtkTreeModel *model;
      model = gtk_tree_view_get_model(GTK_TREE_VIEW(widget));
      gtk_tree_model_get_iter(model, &iter, path);
      if (gtk_tree_model_iter_parent(model, &parent, &iter))
        path = gtk_tree_model_get_path(model, &parent);
      gtk_tree_selection_unselect_all(selection);
      gtk_tree_selection_select_path(selection, path);
      gtk_tree_path_free(path);
    } else
      gtk_tree_selection_unselect_all(selection);
  }
  return TRUE;
}

static void on_drag_data_received(GtkWidget *widget,
                                  GT_UNUSED GdkDragContext *drag_context,
                                  GT_UNUSED gint x,
                                  GT_UNUSED gint y,
                                  GtkSelectionData *sdata,
                                  GT_UNUSED guint info,
                                  GT_UNUSED guint time,
                                  gpointer notebook)
{
  GtkTreeModel *model,
               *model2;
  GtkTreeIter iter,
              iter2;
  GtkTreeSelection *sel,
                   *sel2;
  GtkTreeRowReference *tv_ref;
  GtkWidget *tab_child,
            *tmp_tab_child;
  FamilyTransferData *tdata = NULL;
  GtGenomeNode **gn;
  GtArray *nodes;
  GList *children;
  gint nbpage;
  unsigned long i;
  const char *oldname;
  char curname[BUFSIZ];

  /* add <gn> to selected family, do nothing if no family is selected
     <widget> == <tv_families> */
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
  if (!gtk_tree_selection_get_selected(sel, &model, &iter))
    return;

  /* copy recieved data into new struct */
  memcpy(&tdata, sdata->data, sizeof(tdata));

  /* insert data into <tv_families> */
  gtk_tree_model_get(model, &iter,
                     LTRFAMS_FAM_LV_NODE_ARRAY, &nodes,
                     LTRFAMS_FAM_LV_TAB_CHILD, &tab_child,
                     LTRFAMS_FAM_LV_OLDNAME, &oldname,
                     -1);
  for (i = 0; i < gt_array_size(nodes); i++) {
    gn = *(GtGenomeNode***) gt_array_get(nodes, i);
    if (gn == tdata->gn) {
      /* drag source and destination are the same -> do nothing */
      return;
    }
  }

  /* remove row from drag source */
  nbpage = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
  tmp_tab_child = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), nbpage);
  children = gtk_container_get_children(GTK_CONTAINER(tmp_tab_child));
  sel2 = gtk_tree_view_get_selection(
                                   GTK_TREE_VIEW(g_list_first(children)->data));
  gtk_tree_selection_get_selected(sel2, &model2, &iter2);
  gtk_tree_model_get(model2, &iter2,
                     LTRFAMS_LV_ROWREF, &tv_ref,
                     -1);

  /* tv_ref points to family in <tv_families>. If <gn> was drag'n'dropped from
     one family to another, remove <gn> from node array of old family */
  if (tv_ref) {
    GtkTreePath *tv_path;
    GtkTreeIter tv_iter;
    GtArray *tmp_nodes;
    const char *tmp_oldname;
    char tmp_curname[BUFSIZ];
    tv_path = gtk_tree_row_reference_get_path(tv_ref);
    gtk_tree_model_get_iter(model, &tv_iter, tv_path);
    gtk_tree_model_get(model, &tv_iter,
                       LTRFAMS_FAM_LV_NODE_ARRAY, &tmp_nodes,
                       LTRFAMS_FAM_LV_OLDNAME, &tmp_oldname,
                       -1);
    remove_node_from_array(tmp_nodes, tdata->gn);
    g_snprintf(tmp_curname, BUFSIZ, "%s (%lu)",
               tmp_oldname, gt_array_size(tmp_nodes));
    gtk_tree_store_set(GTK_TREE_STORE(model), &tv_iter,
                       LTRFAMS_FAM_LV_CURNAME, tmp_curname,
                       -1);
    gtk_tree_path_free(tv_path);
  }
  gtk_list_store_remove(GTK_LIST_STORE(model2), &iter2);
  /* clear detailed view if shown <gn> was drag'n'dropped to another family */
  gtk_ltr_families_clear_lv_det_on_equal_nodes(
                                    GTK_TREE_VIEW(g_list_first(children)->data),
                                               tdata->gn);

  /* add node to "family node array" and insert node into list_view from
     assigned tab (if it exists) */
  gt_array_add(nodes, tdata->gn);
  g_snprintf(curname, BUFSIZ, "%s (%lu)", oldname, gt_array_size(nodes));
  gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
                     LTRFAMS_FAM_LV_CURNAME, curname,
                     -1);
  if (tab_child) {
    GtkTreePath *path;
    GtkTreeRowReference  *tv_ref2;
    path = gtk_tree_model_get_path(model, &iter);
    tv_ref2 = gtk_tree_row_reference_new(model, path);
    gtk_ltr_families_nb_fam_lv_append(GTK_TREE_VIEW(tab_child), tdata->gn,
                                    tdata->feat, tv_ref2, NULL);
    gtk_tree_path_free(path);
  }
  g_list_free(children);
}
/* drag'n'drop related functions end */

/* popupmenu related functions start*/
static void gtk_ltr_families_nb_fam_lv_pmenu_remove_clicked(
                                                  GT_UNUSED GtkWidget *menuitem,
                                                  GtkLTRFamilies *ltrfams)
{
  /* TODO: check for main_tab -> modify warning */
  GtkWidget *dialog,
            *main_tab,
            *tab_child;
  GtkTreeView *treeview;
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  GtkTreeIter iter;
  GList *children;
  gint main_tab_no,
      tab_no;

  tab_no = gtk_notebook_get_current_page(GTK_NOTEBOOK(ltrfams->nb_family));
  tab_child = gtk_notebook_get_nth_page(GTK_NOTEBOOK(ltrfams->nb_family),
                                        tab_no);
  children = gtk_container_get_children(GTK_CONTAINER(tab_child));
  treeview = GTK_TREE_VIEW(g_list_first(children)->data);
  model = gtk_tree_view_get_model(treeview);
  sel = gtk_tree_view_get_selection(treeview);
  if (gtk_tree_selection_count_selected_rows(sel) == 0)
    return;
  gtk_tree_selection_get_selected(sel, &model, &iter);

  main_tab_no =
      GPOINTER_TO_INT(
             g_object_get_data(G_OBJECT(ltrfams->nb_family), "main_tab"));

  dialog = gtk_message_dialog_new(NULL,
                          GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
                                  "Are you sure?");
  gtk_window_set_title(GTK_WINDOW(dialog), "Attention!");
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_NO) {
    gtk_widget_destroy(dialog);
    return;
  } else {
    GtGenomeNode **gn;
    GtkTreeRowReference *rowref;
    GList *tmp_children;
    gtk_tree_model_get(model, &iter,
                       LTRFAMS_LV_NODE, &gn,
                       LTRFAMS_LV_ROWREF, &rowref,
                       -1);
    main_tab =
       gtk_notebook_get_nth_page(GTK_NOTEBOOK(ltrfams->nb_family),
                                 main_tab_no);
    tmp_children = gtk_container_get_children(GTK_CONTAINER(main_tab));
    gtk_ltr_families_nb_fam_lv_append(
                                GTK_TREE_VIEW(g_list_first(tmp_children)->data),
                                      gn,
                                      ltrfams->features, NULL, NULL);
    if (rowref) {
      GtkTreeModel *model2;
      GtkTreePath *tv_path;
      GtkTreeIter tv_iter;
      GtArray *tmp_nodes;
      const char *tmp_oldname;
      char tmp_curname[BUFSIZ];

      model2 = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfams->lv_families));
      tv_path = gtk_tree_row_reference_get_path(rowref);
      gtk_tree_model_get_iter(model2, &tv_iter, tv_path);
      gtk_tree_model_get(model2, &tv_iter,
                         LTRFAMS_FAM_LV_NODE_ARRAY, &tmp_nodes,
                         LTRFAMS_FAM_LV_OLDNAME, &tmp_oldname,
                         -1);

      remove_node_from_array(tmp_nodes, gn);
      g_snprintf(tmp_curname, BUFSIZ, "%s (%lu)",
                 tmp_oldname, gt_array_size(tmp_nodes));
      gtk_tree_store_set(GTK_TREE_STORE(model2), &tv_iter,
                         LTRFAMS_FAM_LV_CURNAME, tmp_curname,
                         -1);
      gtk_tree_path_free(tv_path);
    }
    gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
    g_list_free(tmp_children);
  }
  g_list_free(children);
  gtk_widget_destroy(dialog);
}

static void gtk_ltr_families_nb_fam_lv_pmenu(GT_UNUSED GtkWidget *treeview,
                                             GdkEventButton *event,
                                             GtkLTRFamilies *ltrfams)
{
  GtkWidget *menu, *menuitem;

  menu = gtk_menu_new();

  menuitem = gtk_menu_item_new_with_label("Remove selected");
  g_signal_connect(menuitem, "activate",
                   (GCallback) gtk_ltr_families_nb_fam_lv_pmenu_remove_clicked,
                   ltrfams);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  gtk_widget_show_all(menu);
  gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,
                 (event != NULL) ? event->button : 0,
                 gdk_event_get_time((GdkEvent*)event));
}

static gboolean gtk_ltr_families_nb_fam_lv_button_pressed(GtkWidget *treeview,
                                                          GdkEventButton *event,
                                                        GtkLTRFamilies *ltrfams)
{
  if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3)
  {
    GtkTreeSelection *selection;
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    if (gtk_tree_selection_count_selected_rows(selection)  <= 1)
    {
      GtkTreePath *path;
      if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview),
                                        (gint) event->x,
                                        (gint) event->y,
                                        &path, NULL, NULL, NULL)) {
        gtk_tree_selection_unselect_all(selection);
        gtk_tree_selection_select_path(selection, path);
        gtk_tree_path_free(path);
      } else
        return TRUE;
    }
    gtk_ltr_families_nb_fam_lv_pmenu(treeview, event, ltrfams);
    return TRUE;
  }
  return FALSE;
}

static gboolean gtk_ltr_families_nb_fam_lv_on_pmenu(GtkWidget *treeview,
                                                    GtkLTRFamilies *ltrfams)
{
  gtk_ltr_families_nb_fam_lv_pmenu(treeview, NULL, ltrfams);
  return TRUE;
}



static void gtk_ltr_families_tv_fams_pmenu_edit_clicked(
                                                  GT_UNUSED GtkWidget *menuitem,
                                                        gpointer userdata)
{
  GtkTreeView *treeview;
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  GtkTreeViewColumn *column;
  GtkTreeIter iter;
  GtkTreePath *path;
  GtkCellRenderer *renderer;
  GList *tmp;

  treeview = GTK_TREE_VIEW(userdata);
  model = gtk_tree_view_get_model(treeview);
  sel = gtk_tree_view_get_selection(treeview);
  if (gtk_tree_selection_count_selected_rows(sel) == 0)
    return;
  column = gtk_tree_view_get_column(treeview, 0);
  tmp = gtk_cell_layout_get_cells(GTK_CELL_LAYOUT(column));
  renderer = (GtkCellRenderer*) g_list_nth_data(tmp, 0);
  g_object_set(renderer, "editable", TRUE, NULL);
  gtk_tree_selection_get_selected(sel, &model, &iter);
  path = gtk_tree_model_get_path(model, &iter);
  gtk_tree_view_set_cursor(treeview, path, column, TRUE);
}

static void gtk_ltr_families_tv_fams_pmenu_remove_clicked(
                                                  GT_UNUSED GtkWidget *menuitem,
                                                  GtkLTRFamilies *ltrfams)
{
  GtkWidget *dialog,
            *main_tab,
            *tab_label;
  GtkTreeView *treeview;
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  GtkTreeIter iter;
  gint main_tab_no;
  unsigned long i;

  treeview = GTK_TREE_VIEW(ltrfams->lv_families);
  model = gtk_tree_view_get_model(treeview);
  sel = gtk_tree_view_get_selection(treeview);
  if (gtk_tree_selection_count_selected_rows(sel) == 0)
    return;
  gtk_tree_selection_get_selected(sel, &model, &iter);

  main_tab_no =
      GPOINTER_TO_INT(
             g_object_get_data(G_OBJECT(ltrfams->nb_family), "main_tab"));

  dialog = gtk_message_dialog_new(NULL,
                          GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
                                  "Are you sure?");
  gtk_window_set_title(GTK_WINDOW(dialog), "Attention!");
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_NO) {
    gtk_widget_destroy(dialog);
    return;
  } else {
    GtArray *nodes;
    GList *children;
    GtkTreeView *list_view;
    gtk_tree_model_get(model, &iter,
                       LTRFAMS_FAM_LV_NODE_ARRAY, &nodes,
                       -1);
    main_tab =
       gtk_notebook_get_nth_page(GTK_NOTEBOOK(ltrfams->nb_family),
                                 main_tab_no);
    children = gtk_container_get_children(GTK_CONTAINER(main_tab));
    list_view = GTK_TREE_VIEW(g_list_first(children)->data);
    for (i = 0; i < gt_array_size(nodes); i++) {
      GtGenomeNode **tmp_gn;
      tmp_gn = *(GtGenomeNode***) gt_array_get(nodes, i);
      gtk_ltr_families_nb_fam_lv_append(list_view, tmp_gn,
                                        ltrfams->features, NULL, NULL);
    }

    gtk_tree_model_get(model, &iter, LTRFAMS_FAM_LV_TAB_LABEL, &tab_label, -1);
    if (tab_label) {
      gtk_notebook_remove_page(GTK_NOTEBOOK(ltrfams->nb_family),
                  GPOINTER_TO_INT(
                   gtk_label_close_get_button_data(GTKLABELCLOSE(tab_label),
                                                   "nbpage")));
      gtk_ltr_families_nb_fam_refresh_nums(GTK_NOTEBOOK(ltrfams->nb_family));
    }
    gtk_tree_store_remove(GTK_TREE_STORE(model), &iter);
    g_list_free(children);
  }
  gtk_widget_destroy(dialog);
}

static void gtk_ltr_families_tv_fams_pmenu(GtkWidget *treeview,
                                           GdkEventButton *event,
                                           GtkLTRFamilies *ltrfams)
{
  GtkWidget *menu, *menuitem;

  menu = gtk_menu_new();
  menuitem = gtk_menu_item_new_with_label("Edit name");
  g_signal_connect(menuitem, "activate",
                   (GCallback) gtk_ltr_families_tv_fams_pmenu_edit_clicked,
                   treeview);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  menuitem = gtk_menu_item_new_with_label("Remove selected");
  g_signal_connect(menuitem, "activate",
                   (GCallback) gtk_ltr_families_tv_fams_pmenu_remove_clicked,
                   ltrfams);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  gtk_widget_show_all(menu);
  gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,
                 (event != NULL) ? event->button : 0,
                 gdk_event_get_time((GdkEvent*)event));
}

static gboolean gtk_ltr_families_tv_fams_button_pressed(GtkWidget *treeview,
                                                        GdkEventButton *event,
                                                        GtkLTRFamilies *ltrfams)
{
  if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3)
  {
    GtkTreeSelection *selection;
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    if (gtk_tree_selection_count_selected_rows(selection)  <= 1)
    {
      GtkTreePath *path;
      if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview),
                                        (gint) event->x,
                                        (gint) event->y,
                                        &path, NULL, NULL, NULL)) {
        gtk_tree_selection_unselect_all(selection);
        gtk_tree_selection_select_path(selection, path);
        gtk_tree_path_free(path);
      } else
        return TRUE;
    }
    gtk_ltr_families_tv_fams_pmenu(treeview, event, ltrfams);
    return TRUE;
  }
  return FALSE;
}

static gboolean gtk_ltr_families_tv_fams_on_pmenu(GtkWidget *treeview,
                                                  GtkLTRFamilies *ltrfams)
{
  gtk_ltr_families_tv_fams_pmenu(treeview, NULL, ltrfams);
  return TRUE;
}
/* popupmenu related functions end*/

/* <nb_family> related functions start */
static int add_feature_columns(void *key, void *value, void *lv,
                               GT_UNUSED GtError *err)
{
  GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
  GtkTreeViewColumn *column;
  const char *caption = double_underscores((const char*) key);
  unsigned long num = (unsigned long) value;
  GtkTreeView *list_view = (GtkTreeView*) lv;

  g_object_set(renderer,
               "cell-background", "Grey",
               "cell-background-set", TRUE,
               "xalign", 1.0,
               NULL);

  column = gtk_tree_view_column_new_with_attributes(caption, renderer, "text",
                                                    num, NULL);
  gtk_tree_view_column_set_resizable(column, true);
  gtk_tree_view_column_set_sort_column_id(column, num);
  gtk_tree_view_append_column(list_view, column);

  return 0;
}

static void gtk_ltr_families_nb_fam_lv_changed(GtkTreeView *list_view,
                                               GtkLTRFamilies *ltrfams)
{
  GtkTreeModel *list_model, *tree_model;
  GtkTreeIter iter, tmp;
  GtkTreeStore *store;
  GtkTreeSelection *selection;
  GtGenomeNode **gn;
  GtFeatureNodeIterator *fni;
  GtFeatureNode *curnode;
  GtRange range;
  GtHashmap *iter_hash;
  const char *fnt;

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_view));
  if (gtk_tree_selection_count_selected_rows(selection) < 1)
    return;
  list_model = gtk_tree_view_get_model(list_view);
  gtk_tree_selection_get_selected(selection, &list_model, &tmp);
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
    types[4] = G_TYPE_STRING;

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
                     (void*) gt_feature_node_get_attribute(curnode, "ID"),
                     (void*) gtk_tree_iter_copy(&iter));
    } else {
      GtkTreeIter *tmp_iter;
      tmp_iter = (GtkTreeIter*) gt_hashmap_get(iter_hash,
                      (void*) gt_feature_node_get_attribute(curnode, "Parent"));
      if (tmp_iter) {
        GtkTreeIter child;
        const char *id;
        range = gt_genome_node_get_range((GtGenomeNode*) curnode);
        gtk_tree_store_append(store, &child, tmp_iter);
        gtk_tree_store_set(store, &child,
                           LTRFAMS_DETAIL_TV_TYPE, fnt,
                           LTRFAMS_DETAIL_TV_START, range.start,
                           LTRFAMS_DETAIL_TV_END, range.end,
                           -1);
        if ((id = gt_feature_node_get_attribute(curnode, "ID")))
          gt_hashmap_add(iter_hash, (void*) id,
                         (void*) gtk_tree_iter_copy(&child));
      }
    }
  }
  gtk_tree_view_expand_all(GTK_TREE_VIEW(ltrfams->tv_details));
  draw_image(ltrfams, gn);
  gt_feature_node_iterator_delete(fni);
  gt_hashmap_delete(iter_hash);
}

static void gtk_ltr_families_nb_fam_lv_append(GtkTreeView *list_view,
                                              GtGenomeNode **gn,
                                              GtHashmap *features,
                                              GtkTreeRowReference *rowref,
                                              GtkListStore *tmp)
{
  GtFeatureNodeIterator *fni;
  GtFeatureNode *curnode;
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkListStore *store;
  const char *fnt;
  gboolean first_ltr = TRUE;

  model = gtk_tree_view_get_model(list_view);

  if (!tmp)
    store =
      GTK_LIST_STORE(model);
  else
    store = tmp;

  fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);

  while ((curnode = gt_feature_node_iterator_next(fni))) {
    fnt = gt_feature_node_get_type(curnode);
    if ((g_strcmp0(fnt, FNT_REPEATR) == 0)) {
      GtRange range;
      char *seqid;
      seqid = gt_str_get(gt_genome_node_get_seqid((GtGenomeNode*) curnode));
      range = gt_genome_node_get_range((GtGenomeNode*) curnode);
      GtStrand strand = gt_feature_node_get_strand(curnode);
      char c[2];
      g_snprintf(c, 2, "%c", GT_STRAND_CHARS[strand]);
      gtk_list_store_append(store, &iter);
      gtk_list_store_set(store, &iter,
                         LTRFAMS_LV_NODE, gn,
                         LTRFAMS_LV_FEAT, features,
                         LTRFAMS_LV_ROWREF, rowref,
                         LTRFAMS_LV_SEQID, seqid,
                         LTRFAMS_LV_TYPE, c,
                         LTRFAMS_LV_START, range.start,
                         LTRFAMS_LV_END, range.end,
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
        first_ltr = FALSE;
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

  return;
}

static void gtk_ltr_families_nb_fam_create_lv(GtkTreeView *list_view,
                                              GtArray *nodes,
                                              GtHashmap *features,
                                              unsigned long noc,
                                              GtkLTRFamilies *ltrfams)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkListStore *store;
  GType *types;
  GtError *err = NULL;
  unsigned long i;

  types = g_new0(GType, noc);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(LTRFAMS_LV_CAPTION_SEQID,
                                                    renderer, "text",
                                                    LTRFAMS_LV_SEQID, NULL);
  gtk_tree_view_column_set_sort_column_id(column, LTRFAMS_LV_SEQID);
  gtk_tree_view_append_column(list_view, column);

  renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "xalign", 1.0, NULL);
  column = gtk_tree_view_column_new_with_attributes(LTRFAMS_LV_CAPTION_TYPE,
                                                    renderer, "text",
                                                    LTRFAMS_LV_TYPE, NULL);
  gtk_tree_view_column_set_sort_column_id(column, LTRFAMS_LV_TYPE);
  gtk_tree_view_append_column(list_view, column);

  renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "xalign", 1.0, NULL);
  column = gtk_tree_view_column_new_with_attributes(LTRFAMS_LV_CAPTION_START,
                                                    renderer, "text",
                                                    LTRFAMS_LV_START, NULL);
  gtk_tree_view_column_set_sort_column_id(column, LTRFAMS_LV_START);
  gtk_tree_view_append_column(list_view, column);

  renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "xalign", 1.0, NULL);
  column = gtk_tree_view_column_new_with_attributes(LTRFAMS_LV_CAPTION_END,
                                                    renderer, "text",
                                                    LTRFAMS_LV_END, NULL);
  gtk_tree_view_column_set_sort_column_id(column, LTRFAMS_LV_END);
  gtk_tree_view_append_column(list_view, column);

  gt_hashmap_foreach(features, add_feature_columns, (void*) list_view, err);

  column = gtk_tree_view_column_new();
  gtk_tree_view_append_column(list_view, column);

  types[0] = G_TYPE_POINTER;
  types[1] = G_TYPE_POINTER;
  types[2] = G_TYPE_POINTER;
  types[3] = G_TYPE_STRING;
  types[4] = G_TYPE_STRING;
  types[5] = G_TYPE_ULONG;
  types[6] = G_TYPE_ULONG;

  for (i = LTRFAMS_LV_N_COLUMS; i < noc; i++) {
    types[i] = G_TYPE_STRING;
  }

  store = gtk_list_store_newv(noc, types);

  if (nodes) {
    for (i = 0; i < gt_array_size(nodes); i++) {
      GtGenomeNode **gn;
      gn = *(GtGenomeNode***) gt_array_get(nodes, i);
      gtk_ltr_families_nb_fam_lv_append(list_view, gn, features, NULL, store);
    }
  }

  gtk_tree_view_set_model(list_view,
                          GTK_TREE_MODEL(store));
  g_object_unref(store);
  g_signal_connect(G_OBJECT(list_view), "cursor-changed",
                   G_CALLBACK(gtk_ltr_families_nb_fam_lv_changed), ltrfams);
  g_free(types);
}

static void gtk_ltr_families_nb_fam_refresh_nums(GtkNotebook *notebook)
{
  GtkWidget *tab_child,
            *tab_label;
  gint n_pages,
       i,
       old_pagenum;

  n_pages = gtk_notebook_get_n_pages(notebook);
  for (i = 0; i < n_pages; i++) {
    tab_child = gtk_notebook_get_nth_page(notebook, i);
    tab_label = gtk_notebook_get_tab_label(notebook, tab_child);
    old_pagenum =
        GPOINTER_TO_INT(
           gtk_label_close_get_button_data(GTKLABELCLOSE(tab_label), "nbpage"));
    if (old_pagenum != i) {
      gtk_label_close_set_button_data(GTKLABELCLOSE(tab_label),
                                      "nbpage",
                                      GINT_TO_POINTER(i));
      if (g_strcmp0(gtk_label_close_get_text(GTKLABELCLOSE(tab_label)),
                    "General") == 0) {
        g_object_set_data(G_OBJECT(notebook),
                          "main_tab",
                          GINT_TO_POINTER(i));
      }
    }
  }
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
    gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
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
      if (nbpage == tmp)
        gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
                           LTRFAMS_FAM_LV_TAB_CHILD, NULL,
                           LTRFAMS_FAM_LV_TAB_LABEL, NULL,
                           -1);
    }
  }
  gtk_notebook_remove_page(GTK_NOTEBOOK(ltrfams->nb_family), nbpage);
  gtk_ltr_families_nb_fam_refresh_nums(GTK_NOTEBOOK(ltrfams->nb_family));
}

static void gtk_ltr_families_nb_fam_add_tab(GtkTreeModel *model,
                                            GtkTreeIter *iter,
                                            GtArray *nodes,
                                            GtkLTRFamilies *ltrfams)
{
  GtGenomeNode **gn;
  GtkTreeRowReference *rowref;
  GtkTreePath *path;
  GtkWidget *child,
            *label,
            *list_view;
  gint nbpage;
  unsigned long i;
  gchar *name;

  gtk_tree_model_get(model, iter,
                     LTRFAMS_FAM_LV_OLDNAME, &name,
                     -1);
  child = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(child),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);

  list_view = gtk_tree_view_new();
  gtk_ltr_families_nb_fam_create_lv(GTK_TREE_VIEW(list_view), NULL,
                                             ltrfams->features,
                                             ltrfams->n_features, ltrfams);
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
    gn = *(GtGenomeNode***) gt_array_get(nodes, i);
    gtk_ltr_families_nb_fam_lv_append(GTK_TREE_VIEW(list_view),
                                      gn,
                                      ltrfams->features,
                                      rowref,
                                      NULL);
  }
  gtk_tree_path_free(path);

  /* Set <child->list_view> as the source of the Drag-N-Drop operation */
  gtk_drag_source_set(list_view,
                      GDK_BUTTON1_MASK, family_drag_targets,
                      G_N_ELEMENTS(family_drag_targets),
                      GDK_ACTION_MOVE);

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
  gtk_tree_store_set(GTK_TREE_STORE(model), iter,
                     LTRFAMS_FAM_LV_TAB_CHILD, list_view,
                     LTRFAMS_FAM_LV_TAB_LABEL, label,
                     -1);
  gtk_notebook_set_current_page(GTK_NOTEBOOK(ltrfams->nb_family), nbpage);
  g_free(name);
}

static void gtk_ltr_families_nb_fam_page_reordered(GtkNotebook *notebook,
                                GT_UNUSED GtkWidget *child,
                                GT_UNUSED guint page_num,
                                GT_UNUSED gpointer user_data)
{
  gtk_ltr_families_nb_fam_refresh_nums(notebook);
}

static void gtk_ltr_families_nb_fam_create(GtkLTRFamilies *ltrfams)
{
  GtkWidget *child,
            *label,
            *list_view;
  gint nbpage;

  gtk_notebook_set_scrollable(GTK_NOTEBOOK(ltrfams->nb_family), TRUE);

  g_signal_connect(G_OBJECT(ltrfams->nb_family), "page-reordered",
                   G_CALLBACK(gtk_ltr_families_nb_fam_page_reordered), NULL);

  child = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(child),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
  list_view = gtk_tree_view_new();
  gtk_ltr_families_nb_fam_create_lv(GTK_TREE_VIEW(list_view), ltrfams->nodes,
                                    ltrfams->features, ltrfams->n_features,
                                    ltrfams);
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

  label = gtk_label_close_new("General", NULL, NULL);
  gtk_label_close_hide_close(GTKLABELCLOSE(label));
  gtk_widget_show(label);

  /* Set <child->list_view> as the source of the Drag-N-Drop operation */

  /* Attach a "drag-data-received" signal to pull in the dragged data */
  g_signal_connect(G_OBJECT(ltrfams->lv_families), "drag-data-received",
                   G_CALLBACK(on_drag_data_received),
                   ltrfams->nb_family);

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
}
/* <nb_family> related functions end */

/* <tv_details> related functions start */
static void gtk_ltr_families_tv_det_create(GtkTreeView *tree_view)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(LTRFAMS_LV_CAPTION_TYPE,
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

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(LTRFAMS_TV_CAPTION_INFO,
                                                    renderer, "text",
                                                    LTRFAMS_DETAIL_TV_INFO,
                                                    NULL);
  gtk_tree_view_append_column(tree_view, column);
  gtk_widget_show_all(GTK_WIDGET(tree_view));
}
/* <tv_details> related functions end */

/* <lv_families> related functions start */
static gboolean gtk_ltr_families_lv_fams_check_if_name_exists(
                                                         GtkTreeView *list_view,
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
  if ((g_strcmp0(famname, new_name) == 0) && (g_strcmp0(iter_str, tmp) != 0))
    return TRUE;
  g_free(famname);
  g_free(iter_str);
  while (gtk_tree_model_iter_next(model, &iter)) {
    iter_str = gtk_tree_model_get_string_from_iter(model, &iter);
    gtk_tree_model_get(model, &iter,
                       LTRFAMS_FAM_LV_OLDNAME, &famname,
                       -1);
    if ((g_strcmp0(famname, new_name) == 0) &&
        (g_strcmp0(iter_str, tmp) != 0))
      return TRUE;
    g_free(famname);
    g_free(iter_str);
  }
  return FALSE;
}

static void gtk_ltr_families_lv_fams_add_clicked(GT_UNUSED GtkWidget *button,
                                                 gpointer list_view)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkTreeStore *store;
  GtkTreeViewColumn *column;
  GtkTreePath *path;
  GtkCellRenderer *renderer;
  GList *tmp;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(list_view));
  store = GTK_TREE_STORE(model);
  column = gtk_tree_view_get_column(GTK_TREE_VIEW(list_view), 0);
  tmp = gtk_cell_layout_get_cells(GTK_CELL_LAYOUT(column));
  renderer = (GtkCellRenderer*) g_list_nth_data(tmp, 0);
  g_object_set(renderer, "editable", TRUE, NULL);

  gtk_tree_store_append(store, &iter, NULL);
  gtk_tree_store_set(store, &iter,
                     LTRFAMS_FAM_LV_NODE_ARRAY, NULL,
                     LTRFAMS_FAM_LV_TAB_CHILD, NULL,
                     LTRFAMS_FAM_LV_TAB_LABEL, NULL,
                     -1);

  path = gtk_tree_model_get_path(model, &iter);

  gtk_tree_view_set_cursor(GTK_TREE_VIEW(list_view),
                           path, column, TRUE);
  g_list_free(tmp);
}

static void gtk_ltr_families_lv_fams_cell_edited(
                                            GT_UNUSED GtkCellRendererText *cell,
                                                 gchar *path_string,
                                                 gchar *new_name,
                                                 gpointer list_view)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkTreeViewColumn *column;
  GtkCellRenderer *renderer;
  GList *tmp;
  gchar *old_name;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(list_view));
  gtk_tree_model_get_iter_from_string(model, &iter, path_string);
  gtk_tree_model_get(model, &iter,
                     LTRFAMS_FAM_LV_OLDNAME, &old_name,
                     -1);

  if (!old_name && (g_strcmp0(new_name, "") == 0))
    gtk_tree_store_remove(GTK_TREE_STORE(model), &iter);
  else if (g_strcmp0(new_name, "") == 0) {
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(NULL,
                              GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                    "Family name must not be empty!");
    gtk_window_set_title(GTK_WINDOW(dialog), "Information");
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
                              GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                      GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                      "Family name already exists!");
      gtk_window_set_title(GTK_WINDOW(dialog), "Information");
      gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
      gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
      column = gtk_tree_view_get_column(GTK_TREE_VIEW(list_view), 0);
      tmp = gtk_cell_layout_get_cells(GTK_CELL_LAYOUT(column));
      renderer = (GtkCellRenderer*) g_list_nth_data(tmp, 0);
      g_signal_emit_by_name(renderer, "editing-canceled");
    } else {
      char tmp_name[BUFSIZ];
      gtk_tree_model_get(model, &iter,
                         LTRFAMS_FAM_LV_NODE_ARRAY, &nodes,
                         -1);
      g_snprintf(tmp_name, BUFSIZ, "%s (%lu)", new_name, gt_array_size(nodes));
      gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
                         LTRFAMS_FAM_LV_CURNAME, tmp_name,
                         -1);
      gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
                         LTRFAMS_FAM_LV_OLDNAME, new_name,
                         -1);
      gtk_tree_model_get(model, &iter,
                         LTRFAMS_FAM_LV_TAB_LABEL, &label,
                         -1);
      if (label)
        gtk_label_close_set_text(GTKLABELCLOSE(label), new_name);

      if (!nodes) {
        nodes = gt_array_new(sizeof(GtGenomeNode*));
        gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
                           LTRFAMS_FAM_LV_NODE_ARRAY, nodes,
                           -1);
      }
      g_free(iter_str);
    }
  }
  column = gtk_tree_view_get_column(GTK_TREE_VIEW(list_view), 0);
  tmp = gtk_cell_layout_get_cells(GTK_CELL_LAYOUT(column));
  renderer = (GtkCellRenderer*) g_list_nth_data(tmp, 0);
  g_object_set(renderer, "editable", FALSE, NULL);
  g_list_free(tmp);
  g_free(old_name);
}

static void gtk_ltr_families_lv_fams_cell_edit_canceled(
                                            GT_UNUSED GtkCellRenderer *renderer,
                                            gpointer list_view)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreeSelection *sel;
  gchar *name;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(list_view));
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_view));
  gtk_tree_selection_get_selected(sel, &model, &iter);
  gtk_tree_model_get(model, &iter, LTRFAMS_FAM_LV_CURNAME, &name, -1);

  if(!name)
    gtk_tree_store_remove(GTK_TREE_STORE(model), &iter);
  g_free(name);
}

static void gtk_ltr_families_lv_fams_cell_edit_started(
                                            GT_UNUSED GtkCellRenderer *renderer,
                                                      GtkCellEditable *editable,
                                                       gchar *path_str,
                                                       gpointer list_view)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreePath *path = NULL;
  const char *text = NULL;

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
}

static void gtk_ltr_families_lv_fams_row_activated(GtkTreeView *treeview,
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

  model = gtk_tree_view_get_model(treeview);
  gtk_tree_model_get_iter(model, &iter, path);
  gtk_tree_model_get(model, &iter,
                     LTRFAMS_FAM_LV_TAB_LABEL, &tab_label,
                     LTRFAMS_FAM_LV_NODE_ARRAY, &nodes,
                     -1);
  notebook = GTK_NOTEBOOK(ltrfams->nb_family);

  if (tab_label) {
    nbpage =
          GPOINTER_TO_INT(
           gtk_label_close_get_button_data(GTKLABELCLOSE(tab_label), "nbpage"));
    gtk_notebook_set_current_page(notebook, nbpage);
  } else {
    gtk_ltr_families_nb_fam_add_tab(model, &iter, nodes, ltrfams);
  }
}

static void gtk_ltr_families_lv_fams_create(GtkLTRFamilies *ltrfams)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkTreeStore *store;

  renderer = gtk_cell_renderer_text_new();
  g_signal_connect(G_OBJECT(renderer), "edited",
                   G_CALLBACK(gtk_ltr_families_lv_fams_cell_edited),
                   ltrfams->lv_families);
  g_signal_connect(G_OBJECT(renderer), "editing-canceled",
                   G_CALLBACK(gtk_ltr_families_lv_fams_cell_edit_canceled),
                   ltrfams->lv_families);
  g_signal_connect(G_OBJECT(renderer), "editing-started",
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
                                                     renderer, "text",
                                                     LTRFAMS_FAM_LV_CURNAME,
                                                     NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrfams->lv_families), column);

  store = gtk_tree_store_new(LTRFAMS_FAM_LV_N_COLUMS,
                             G_TYPE_POINTER,
                             G_TYPE_POINTER,
                             G_TYPE_POINTER,
                             G_TYPE_STRING,
                             G_TYPE_STRING);

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

void gtk_ltr_families_fill_with_data(GtkLTRFamilies *ltrfams,
                                     GtArray *nodes,
                                     GtHashmap *features,
                                     unsigned long noc)
{
  ltrfams->nodes = nodes;
  ltrfams->features = features;
  ltrfams->n_features = noc;
  gtk_ltr_families_nb_fam_create(ltrfams);
}

static void gtk_ltr_families_init(GtkLTRFamilies *ltrfams)
{
  GtkWidget *vbox1,
            *vbox2,
            *vbox3,
            *toolbar1,
            *toolbar2,
            *hpane,
            *vpane,
            *label,
            *sw1,
            *sw2,
            *sw3;
  GtkAdjustment *vadj = NULL;
  GtkToolItem *add,
              *copy;

  vbox1 = gtk_vbox_new(FALSE, 0);
  toolbar1 = gtk_toolbar_new();
  add = gtk_tool_button_new_from_stock(GTK_STOCK_ADD);
  gtk_toolbar_set_show_arrow(GTK_TOOLBAR(toolbar1), TRUE);
  gtk_toolbar_set_style(GTK_TOOLBAR(toolbar1), GTK_TOOLBAR_ICONS);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar1), add, 0);
  sw1 = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw1),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
  ltrfams->lv_families = gtk_tree_view_new();
  g_signal_connect(G_OBJECT(add), "clicked",
                           G_CALLBACK(gtk_ltr_families_lv_fams_add_clicked),
                           ltrfams->lv_families);
  gtk_container_add(GTK_CONTAINER(sw1), ltrfams->lv_families);
  gtk_box_pack_start(GTK_BOX(vbox1), toolbar1, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox1), sw1, TRUE, TRUE, 0);

  gtk_paned_add1(GTK_PANED(ltrfams), vbox1);

  vpane = gtk_vpaned_new();
  vbox2 = gtk_vbox_new(FALSE, 0);
  toolbar2 = gtk_toolbar_new();
  copy = gtk_tool_button_new_from_stock(GTK_STOCK_COPY);
  gtk_toolbar_set_show_arrow(GTK_TOOLBAR(toolbar2), TRUE);
  gtk_toolbar_set_style(GTK_TOOLBAR(toolbar2), GTK_TOOLBAR_ICONS);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar2), copy, 0);
  ltrfams->nb_family = gtk_notebook_new();
  gtk_box_pack_start(GTK_BOX(vbox2), toolbar2, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox2), ltrfams->nb_family, TRUE, TRUE, 0);

  gtk_paned_add1(GTK_PANED(vpane), vbox2);

  hpane = gtk_hpaned_new();
  vbox3 = gtk_vbox_new(FALSE, 0);
  label = gtk_label_new(LTRFAM_DETINFO);
  sw2 = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw2),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
  ltrfams->tv_details = gtk_tree_view_new();
  gtk_container_add(GTK_CONTAINER(sw2), ltrfams->tv_details);
  gtk_box_pack_start(GTK_BOX(vbox3), label, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox3), sw2, TRUE, TRUE, 0);

  gtk_paned_add1(GTK_PANED(hpane), vbox3);

  sw3 = gtk_scrolled_window_new(NULL, vadj);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw3),
                                 GTK_POLICY_NEVER,
                                 GTK_POLICY_AUTOMATIC);
  ltrfams->image_area = gtk_layout_new(NULL, vadj);
  g_signal_connect(ltrfams->image_area, "expose-event",
      G_CALLBACK (image_area_expose_event), ltrfams);
  gtk_layout_set_size(GTK_LAYOUT(ltrfams->image_area), 100, 100);
  gtk_container_add(GTK_CONTAINER(sw3), ltrfams->image_area);

  gtk_paned_add2(GTK_PANED(hpane), sw3);

  gtk_paned_add2(GTK_PANED(vpane), hpane);

  gtk_paned_add2(GTK_PANED(ltrfams), vpane);

  gtk_paned_set_position(GTK_PANED(hpane), 400);
  gtk_paned_set_position(GTK_PANED(vpane), 400);
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

GtkWidget* gtk_ltr_families_new()
{
  GtkLTRFamilies *ltrfams;
  ltrfams = gtk_type_new(GTK_LTR_FAMILIES_TYPE);
  gtk_ltr_families_lv_fams_create(ltrfams);
  gtk_ltr_families_tv_det_create(GTK_TREE_VIEW(ltrfams->tv_details));
  return GTK_WIDGET(ltrfams);
}
