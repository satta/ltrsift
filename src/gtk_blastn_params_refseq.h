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

#define FAS_FILTER_PATTERN "*.fas"
#define FASTA_FILTER_PATTERN "*.fasta"
#define FASTA_FILTER_NAME "FASTA files"

typedef struct _GtkBlastnParamsRefseq GtkBlastnParamsRefseq;
typedef struct _GtkBlastnParamsRefseqClass GtkBlastnParamsRefseqClass;

struct _GtkBlastnParamsRefseq
{
  GtkBlastnParams blastn_params;
  GtkWidget *label_refseq;
  GtkWidget *spinb_mlen;
  GtkWidget *checkb_flcands;
  gchar *last_dir;
};

struct _GtkBlastnParamsRefseqClass
{
  GtkBlastnParamsClass parent_class;
  void (* gtk_blastn_params_refseq) (GtkBlastnParamsRefseq *blastparref);
};

GType        gtk_blastn_params_refseq_get_type(void);

const gchar* gtk_blastn_params_refseq_get_refseq_file(GtkBlastnParamsRefseq*);

gint         gtk_blastn_params_refseq_get_match_len(GtkBlastnParamsRefseq*);

gboolean     gtk_blastn_params_refseq_get_flcands(GtkBlastnParamsRefseq*);

GtkWidget*   gtk_blastn_params_refseq_new();

#endif
