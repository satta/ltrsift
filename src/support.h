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

#ifndef SUPPORT_H
#define SUPPORT_H

#include "ltrgui.h"

typedef struct _ThreadData    ThreadData;
typedef struct _CandidateData CandidateData;

struct _ThreadData {
  GUIData *ltrgui;
  GtkLTRFamilies *ltrfams;
  GtkWidget *window,
            *progressbar,
            *dialog,
            *blastn_refseq;
  GtkTreeView *list_view;
  GList *references;
  GtArray *nodes,
          *old_nodes,
          *new_nodes;
  GtError *err;
  GtHashmap *sel_features,
            *features;
  gboolean classification,
           projectw,
           save,
           save_as,
           open,
           bakfile;
  gchar *current_state,
        *filename,
        *tmp_filename,
        *projectfile,
        *projectdir,
        *fam_prefix;
  const gchar *fullname;
  gfloat ltrtolerance,
         lentolerance;
  int had_err;
  unsigned long progress,
                n_features,
                last_id;
};

struct _CandidateData
{
  GtkTreeRowReference *fam_ref,
                      *cand_ref;
};

void          delete_gt_genome_node(GtGenomeNode *gn);

void          free_gt_hash_elem(void *elem);

gboolean      entry_in_list_view(GtkTreeModel *model, const gchar *entry,
                                 gint column_no);

void          create_recently_used_resource(const gchar *filename);

void          reset_progressbar(GtkWidget *progressbar);

GtkWidget*    unsaved_changes_dialog(GUIData *ltrgui, const gchar *text);

void          progress_dialog_init(ThreadData *threaddata, GtkWidget *toplevel);

void          threaddata_delete(ThreadData *threaddata);

ThreadData*   threaddata_new();

void          extract_project_settings(GUIData *ltrgui);

GtArray*      create_region_nodes_from_node_array(GtArray *nodes);

void          export_annotation(GtArray *nodes, gchar *filen, gboolean flcands,
                                GtkWidget *toplevel, GError *err);

void          export_sequences(GtArray *nodes, gchar *filen,
                               const gchar *indexname, gboolean flcands,
                               GtkWidget *toplevel, GError *err);

void          remove_node_from_array(GtArray *nodes, GtGenomeNode *gn);

void          remove_row(GtkTreeRowReference *rowref);

unsigned long determine_full_length_candidates(GtArray *nodes,
                                               gfloat ltrtolerance,
                                               gfloat lentolerance);

#endif
