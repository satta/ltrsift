#ifndef TREEVIEW_MAIN_H
#define TREEVIEW_MAIN_H

#include "ltrgui.h"

#define TV_MAIN_CAPTION_SEQID "SeqID"
#define TV_MAIN_CAPTION_TYPE  "Type"
#define TV_MAIN_CAPTION_START "Start"
#define TV_MAIN_CAPTION_END   "End"
#define TV_MAIN_CAPTION_CLUSTER "cluster no."
#define ATTR_CLUSTID "clid"
#define ATTR_PFAMN   "pfamname"
#define FNT_PROTEINM "protein_match"
#define FNT_LTR      "long_terminal_repeat"
#define FNT_REPEATR  "repeat_region"
#define FNT_LLTR     "lLTR"
#define FNT_RLTR     "rLTR"

typedef enum {
  TV_MAIN_NODE = 0,
  TV_MAIN_SEQID,
  TV_MAIN_TYPE,
  TV_MAIN_START,
  TV_MAIN_END,
  TV_MAIN_EMPTY,
  TV_MAIN_N_COLUMS
} TreeViewMainColumns;

void tv_main_init(GUIData *ltrgui, GtArray *nodes, unsigned long nof_features);

void tw_main_row_collapsed(GtkTreeView *tree_view, GtkTreeIter *iter,
                           GtkTreePath *path, GUIData *ltrgui);

void tw_main_row_expanded(GtkTreeView *tree_view, GtkTreeIter *iter,
                          GtkTreePath *path, GUIData *ltrgui);

#endif // TREEVIEW_MAIN_H
