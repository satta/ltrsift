#include "notebook_main_families.h"
#include "treeview_families.h"
#include "unused.h"

void nb_main_families_page_reordered(GtkNotebook *notebook,
                                     G_UNUSED GtkWidget *child,
                                     G_UNUSED guint page_num,
                                     G_UNUSED GUIData *ltrgui)
{
  tv_families_refresh_notebook_nums(notebook);
}

void nb_main_families_init(GUIData *ltrgui, GtArray *nodes)
{
  GtkWidget *child, *label;
  gint nbpage;
  ltrgui->nb_main_families = gtk_notebook_new();
  gtk_notebook_set_scrollable(GTK_NOTEBOOK(ltrgui->nb_main_families), TRUE);

  g_signal_connect(G_OBJECT(ltrgui->nb_main_families), "page-reordered",
                   G_CALLBACK(nb_main_families_page_reordered), ltrgui);

  child = gtk_ltr_family_new(ltrgui->features, nodes, ltrgui->n_features);
  label = gtk_label_close_new("General", NULL, NULL);
  gtk_label_close_hide_close(GTKLABELCLOSE(label));

  gtk_widget_show(child);

  nbpage = gtk_notebook_append_page(GTK_NOTEBOOK(ltrgui->nb_main_families),
                                    child, label);
  gtk_label_close_set_button_data(GTKLABELCLOSE(label),
                                  "nbpage",
                                  GINT_TO_POINTER(nbpage));

  gtk_box_pack_end(GTK_BOX(ltrgui->hbox1_main), ltrgui->nb_main_families,
                   TRUE, TRUE, 5);
  gtk_widget_show(ltrgui->nb_main_families);
}

void nb_main_families_close_tab_clicked(G_UNUSED GtkButton *button,
                                        G_UNUSED GUIData *ltrgui)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkWidget *tab_label;
  gint nbpage, tmp = -1;

  nbpage = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "nbpage"));
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->tv_families));
  gtk_tree_model_get_iter_first(model, &iter);
  gtk_tree_model_get(model, &iter,
                     TV_FAM_TAB_LABEL, &tab_label,
                     -1);
  if (tab_label)
    tmp = GPOINTER_TO_INT(
           gtk_label_close_get_button_data(GTKLABELCLOSE(tab_label), "nbpage"));
  if (nbpage == tmp)
    gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
                       TV_FAM_TAB_CHILD, NULL,
                       TV_FAM_TAB_LABEL, NULL,
                       -1);
  else {
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
  gtk_notebook_remove_page(GTK_NOTEBOOK(ltrgui->nb_main_families), nbpage);
  tv_families_refresh_notebook_nums(GTK_NOTEBOOK(ltrgui->nb_main_families));
}
