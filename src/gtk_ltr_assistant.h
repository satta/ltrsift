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

#ifndef __GTK_LTR_ASSISTANT_H__
#define __GTK_LTR_ASSISTANT_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include "genometools.h"

#define GTK_LTR_ASSISTANT_TYPE\
        gtk_ltr_assistant_get_type()
#define GTK_LTR_ASSISTANT(obj)\
        G_TYPE_CHECK_INSTANCE_CAST((obj), GTK_LTR_ASSISTANT_TYPE,\
                                   GtkLTRAssistant)
#define GTK_LTR_ASSISTANT_CLASS(klass)\
        G_TYPE_CHECK_CLASS_CAST((klass), GTK_LTR_ASSISTANT_TYPE,\
                                GtkLTRAssistantClass)
#define IS_GTK_LTR_ASSISTANT(obj)\
        G_TYPE_CHECK_INSTANCE_TYPE((obj), GTK_LTR_ASSISTANT_TYPE)
#define IS_GTK_LTR_ASSISTANT_CLASS(klass)\
        G_TYPE_CHECK_CLASS_TYPE((klass), GTK_LTR_ASSISTANT_TYPE)

#define SELECT_GFF3_FILES "Select GFF3 files..."

#define GFF3_PATTERN   "*.gff3"
#define BROWSE_PROJECT "Bro_wse..."
#define BROWSE_INDEX   "Brow_se..."

typedef struct _GtkLTRAssistant GtkLTRAssistant;
typedef struct _GtkLTRAssistantClass GtkLTRAssistantClass;

typedef enum {
  PAGE_INTRODUCTION = 0,
  PAGE_GENERAL,
  PAGE_CLUSTERING,
  PAGE_CLASSIFICATION,
  PAGE_SUMMARY,
  N_PAGES
} LTRAssistantPages;

struct _GtkLTRAssistant
{
  GtkAssistant assistant;
  /* General settings page */
  GtkWidget *label_projectfile;
  GtkWidget *list_view_gff3files;
  GtkWidget *label_indexname;
  GtkWidget *checkb_clustering;
  /* Matching/Clustering settings page */
  GtkWidget *spinb_evalue;
  GtkWidget *checkb_dust;
  GtkWidget *spinb_gapopen;
  GtkWidget *spinb_gapextend;
  GtkWidget *spinb_xdrop;
  GtkWidget *spinb_penalty;
  GtkWidget *spinb_reward;
  GtkWidget *spinb_threads;
  GtkWidget *spinb_words;
  GtkWidget *spinb_seqid;
  GtkWidget *spinb_psmall;
  GtkWidget *spinb_plarge;
  GtkWidget *checkb_classification;
  /* Classification settings page */
  GtkWidget *spinb_ltrtol;
  GtkWidget *spinb_candtol;
  GtkWidget *list_view_features;
  /* Overview page */
  GtkWidget *notebook;
  GtkWidget *label_projectfile2;
  GtkWidget *label_gff3files;
  GtkWidget *label_indexname2;
  GtkWidget *label_doclustering;
  GtkWidget *label_evalue;
  GtkWidget *label_dust;
  GtkWidget *label_gapopen;
  GtkWidget *label_gapextend;
  GtkWidget *label_xdrop;
  GtkWidget *label_penalty;
  GtkWidget *label_reward;
  GtkWidget *label_threads;
  GtkWidget *label_wordsize;
  GtkWidget *label_seqidentity;
  GtkWidget *label_psmall;
  GtkWidget *label_plarge;
  GtkWidget *label_doclassification;
  GtkWidget *label_ltrtolerance;
  GtkWidget *label_candtolerance;
  GtkWidget *label_usedfeatures;
};

struct _GtkLTRAssistantClass
{
  GtkAssistantClass parent_class;
  void (* gtk_ltr_assistant) (GtkLTRAssistant *ltrassi);
};

GType gtk_ltr_assistant_get_type(void);

GtkWidget* gtk_ltr_assistant_new();

const gchar* gtk_ltr_assistant_get_projectfile(GtkLTRAssistant *ltrassi);

GtkTreeView* gtk_ltr_assistant_get_list_view_gff3files(GtkLTRAssistant *ltrassi);

const gchar* gtk_ltr_assistant_get_indexname(GtkLTRAssistant *ltrassi);

gboolean gtk_ltr_assistant_get_clustering(GtkLTRAssistant *ltrassi);

gdouble gtk_ltr_assistant_get_evalue(GtkLTRAssistant *ltrassi);

gboolean gtk_ltr_assistant_get_dust(GtkLTRAssistant *ltrassi);

gint gtk_ltr_assistant_get_gapopen(GtkLTRAssistant *ltrassi);

gint gtk_ltr_assistant_get_gapextend(GtkLTRAssistant *ltrassi);

gdouble gtk_ltr_assistant_get_xdrop(GtkLTRAssistant *ltrassi);

gint gtk_ltr_assistant_get_penalty(GtkLTRAssistant *ltrassi);

gint gtk_ltr_assistant_get_reward(GtkLTRAssistant *ltrassi);

gint gtk_ltr_assistant_get_threads(GtkLTRAssistant *ltrassi);

gint gtk_ltr_assistant_get_wordsize(GtkLTRAssistant *ltrassi);

gdouble gtk_ltr_assistant_get_seqid(GtkLTRAssistant *ltrassi);

gint gtk_ltr_assistant_get_psmall(GtkLTRAssistant *ltrassi);

gint gtk_ltr_assistant_get_plarge(GtkLTRAssistant *ltrassi);

gboolean gtk_ltr_assistant_get_classification(GtkLTRAssistant *ltrassi);

gdouble gtk_ltr_assistant_get_ltrtol(GtkLTRAssistant *ltrassi);

gdouble gtk_ltr_assistant_get_candtol(GtkLTRAssistant *ltrassi);

GtkTreeView* gtk_ltr_assistant_get_list_view_features(GtkLTRAssistant *ltrassi);

#endif /* __GTK_LTR_ASSISTANT_H__ */
