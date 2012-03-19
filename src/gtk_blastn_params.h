/*
  Copyright (c) 2012 Sascha Kastens <sascha.kastens@studium.uni-hamburg.de>
  Copyright (c) 2012 Center for Bioinformatics, University of Hamburg

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

#ifndef GTK_BLASTN_PARAMS_H
#define GTK_BLASTN_PARAMS_H

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include "genometools.h"

#define GTK_BLASTN_PARAMS_TYPE\
        gtk_blastn_params_get_type()
#define GTK_BLASTN_PARAMS(obj)\
        G_TYPE_CHECK_INSTANCE_CAST((obj), GTK_BLASTN_PARAMS_TYPE,\
        GtkBlastnParams)
#define GTK_BLASTN_PARAMS_CLASS(klass)\
        G_TYPE_CHECK_CLASS_CAST((klass), GTK_BLASTN_PARAMS_TYPE,\
        GtkBlastnParamsClass)
#define IS_GTK_BLASTN_PARAMS(obj)\
        G_TYPE_CHECK_INSTANCE_TYPE((obj), GTK_BLASTN_PARAMS_TYPE)
#define IS_GTK_BLASTN_PARAMS_CLASS(klass)\
        G_TYPE_CHECK_CLASS_TYPE((klass), GTK_BLASTN_PARAMS_TYPE)

typedef struct _GtkBlastnParams GtkBlastnParams;
typedef struct _GtkBlastnParamsClass GtkBlastnParamsClass;

struct _GtkBlastnParams
{
  GtkVBox vbox;
  GtkWidget *spinb_evalue;
  GtkWidget *checkb_evalue;
  GtkWidget *checkb_dust;
  GtkWidget *spinb_gapopen;
  GtkWidget *checkb_gapopen;
  GtkWidget *spinb_gapextend;
  GtkWidget *checkb_gapextend;
  GtkWidget *spinb_xdrop;
  GtkWidget *checkb_xdrop;
  GtkWidget *spinb_penalty;
  GtkWidget *checkb_penalty;
  GtkWidget *spinb_reward;
  GtkWidget *checkb_reward;
  GtkWidget *spinb_threads;
  GtkWidget *checkb_threads;
  GtkWidget *spinb_words;
  GtkWidget *checkb_words;
  GtkWidget *spinb_seqid;
  GtkWidget *checkb_seqid;
  GtkWidget *combob_blastparams;
  GtkWidget *entry_blastvalues;
  GtkWidget *button_addblastparam;
  GtkWidget *button_rmblastparam;
  GtkWidget *label_moreblast;
  GtkWidget *extra_widget;
  GHashTable *hasht_blastparams;
};

struct _GtkBlastnParamsClass
{
  GtkVBoxClass parent_class;
  void (* gtk_blastn_params) (GtkBlastnParams *blastpar);
};

GType        gtk_blastn_params_get_type(void);

void         gtk_blastn_params_set_extra_widget(GtkBlastnParams *blastpar,
                                                GtkWidget *widget);

GtkWidget*   gtk_blastn_params_get_extra_widget(GtkBlastnParams *blastpar);

gdouble      gtk_blastn_params_get_evalue(GtkBlastnParams *blastpar);

gboolean     gtk_blastn_params_get_dust(GtkBlastnParams *blastpar);

gint         gtk_blastn_params_get_gapopen(GtkBlastnParams *blastpar);

gint         gtk_blastn_params_get_gapextend(GtkBlastnParams *blastpar);

gdouble      gtk_blastn_params_get_xdrop(GtkBlastnParams *blastpar);

gint         gtk_blastn_params_get_penalty(GtkBlastnParams *blastpar);

gint         gtk_blastn_params_get_reward(GtkBlastnParams *blastpar);

gint         gtk_blastn_params_get_threads(GtkBlastnParams *blastpar);

gint         gtk_blastn_params_get_wordsize(GtkBlastnParams *blastpar);

gdouble      gtk_blastn_params_get_seqid(GtkBlastnParams *blastpar);

const gchar* gtk_blastn_params_get_moreblast(GtkBlastnParams *blastpar);

void         gtk_blastn_params_set_sensitive(GtkBlastnParams *blastpar);

void         gtk_blastn_params_unset_sensitive(GtkBlastnParams *blastpar);

void         gtk_blastn_params_set_paramset(GtkBlastnParams *blastpar,
                                            gdouble evalue,
                                            gboolean dust,
                                            gint gapopen,
                                            gint gapextend,
                                            gdouble xdrop,
                                            gint penalty,
                                            gint reward,
                                            gint threads,
                                            gint wordsize,
                                            gdouble seqid,
                                            const gchar *moreblast);

GtkWidget*   gtk_blastn_params_new();

#endif
