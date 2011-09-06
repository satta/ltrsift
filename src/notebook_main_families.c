#include "notebook_main_families.h"
#include "unused.h"

void nb_main_families_init(GUIData *ltrgui, GtArray *nodes, unsigned long noc)
{
  GtkWidget *child, *label;
  ltrgui->nb_main_families = gtk_notebook_new();

  child = gtk_ltr_family_new(ltrgui->features, nodes, noc);
  label = gtk_label_new("General");

  gtk_widget_show(child);

  G_UNUSED gint page =
                gtk_notebook_append_page(GTK_NOTEBOOK(ltrgui->nb_main_families),
                                         child, label);

  gtk_box_pack_end(GTK_BOX(ltrgui->hbox1_main), ltrgui->nb_main_families,
                   TRUE, TRUE, 5);
  gtk_widget_show(ltrgui->nb_main_families);
}

/*void nb_main_families_first_page(G_UNUSED GUIData *ltrgui)
{
 function for first page
}*/

/*void nb_main_families_new_page(G_UNUSED GUIData *ltrgui)
{
   function for adding a new page when the user clicks on a family
}*/
