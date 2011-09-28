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

#include "notebook_families.h"
#include "treeview_families.h"
#include "treeview_families_dnd.h"

void nb_families_refresh_notebook_nums(GtkNotebook *notebook)
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

void nb_families_page_reordered(GtkNotebook *notebook,
                                     GT_UNUSED GtkWidget *child,
                                     GT_UNUSED guint page_num,
                                     GT_UNUSED GUIData *ltrgui)
{
  nb_families_refresh_notebook_nums(notebook);
}

void nb_families_close_tab_clicked(GT_UNUSED GtkButton *button,
                                   GT_UNUSED GUIData *ltrgui)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkWidget *tab_label;
  gint nbpage,
       tmp = -1;

  nbpage = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "nbpage"));
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->tv_families));
  gtk_tree_model_get_iter_first(model, &iter);
  gtk_tree_model_get(model, &iter,
                     TV_FAM_TAB_LABEL, &tab_label,
                     -1);
  if (tab_label)
    tmp = GPOINTER_TO_INT(
           gtk_label_close_get_button_data(GTKLABELCLOSE(tab_label), "nbpage"));
  if (nbpage == tmp) {
    gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
                       TV_FAM_TAB_CHILD, NULL,
                       TV_FAM_TAB_LABEL, NULL,
                       -1);
  } else {
    while (gtk_tree_model_iter_next(model, &iter)) {
      gtk_tree_model_get(model, &iter,
                         TV_FAM_TAB_LABEL, &tab_label,
                         -1);
      if (tab_label)
        tmp = GPOINTER_TO_INT(
               gtk_label_close_get_button_data(GTKLABELCLOSE(tab_label),
                                               "nbpage"));
      if (nbpage == tmp)
        gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
                           TV_FAM_TAB_CHILD, NULL,
                           TV_FAM_TAB_LABEL, NULL,
                           -1);
    }
  }
  gtk_notebook_remove_page(GTK_NOTEBOOK(ltrgui->nb_families), nbpage);
  nb_families_refresh_notebook_nums(GTK_NOTEBOOK(ltrgui->nb_families));
}

void nb_families_add_tab(GtkTreeModel *model,
                         GtkTreeIter *iter,
                         GUIData *ltrgui)
{
  GtGenomeNode **gn;
  GtkWidget *child,
            *label;
  GtkTreeIter tmp;
  gint nbpage;
  gchar *name;
  gboolean valid = FALSE;

  gtk_tree_model_get(model, iter,
                     TV_FAM_NAME, &name,
                     -1);
  child = gtk_ltr_family_new(ltrgui->features, NULL, ltrgui->n_features);
  valid = gtk_tree_model_iter_children(model, &tmp, iter);
  if (valid) {
    GtkTreeRowReference *rowref, *tmp_ref;
    GtkTreePath *path;
    gtk_tree_model_get(model, &tmp,
                       TV_FAM_NODE, &gn,
                       -1);
    path = gtk_tree_model_get_path(model, &tmp);
    tmp_ref = gtk_tree_row_reference_new(model, path);
    rowref = gtk_ltr_family_list_view_append(GTKLTRFAMILY(child),
                                             gn,
                                             ltrgui->features,
                                             tmp_ref,
                                             NULL);
    gtk_tree_store_set(GTK_TREE_STORE(model), &tmp,
                       TV_FAM_ROWREF, rowref,
                       -1);
    gtk_tree_path_free(path);
    while (gtk_tree_model_iter_next(model, &tmp)) {
      gtk_tree_model_get(model, &tmp,
                         TV_FAM_NODE, &gn,
                         -1);
      path = gtk_tree_model_get_path(model, &tmp);
      tmp_ref = gtk_tree_row_reference_new(model, path);
      rowref = gtk_ltr_family_list_view_append(GTKLTRFAMILY(child),
                                               gn,
                                               ltrgui->features,
                                               tmp_ref,
                                               NULL);
      gtk_tree_store_set(GTK_TREE_STORE(model), &tmp,
                         TV_FAM_ROWREF, rowref,
                         -1);
      gtk_tree_path_free(path);
    }
  }
  gtk_widget_show(child);

  /* Set <child->list_view> as the source of the Drag-N-Drop operation */
  gtk_drag_source_set(gtk_ltr_family_get_list_view(GTKLTRFAMILY(child)),
                      GDK_BUTTON1_MASK, family_drag_targets,
                      G_N_ELEMENTS(family_drag_targets),
                      GDK_ACTION_COPY|GDK_ACTION_MOVE);
  /* Attach a "drag-data-get" signal to send out the dragged data */
  g_signal_connect(G_OBJECT(gtk_ltr_family_get_list_view(GTKLTRFAMILY(child))),
                   "drag-data-get",
                   G_CALLBACK(ltrfam_lv_on_drag_data_get),NULL);

  label = gtk_label_close_new(name,
                              G_CALLBACK(nb_families_close_tab_clicked),
                              ltrgui);
  nbpage = gtk_notebook_append_page(GTK_NOTEBOOK(ltrgui->nb_families),
                                    child,
                                    label);
  gtk_label_close_set_button_data(GTKLABELCLOSE(label),
                                  "nbpage",
                                  GINT_TO_POINTER(nbpage));
  gtk_notebook_set_tab_reorderable(GTK_NOTEBOOK(ltrgui->nb_families),
                                   child,
                                   TRUE);
  gtk_tree_store_set(GTK_TREE_STORE(model), iter,
                     TV_FAM_TAB_CHILD, child,
                     TV_FAM_TAB_LABEL, label,
                     -1);
  gtk_notebook_set_current_page(GTK_NOTEBOOK(ltrgui->nb_families), nbpage);
  g_free(name);
}

