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
#include "treeview_families.h"
#include "treeview_families_dnd.h"

void ltrfam_lv_on_drag_data_get(GtkWidget *widget,
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
                     LTRFAM_LV_NODE, &tdata->gn,
                     LTRFAM_LV_FEAT, &tdata->feat,
                     -1);
  /* add retrieved data to GtkSelectionData object */
  gtk_selection_data_set(sdata,
                         gdk_atom_intern("FamilyTransferData pointer", FALSE),
                         8,
                         (void*) &tdata,
                         sizeof(tdata));
}

gboolean tv_families_on_drag_motion(GtkWidget *widget,
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

void tv_families_on_drag_data_received(GtkWidget *widget,
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
                     TV_FAM_NODE_ARRAY, &nodes,
                     TV_FAM_TAB_CHILD, &tab_child,
                     TV_FAM_OLDNAME, &oldname,
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
  sel2 = gtk_tree_view_get_selection(GTK_TREE_VIEW(
                                       gtk_ltr_family_get_list_view(
                                         GTKLTRFAMILY(tmp_tab_child))));
  gtk_tree_selection_get_selected(sel2, &model2, &iter2);
  gtk_tree_model_get(model2, &iter2,
                     LTRFAM_LV_ROWREF, &tv_ref,
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
                       TV_FAM_NODE_ARRAY, &tmp_nodes,
                       TV_FAM_OLDNAME, &tmp_oldname,
                       -1);
    remove_node_from_array(tmp_nodes, tdata->gn);
    g_snprintf(tmp_curname, BUFSIZ, "%s (%lu)",
               tmp_oldname, gt_array_size(tmp_nodes));
    gtk_tree_store_set(GTK_TREE_STORE(model), &tv_iter,
                       TV_FAM_CURNAME, tmp_curname,
                       -1);
    gtk_tree_path_free(tv_path);
  }
  gtk_list_store_remove(GTK_LIST_STORE(model2), &iter2);
  /* clear detailed view if shown <gn> was drag'n'dropped to another family */
  gtk_ltr_family_clear_detail_on_equal_nodes(GTKLTRFAMILY(tmp_tab_child),
                                             tdata->gn);

  /* add node to "family node array" and insert node into list_view from
     assigned tab (if it exists) */
  gt_array_add(nodes, tdata->gn);
  g_snprintf(curname, BUFSIZ, "%s (%lu)", oldname, gt_array_size(nodes));
  gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
                     TV_FAM_CURNAME, curname,
                     -1);
  if (tab_child) {
    GtkTreePath *path;
    GtkTreeRowReference  *tv_ref2;
    path = gtk_tree_model_get_path(model, &iter);
    tv_ref2 = gtk_tree_row_reference_new(model, path);
    gtk_ltr_family_list_view_append(GTKLTRFAMILY(tab_child), tdata->gn,
                                    tdata->feat, tv_ref2, NULL);
    gtk_tree_path_free(path);
  }
}
