#ifndef TREEVIEW_MAIN_H
#define TREEVIEW_MAIN_H

#include "ltrgui.h"

#define TV_MAIN_CAPTION_SEQID "SeqID"
#define TV_MAIN_CAPTION_TYPE "Type"
#define TV_MAIN_CAPTION_START "Start"
#define TV_MAIN_CAPTION_END "End"

typedef enum {
  TV_MAIN_SEQID = 0,
  TV_MAIN_TYPE,
  TV_MAIN_START,
  TV_MAIN_END,
  TV_MAIN_N_COLUMS
} TreeViewMainColumns;

void tv_main_init(GUIData *ltrgui);

#endif // TREEVIEW_MAIN_H
