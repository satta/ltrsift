#include "notebook_main_families.h"
#include "treeview_families.h"
#include "unused.h"

void nb_main_families_page_reordered(GtkNotebook *notebook,
                                     G_UNUSED GtkWidget *child,
                                     G_UNUSED guint page_num,
                                     GUIData *ltrgui)
{
  tv_families_notebook_nums(GTK_TREE_VIEW(ltrgui->tv_families),
                                    notebook);
}

void nb_main_families_init(GUIData *ltrgui, GtArray *nodes)
{
  GtkWidget *child, *label;
  ltrgui->nb_main_families = gtk_notebook_new();
  gtk_notebook_set_scrollable(GTK_NOTEBOOK(ltrgui->nb_main_families), TRUE);

  g_signal_connect(G_OBJECT(ltrgui->nb_main_families), "page-reordered",
                   G_CALLBACK(nb_main_families_page_reordered), ltrgui);

  child = gtk_ltr_family_new(ltrgui->features, nodes, ltrgui->n_features);
  label = gtk_label_close_new("General", NULL, NULL, TRUE);

  gtk_widget_show(child);

  G_UNUSED gint page =
                gtk_notebook_append_page(GTK_NOTEBOOK(ltrgui->nb_main_families),
                                         child, label);

  gtk_box_pack_end(GTK_BOX(ltrgui->hbox1_main), ltrgui->nb_main_families,
                   TRUE, TRUE, 5);
  gtk_widget_show(ltrgui->nb_main_families);
}

void nb_main_families_close_tab_clicked(G_UNUSED GtkButton *button,
                                        GUIData *ltrgui)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  gint nbpage, tmp;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->tv_families));
  nbpage =
          gtk_notebook_get_current_page(GTK_NOTEBOOK(ltrgui->nb_main_families));
  gtk_tree_model_get_iter_first(model, &iter);
  gtk_tree_model_get(model, &iter,
                     TV_FAM_NID, &tmp,
                     -1);
  if (nbpage == tmp)
    gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
                       TV_FAM_NID, -1,
                       -1);
  else {
    while (gtk_tree_model_iter_next(model, &iter)) {
      gtk_tree_model_get(model, &iter,
                         TV_FAM_NID, &tmp,
                         -1);
      if (nbpage == tmp)
        gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
                           TV_FAM_NID, -1,
                           -1);
    }
  }
  gtk_notebook_remove_page(GTK_NOTEBOOK(ltrgui->nb_main_families), nbpage);
}

GtkWidget* create_notebook_label_with_close_button(const gchar *text,
                                                   GUIData *ltrgui)
{
  GtkWidget *hbox, *label, *button, *image;

  hbox = gtk_hbox_new(FALSE, 3);
  label = gtk_label_new(text);
  gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 0);
  button = gtk_button_new();
  g_signal_connect(G_OBJECT(button), "clicked",
                     G_CALLBACK(nb_main_families_close_tab_clicked), ltrgui);
  gtk_box_pack_start(GTK_BOX (hbox), button, FALSE, FALSE, 0);
  gtk_widget_set_size_request(button, 20, 18);
  image = gtk_image_new_from_stock(GTK_STOCK_CLOSE, GTK_ICON_SIZE_MENU);
  gtk_container_add(GTK_CONTAINER(button), image);
  gtk_widget_show_all(hbox);
  return(hbox);
}