void nb_families_init(GUIData *ltrgui)
{
  GtkWidget *child, *label;
  gint nbpage;
  ltrgui->nb_families = gtk_notebook_new();
  gtk_notebook_set_scrollable(GTK_NOTEBOOK(ltrgui->nb_families), TRUE);

  g_signal_connect(G_OBJECT(ltrgui->nb_families), "page-reordered",
                   G_CALLBACK(nb_families_page_reordered), ltrgui);

  child = gtk_ltr_family_new(ltrgui->features, ltrgui->nodes,
                             ltrgui->n_features);
  label = gtk_label_close_new("General", NULL, NULL);
  gtk_label_close_hide_close(GTKLABELCLOSE(label));

  /* Set <child->list_view> as the source of the Drag-N-Drop operation */
  gtk_drag_source_set(gtk_ltr_family_get_list_view(GTKLTRFAMILY(child)),
                      GDK_BUTTON1_MASK,
                      family_drag_targets,
                      G_N_ELEMENTS(family_drag_targets),
                      GDK_ACTION_COPY|GDK_ACTION_MOVE);
  /* Attach a "drag-data-get" signal to send out the dragged data */
  g_signal_connect(G_OBJECT(gtk_ltr_family_get_list_view(GTKLTRFAMILY(child))),
                   "drag-data-get",
                   G_CALLBACK(ltrfam_lv_on_drag_data_get),NULL);
  /* Attach a "drag-data-received" signal to pull in the dragged data */
  g_signal_connect(G_OBJECT(ltrgui->tv_families), "drag-data-received",
                   G_CALLBACK(tv_families_on_drag_data_received),
                   ltrgui->nb_families);
  gtk_widget_show(child);

  nbpage = gtk_notebook_append_page(GTK_NOTEBOOK(ltrgui->nb_families),
                                    child, label);
  gtk_notebook_set_tab_reorderable(GTK_NOTEBOOK(ltrgui->nb_families),
                                   child,
                                   TRUE);
  gtk_label_close_set_button_data(GTKLABELCLOSE(label),
                                  "nbpage",
                                  GINT_TO_POINTER(nbpage));
  g_object_set_data(G_OBJECT(ltrgui->nb_families),
                    "main_tab",
                    GINT_TO_POINTER(nbpage));
  gtk_box_pack_end(GTK_BOX(ltrgui->hbox1_main), ltrgui->nb_families,
                   TRUE, TRUE, 5);
  gtk_widget_show(ltrgui->nb_families);
}
