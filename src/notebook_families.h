#ifndef NOTEBOOK_FAMILIES_H
#define NOTEBOOK_FAMILIES_H

#include "ltrgui.h"

void nb_families_page_reordered(GtkNotebook *notebook,
                                     GtkWidget *child,
                                     guint page_num,
                                     GUIData *ltrgui);

void nb_families_close_tab_clicked(GtkButton *button,
                                        GUIData *ltrgui);

void nb_families_add_tab(GtkTreeModel *model,
                         GtkTreeIter *iter,
                         GUIData *ltrgui);

void nb_families_refresh_notebook_nums(GtkNotebook *notebook);

void nb_families_init(GUIData *ltrgui, GtArray *nodes);

#endif // NOTEBOOK_FAMILIES_H
