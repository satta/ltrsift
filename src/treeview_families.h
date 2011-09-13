#ifndef TREEVIEW_FAMILIES_H
#define TREEVIEW_FAMILIES_H

#include "ltrgui.h"

typedef enum {
  TV_FAM_NODE = 0,
  TV_FAM_NODE_ARRAY,
  TV_FAM_TAB_CHILD,
  TV_FAM_TAB_LABEL,
  TV_FAM_NAME,
  TV_FAM_N_COLUMS
} TreeViewFamiliesColumns;

struct DATA {
        gchar *row;
        GtGenomeNode **gn;
        GtHashmap *feat;
};

enum {
        TARGET_STRING,
        TARGET_POINTER,
};

static const GtkTargetEntry drag_targets = {
        "STRING", GTK_TARGET_SAME_APP,TARGET_STRING
};

void tv_families_init(GUIData *ltrgui);

void add_family_button_clicked(GtkWidget *button, GUIData *ltrgui);

void tv_families_refresh_notebook_nums(GtkNotebook *notebook);

void on_drag_data_get(GtkWidget *widget, GdkDragContext *drag_context,
                      GtkSelectionData *sdata, guint info, guint time,
                      gpointer user_data);

void on_drag_data_received(GtkWidget *widget, GdkDragContext *drag_context,
                        gint x, gint y, GtkSelectionData *sdata, guint info,
                        guint time, gpointer user_data);

void
on_drag_begin
(GtkWidget *widget, GdkDragContext *context, gpointer user_data);

#endif // TREEVIEW_FAMILIES_H
