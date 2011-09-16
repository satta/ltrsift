#ifndef TREEVIEW_FAMILIES_H
#define TREEVIEW_FAMILIES_H

#include "ltrgui.h"

typedef enum {
  TV_FAM_NODE = 0,
  TV_FAM_NODE_ARRAY,
  TV_FAM_ROWREF,
  TV_FAM_TAB_CHILD,
  TV_FAM_TAB_LABEL,
  TV_FAM_NAME,
  TV_FAM_N_COLUMS
} TreeViewFamiliesColumns;

void tv_families_init(GUIData *ltrgui);

void tv_families_add_family_button_clicked(GtkWidget *button, GUIData *ltrgui);

void remove_node_from_array(GtArray *nodes, GtGenomeNode **gn);

#endif // TREEVIEW_FAMILIES_H
