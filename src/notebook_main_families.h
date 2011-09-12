#ifndef NOTEBOOK_MAIN_FAMILIES_H
#define NOTEBOOK_MAIN_FAMILIES_H

#include "ltrgui.h"

void nb_main_families_page_reordered(GtkNotebook *notebook,
                                     GtkWidget *child,
                                     guint page_num,
                                     GUIData *ltrgui);

void nb_main_families_init(GUIData *ltrgui, GtArray *nodes);

void nb_main_families_close_tab_clicked(GtkButton *button,
                                        GUIData *ltrgui);

#endif // NOTEBOOK_MAIN_FAMILIES_H
