#ifndef TREEVIEW_FAMILIES_H
#define TREEVIEW_FAMILIES_H

#include "ltrgui.h"

typedef enum {
  TV_FAM_NODE = 0,
  TV_FAM_NID,
  TV_FAM_NAME,
  TV_FAM_N_COLUMS
} TreeViewFamiliesColumns;

void tv_families_init(GUIData *ltrgui);

void add_family_button_clicked(GtkWidget *button, GUIData *ltrgui);

#endif // TREEVIEW_FAMILIES_H
