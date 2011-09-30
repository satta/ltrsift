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

#ifndef __GTK_LTR_FAMILY_H__
#define __GTK_LTR_FAMILY_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include "genometools.h"

#define GTKLTRFAMILY_TYPE\
        gtk_ltr_family_get_type()
#define GTKLTRFAMILY(obj)\
        G_TYPE_CHECK_INSTANCE_CAST((obj), GTKLTRFAMILY_TYPE, GtkLTRFamily)
#define GTKLTRFAMILY_CLASS(klass)\
        G_TYPE_CHECK_CLASS_CAST((klass), GTKLTRFAMILY_TYPE, GtkLTRFamilyClass)
#define IS_GTKLTRFAMILY(obj)\
        G_TYPE_CHECK_INSTANCE_TYPE((obj), GTKLTRFAMILY_TYPE)
#define IS_GTKLTRFAMILY_CLASS(klass)\
        G_TYPE_CHECK_CLASS_TYPE((klass), GTKLTRFAMILY_TYPE)

#define LTRFAM_LV_CAPTION_SEQID "SeqID"
#define LTRFAM_LV_CAPTION_TYPE  "S"
#define LTRFAM_LV_CAPTION_START "Start"
#define LTRFAM_LV_CAPTION_END   "End"
#define LTRFAM_TV_CAPTION_INFO  "Attributes"

#define ATTR_RID       "ID"
#define ATTR_CLUSTID   "clid"
#define ATTR_PFAMN     "pfamname"
#define FNT_PROTEINM   "protein_match"
#define FNT_LTR        "long_terminal_repeat"
#define FNT_REPEATR    "repeat_region"
#define FNT_LLTR       "lLTR"
#define FNT_RLTR       "rLTR"
#define LTRFAM_DETINFO "Detailed Information"
#define LTRFAM_IMAGE   "Image"

typedef enum {
  LTRFAM_LV_NODE = 0,
  LTRFAM_LV_FEAT,
  LTRFAM_LV_ROWREF,
  LTRFAM_LV_SEQID,
  LTRFAM_LV_TYPE,
  LTRFAM_LV_START,
  LTRFAM_LV_END,
  LTRFAM_LV_N_COLUMS
} LTRFamListViewColumns;

typedef enum {
  LTRFAM_TV_NODE = 0,
  LTRFAM_TV_TYPE,
  LTRFAM_TV_START,
  LTRFAM_TV_END,
  LTRFAM_TV_INFO,
  LTRFAM_TV_N_COLUMS
} LTRFamTreeViewColumns;

typedef struct _GtkLTRFamily       GtkLTRFamily;
typedef struct _GtkLTRFamilyClass  GtkLTRFamilyClass;

struct _GtkLTRFamily
{
    GtkVPaned vpane;
    GtkWidget *tree_view;
    GtkWidget *list_view; 
    GtkWidget *image_area;
    GtDiagram *diagram;
    GtStyle *style;
};

struct _GtkLTRFamilyClass
{
    GtkVPanedClass parent_class;
    void (* gtk_ltr_family) (GtkLTRFamily *ltrfam);
};

GType          gtk_ltr_family_get_type        (void);
GtkWidget*     gtk_ltr_family_new             (GtHashmap *features,
                                               GtArray *nodes,
                                               unsigned long noc);
void           gtk_ltr_family_clear           (GtkLTRFamily *ltrfam);

GtkWidget* gtk_ltr_family_get_list_view(GtkLTRFamily *ltrfam);

void gtk_ltr_family_list_view_append(GtkLTRFamily *ltrfam,
                                     GtGenomeNode **gn,
                                     GtHashmap *features,
                                     GtkTreeRowReference *rowref,
                                     GtkListStore *store);

void gtk_ltr_family_list_view_remove(GtkLTRFamily *ltrfam,
                                     GtkTreeRowReference *rowref);

void gtk_ltr_family_clear_tree_view(GtkLTRFamily *ltrfam);

void gtk_ltr_family_clear_image(GtkLTRFamily *ltrfam);

void gtk_ltr_family_clear_detail_on_equal_nodes(GtkLTRFamily *ltrfam,
                                                GtGenomeNode **gn);

gboolean pane_moved(GtkPaned *pane, GtkScrollType scrolltype,
                    gpointer user_data);

gboolean pane_move_accept(GtkPaned *pane, gpointer user_data);

#endif /* __GTK_LTR_FAMILY_H__ */
