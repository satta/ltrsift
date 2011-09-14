#ifndef TREEVIEW_FAMILIES_DND_H
#define TREEVIEW_FAMILIES_DND_H

typedef struct _FamilyTransferData FamilyTransferData;

struct _FamilyTransferData
{
  gchar *row;
  GtGenomeNode **gn;
  GtHashmap *feat;
};

typedef enum {
  TARGET_STRING = 0
} FamilyTargets;

static const GtkTargetEntry family_drag_targets = {
  "STRING",
  GTK_TARGET_SAME_APP,
  TARGET_STRING
};

void ltrfam_lv_on_drag_data_get(GtkWidget *widget,
                                GdkDragContext *drag_context,
                                GtkSelectionData *sdata,
                                guint info,
                                guint time,
                                gpointer user_data);

void tv_families_on_drag_data_received(GtkWidget *widget,
                                       GdkDragContext *drag_context,
                                       gint x,
                                       gint y,
                                       GtkSelectionData *sdata,
                                       guint info,
                                       guint time,
                                       gpointer user_data);

gboolean tv_families_on_drag_motion(GtkWidget *widget,
                                    GdkDragContext *context,
                                    gint x,
                                    gint y,
                                    guint time,
                                    gpointer user_data);

#endif // TREEVIEW_FAMILIES_DND_H
