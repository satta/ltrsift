/*
  Copyright (c) 2011-2012 Sascha Kastens <sascha.kastens@studium.uni-hamburg.de>
  Copyright (c) 2011-2012 Center for Bioinformatics, University of Hamburg

  Permission to use, copy, modify, and distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef __GTK_LTR_FAMILIES_H__
#define __GTK_LTR_FAMILIES_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include "gtk_label_close.h"
#include "genometools.h"

#define GTK_LTR_FAMILIES_TYPE\
        gtk_ltr_families_get_type()
#define GTK_LTR_FAMILIES(obj)\
        G_TYPE_CHECK_INSTANCE_CAST((obj), GTK_LTR_FAMILIES_TYPE, GtkLTRFamilies)
#define GTK_LTR_FAMILIES_CLASS(klass)\
        G_TYPE_CHECK_CLASS_CAST((klass), GTK_LTR_FAMILIES_TYPE,\
                                GtkLTRFamiliesClass)
#define IS_GTK_LTR_FAMILIES(obj)\
        G_TYPE_CHECK_INSTANCE_TYPE((obj), GTK_LTR_FAMILIES_TYPE)
#define IS_GTK_LTR_FAMILIES_CLASS(klass)\
        G_TYPE_CHECK_CLASS_TYPE((klass), GTK_LTR_FAMILIES_TYPE)

#define DEFAULT_STYLE "../style/default.style"
#define GUI_NAME      "LTRGui"

#define LTRFAMS_LV_CAPTION_SEQID "SeqID"
#define LTRFAMS_LV_CAPTION_TYPE  "S"
#define LTRFAMS_LV_CAPTION_START "Start"
#define LTRFAMS_LV_CAPTION_END   "End"
#define LTRFAMS_TV_CAPTION_INFO  "Attributes"
#define LTRFAMS_TV_CAPTION_TYPE  "Feature name"

#define ATTR_RID       "ID"
#define ATTR_CLUSTID   "clid"
#define ATTR_PFAMN     "name"
#define FNT_PROTEINM   "protein_match"
#define FNT_LTR        "long_terminal_repeat"
#define FNT_REPEATR    "repeat_region"
#define FNT_LLTR       "lLTR"
#define FNT_RLTR       "rLTR"
#define LTRFAM_DETINFO "Detailed Information"

#define TB_FAMS_ADD    "Add new family"
#define TB_FAMS_REMOVE "Remove families with less than three members"
#define TB_NB_NEW_FAM  "Check for new family with selected candidates"

#define FAMS_RM_DIALOG    "You're about to remove %d family/families. All "\
                          "members (if any) will be unclassified after this "\
                          "action and added to the 'General' tab.\n\nAre you "\
                          "sure?"
#define FAMS_EMPTY_DIALOG "Family name must not be empty!"
#define FAMS_EXIST_DIALOG "Family name already exists!"
#define NEW_FAM_DIALOG    "Please select at least three candidates for "\
                          "classification"

#define CAND_RM_DIALOG "All selected candidates will be deleted from the "\
                       "project. This action cannot be undone!\nAre you sure?"
#define CAND_UC_DIALOG "All selected members will be unclassified after this "\
                       "operation and added to the 'General' tab.\n"\
                       "Are you sure?"

#define FILE_EXISTS_DIALOG "A file named \"%s\" already exists.\n\nDo you "\
                           "want to replace it?"

#define REMOVE  "Remove selected"
#define UNCLASS "Unclassify selected"

typedef struct _FamilyTransferData  FamilyTransferData;
typedef struct _GtkLTRFamilies      GtkLTRFamilies;
typedef struct _GtkLTRFamiliesClass GtkLTRFamiliesClass;
typedef struct _FamilyThreadData FamilyThreadData;

enum {
  LTRFAMS_LV_NODE = 0,
  LTRFAMS_LV_ROWREF,
  LTRFAMS_LV_SEQID,
  LTRFAMS_LV_STRAND,
  LTRFAMS_LV_START,
  LTRFAMS_LV_END,
  LTRFAMS_LV_N_COLUMS
};

enum {
  LTRFAMS_DETAIL_TV_NODE = 0,
  LTRFAMS_DETAIL_TV_TYPE,
  LTRFAMS_DETAIL_TV_START,
  LTRFAMS_DETAIL_TV_END,
  LTRFAMS_DETAIL_TV_INFO,
  LTRFAMS_DETAIL_TV_N_COLUMS
};

enum
{
  LTRFAMS_FAM_LV_NODE_ARRAY = 0,
  LTRFAMS_FAM_LV_TAB_CHILD,
  LTRFAMS_FAM_LV_TAB_LABEL,
  LTRFAMS_FAM_LV_CURNAME,
  LTRFAMS_FAM_LV_OLDNAME,
  LTRFAMS_FAM_LV_N_COLUMS
};

struct _FamilyTransferData
{
  GtArray *nodes;
  GtkTreeRowReference *rowref;
  GList *references;
  GtkTreeView *list_view;
};

typedef enum
{
  TARGET_STRING = 0
} FamilyDnDTargets;

static const GtkTargetEntry family_drag_targets[] = {
  {"STRING", 0, TARGET_STRING}
};

struct _GtkLTRFamilies
{
  GtkHPaned hpane;

  GtkWidget *lv_families;
  GtkCellRenderer *lv_fams_renderer;
  GtkWidget *tb_lv_families;
  GtkWidget *nb_family;
  GtkWidget *tb_nb_family;
  GtkWidget *tv_details;
  GtkWidget *image_area;
  GtkWidget *hpaned;
  GtkWidget *vpaned;
  GError *gerr;
  GtDiagram *diagram;
  GtStyle *style;
  GtArray *nodes;
  GtHashmap *features;
  GtHashmap *colors;
  GtError *err;
  unsigned long n_features;
  gboolean modified;
  gchar *projectfile;
};

struct _GtkLTRFamiliesClass
{
  GtkHPanedClass parent_class;
  void (* gtk_ltr_families) (GtkLTRFamilies *ltrfams);
};

struct _FamilyThreadData
{
  GtkLTRFamilies *ltrfams;
  GtkWidget *window;
  GtkWidget *progressbar;
  GtArray *old_nodes,
          *new_nodes;
  GtError *err;
  GList *references;
  GtkTreeView *list_view;
  gchar *current_state;
  unsigned long progress;
  int had_err;
};

GType        gtk_ltr_families_get_type(void);

GtkWidget*   gtk_ltr_families_new();

char*        double_underscores(const char *str);

void         gtk_ltr_families_fill_with_data(GtkLTRFamilies *ltrfams,
                                           GtArray *nodes,
                                           GtHashmap *features,
                                           unsigned long noc);

GtkNotebook* gtk_ltr_families_get_nb(GtkLTRFamilies *ltrfams);

GtArray*     gtk_ltr_families_get_nodes(GtkLTRFamilies *ltrfams);

gboolean     gtk_ltr_families_get_modified(GtkLTRFamilies *ltrfams);

gchar*       gtk_ltr_families_get_projectfile(GtkLTRFamilies *ltrfams);

void         gtk_ltr_families_set_projectfile(GtkLTRFamilies *ltrfams,
                                              gchar *projectfile);

void         gtk_ltr_families_set_modified(GtkLTRFamilies *ltrfams,
                                           gboolean modified);

GtkTreeView* gtk_ltr_families_get_lv_fams(GtkLTRFamilies *ltrfams);

gint         gtk_ltr_families_get_position(GtkLTRFamilies *ltrfams);

gint         gtk_ltr_families_get_hpaned_position(GtkLTRFamilies *ltrfams);

gint         gtk_ltr_families_get_vpaned_position(GtkLTRFamilies *ltrfams);

void         gtk_ltr_families_set_position(GtkLTRFamilies *ltrfams, gint pos);

void         gtk_ltr_families_set_hpaned_position(GtkLTRFamilies *ltrfams,
                                                  gint pos);

void         gtk_ltr_families_set_vpaned_position(GtkLTRFamilies *ltrfams,
                                                  gint pos);

void         gtk_ltr_families_nb_fam_add_tab(GtkTreeModel *model,
                                             GtkTreeIter *iter,
                                             GtArray *nodes,
                                             gboolean load,
                                             GtkLTRFamilies *ltrfams);
#endif /* GTK_LTR_FAMILIES_H */
