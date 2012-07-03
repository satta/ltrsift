/*
  Copyright (c) 2012 Sascha Kastens <sascha.kastens@studium.uni-hamburg.de>
  Copyright (c) 2012 Center for Bioinformatics, University of Hamburg

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GTK_BLASTN_PARAMS_REFSEQ_H
#define GTK_BLASTN_PARAMS_REFSEQ_H

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include "genometools.h"
#include "gtk_blastn_params.h"

#define GTK_BLASTN_PARAMS_REFSEQ_TYPE\
        gtk_blastn_params_refseq_get_type()
#define GTK_BLASTN_PARAMS_REFSEQ(obj)\
        G_TYPE_CHECK_INSTANCE_CAST((obj), GTK_BLASTN_PARAMS_REFSEQ_TYPE,\
        GtkBlastnParamsRefseq)
#define GTK_BLASTN_PARAMS_REFSEQ_CLASS(klass)\
        G_TYPE_CHECK_CLASS_CAST((klass), GTK_BLASTN_PARAMS_REFSEQ_TYPE,\
        GtkBlastnParamsRefseqClass)
#define IS_GTK_BLASTN_PARAMS_REFSEQ(obj)\
        G_TYPE_CHECK_INSTANCE_TYPE((obj), GTK_BLASTN_PARAMS_REFSEQ_TYPE)
#define IS_GTK_BLASTN_PARAMS_REFSEQ_CLASS(klass)\
        G_TYPE_CHECK_CLASS_TYPE((klass), GTK_BLASTN_PARAMS_REFSEQ_TYPE)

typedef struct _GtkBlastnParamsRefseq GtkBlastnParamsRefseq;
typedef struct _GtkBlastnParamsRefseqClass GtkBlastnParamsRefseqClass;

struct _GtkBlastnParamsRefseq
{
  GtkBlastnParams blastn_params;
  GtkWidget *label_refseq;
  GtkWidget *spinb_mlen;
  GtkWidget *checkb_flcands;
  GtkWidget *extra_widget;
  gchar *last_dir;
};

struct _GtkBlastnParamsRefseqClass
{
  GtkBlastnParamsClass parent_class;
  void (* gtk_blastn_params_refseq) (GtkBlastnParamsRefseq *blastparref);
};

GType        gtk_blastn_params_refseq_get_type(void);

const gchar* gtk_blastn_params_refseq_get_refseq_file(GtkBlastnParamsRefseq*);

gdouble      gtk_blastn_params_refseq_get_match_len(GtkBlastnParamsRefseq*);

gboolean     gtk_blastn_params_refseq_get_flcands(GtkBlastnParamsRefseq*);

void         gtk_blastn_params_refseq_set_extra_widget(GtkBlastnParamsRefseq*,
                                                       GtkWidget*, gint);

void         gtk_blastn_params_refseq_set_sensitive(GtkBlastnParamsRefseq*);

void         gtk_blastn_params_refseq_unset_sensitive(GtkBlastnParamsRefseq*);

void         gtk_blastn_params_refseq_set_paramset(GtkBlastnParamsRefseq *blast,
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
                                                   const gchar *moreblast,
                                                   gdouble mlen);

GtkWidget*   gtk_blastn_params_refseq_new();

#endif
