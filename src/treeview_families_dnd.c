#include <string.h>
#include "treeview_families.h"
#include "treeview_families_dnd.h"
#include "unused.h"

void ltrfam_lv_on_drag_data_get(GtkWidget *widget,
                                G_UNUSED GdkDragContext *drag_context,
                                GtkSelectionData *sdata,
                                G_UNUSED guint info,
                                G_UNUSED guint time,
                                G_UNUSED gpointer user_data)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  FamilyTransferData *tmp;
  gboolean valid;

  /* retrieve data from selected row, do nothing if no row is selected */
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
  valid = gtk_tree_selection_get_selected(sel, &model, &iter);
  if (!valid)
    return;
  tmp = g_slice_new(FamilyTransferData);
  gtk_tree_model_get(model, &iter,
                     LTRFAM_LV_SEQID, &tmp->row,
                     LTRFAM_LV_NODE, &tmp->gn,
                     LTRFAM_LV_FEAT, &tmp->feat,
                     -1);
  /* add retrieved data to GtkSelectionData object */
  gtk_selection_data_set(sdata,
                         gdk_atom_intern("FamilyTransferData pointer", false),
                         8,
                         (void*) &tmp,
                         sizeof(tmp));
}

gboolean tv_families_on_drag_motion(GtkWidget *widget,
                                    G_UNUSED GdkDragContext *context,
                                    gint x,
                                    gint y,
                                    G_UNUSED guint time,
                                    G_UNUSED gpointer user_data)
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
  return FALSE;
}

void tv_families_on_drag_data_received(G_UNUSED GtkWidget *widget,
                                       G_UNUSED GdkDragContext *drag_context,
                                       G_UNUSED gint x,
                                       G_UNUSED gint y,
                                       GtkSelectionData *sdata,
                                       G_UNUSED guint info,
                                       G_UNUSED guint time,
                                       gpointer user_data)
{
  GtkTreeModel *model, *model2;
  GtkTreeIter iter, iter2, child;
  GtkTreeSelection *sel, *sel2;
  GtkWidget *tab_child;
  FamilyTransferData *tmp = NULL;
  GtArray *nodes;
  gboolean valid;

  /* add row to selected family, do nothing if no family is selected */
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
  valid = gtk_tree_selection_get_selected(sel, &model, &iter);
  if (!valid)
    return;
  gtk_tree_store_append(GTK_TREE_STORE(model), &child, &iter);

  /* copy recieved data into new struct */
  memcpy(&tmp, sdata->data, sizeof(tmp));

  /* insert data into <tv_families> */
  gtk_tree_store_set(GTK_TREE_STORE(model), &child,
                     TV_FAM_NODE, tmp->gn,
                     TV_FAM_NAME, tmp->row,
                     -1);

  /* remove row from drag source */
  sel2 = gtk_tree_view_get_selection(GTK_TREE_VIEW(user_data));
  gtk_tree_selection_get_selected(sel2, &model2, &iter2);
  gtk_list_store_remove(GTK_LIST_STORE(model2), &iter2);

  /* add node to "family node array" and insert node into list_view from
     assigned tab (if exists) */
  gtk_tree_model_get(model, &iter,
                     TV_FAM_NODE_ARRAY, &nodes,
                     TV_FAM_TAB_CHILD, &tab_child,
                     -1);
  gt_array_add(nodes, tmp->gn);
  if (tab_child)
    gtk_ltr_family_list_view_append(GTKLTRFAMILY(tab_child),
                                    tmp->gn, tmp->feat, NULL);
}
