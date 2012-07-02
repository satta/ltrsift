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

#ifndef GTK_LTR_ASSISTANT_H
#define GTK_LTR_ASSISTANT_H

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

typedef struct _GtkLTRAssistant GtkLTRAssistant;
typedef struct _GtkLTRAssistantClass GtkLTRAssistantClass;

typedef enum {
  /* PAGE_INTRODUCTION, */
  PAGE_GENERAL = 0,
  PAGE_CLUSTERING,
  PAGE_CLASSIFICATION,
  PAGE_SUMMARY,
  N_PAGES
} GtkLTRAssistantPages;

struct _GtkLTRAssistant
{
  GtkAssistant assistant;
  /* General settings page */
  GtkWidget *label_projectfile;
  GtkWidget *list_view_gff3files;
  GtkWidget *label_indexname;
  GtkWidget *checkb_clustering;
  /* Matching/Clustering settings page */
  GtkWidget *spinb_matchscore;
  GtkWidget *checkb_matchscore;
  GtkWidget *spinb_mismatchcost;
  GtkWidget *checkb_mismatchcost;
  GtkWidget *spinb_gapopen;
  GtkWidget *checkb_gapopen;
  GtkWidget *spinb_gapextend;
  GtkWidget *checkb_gapextend;
  GtkWidget *spinb_xgapped;
  GtkWidget *checkb_xgapped;
  GtkWidget *spinb_xgapless;
  GtkWidget *checkb_xgapless;
  GtkWidget *spinb_xfinal;
  GtkWidget *checkb_xfinal;
  GtkWidget *spinb_mscoregapless;
  GtkWidget *checkb_mscoregapless;
  GtkWidget *spinb_mscoregapped;
  GtkWidget *checkb_mscoregapped;
  GtkWidget *spinb_stepsize;
  GtkWidget *checkb_stepsize;
  GtkWidget *combob_lastparams;
  GtkWidget *entry_lastvalues;
  GtkWidget *button_addlastparam;
  GtkWidget *button_rmlastparam;
  GtkWidget *label_morelast;
  GtkWidget *spinb_psmall;
  GtkWidget *spinb_plarge;
  GtkWidget *checkb_classification;
  /* Classification settings page */
  GtkWidget *spinb_ltrtol;
  GtkWidget *spinb_candtol;
  GtkWidget *list_view_features;
  GtkWidget *entry_famprefix;
  /* Overview page */
  GtkWidget *notebook;
  GtkWidget *label_projectfile2;
  GtkWidget *label_gff3files;
  GtkWidget *label_indexname2;
  GtkWidget *label_doclustering;
  GtkWidget *label_gapopen;
  GtkWidget *label_gapextend;
  GtkWidget *label_xgapped;
  GtkWidget *label_xgapless;
  GtkWidget *label_xfinal;
  GtkWidget *label_mscoregapped;
  GtkWidget *label_mscoregapless;
  GtkWidget *label_matchscore;
  GtkWidget *label_mismatchcost;
  GtkWidget *label_stepsize;
  GtkWidget *label_morelast2;
  GtkWidget *label_psmall;
  GtkWidget *label_plarge;
  GtkWidget *label_doclassification;
  GtkWidget *label_ltrtolerance;
  GtkWidget *label_candtolerance;
  GtkWidget *label_famprefix;
  GtkWidget *label_usedfeatures;
  /* misc */
  gchar *last_dir;
  gboolean added_features;
  GHashTable *hasht_lastparams;
};

struct _GtkLTRAssistantClass
{
  GtkAssistantClass parent_class;
  void (* gtk_ltr_assistant) (GtkLTRAssistant *ltrassi);
};

GType        gtk_ltr_assistant_get_type(void);

const gchar* gtk_ltr_assistant_get_projectfile(GtkLTRAssistant *ltrassi);

GtkTreeView* gtk_ltr_assistant_get_list_view_gff3files( GtkLTRAssistant *ltra);

const gchar* gtk_ltr_assistant_get_indexname(GtkLTRAssistant *ltrassi);

gboolean     gtk_ltr_assistant_get_clustering(GtkLTRAssistant *ltrassi);

gint         gtk_ltr_assistant_get_xgapless(GtkLTRAssistant *ltrassi);

gint         gtk_ltr_assistant_get_xgapped(GtkLTRAssistant *ltrassi);

gint         gtk_ltr_assistant_get_xfinal(GtkLTRAssistant *ltrassi);

gint         gtk_ltr_assistant_get_matchscore(GtkLTRAssistant *ltrassi);

gint         gtk_ltr_assistant_get_mscoregapped(GtkLTRAssistant *ltrassi);

gint         gtk_ltr_assistant_get_mscoregapless(GtkLTRAssistant *ltrassi);

gint         gtk_ltr_assistant_get_mismatchcost(GtkLTRAssistant *ltrassi);

gint         gtk_ltr_assistant_get_gapopen(GtkLTRAssistant *ltrassi);

gint         gtk_ltr_assistant_get_gapextend(GtkLTRAssistant *ltrassi);

gdouble      gtk_ltr_assistant_get_seqid(GtkLTRAssistant *ltrassi);

const gchar* gtk_ltr_assistant_get_morelast(GtkLTRAssistant *ltrassi);

gint         gtk_ltr_assistant_get_stepsize(GtkLTRAssistant *ltrassi);

gint         gtk_ltr_assistant_get_psmall(GtkLTRAssistant *ltrassi);

gint         gtk_ltr_assistant_get_plarge(GtkLTRAssistant *ltrassi);

gboolean     gtk_ltr_assistant_get_classification(GtkLTRAssistant *ltrassi);

gdouble      gtk_ltr_assistant_get_ltrtol(GtkLTRAssistant *ltrassi);

gdouble      gtk_ltr_assistant_get_lentol(GtkLTRAssistant *ltrassi);

const gchar* gtk_ltr_assistant_get_fam_prefix(GtkLTRAssistant *ltrassi);

GtkTreeView* gtk_ltr_assistant_get_list_view_features(GtkLTRAssistant *ltrassi);

gchar*       gtk_ltr_assistant_get_match_params(GtkLTRAssistant *ltrassi);

GtkWidget*   gtk_ltr_assistant_new();

#endif
