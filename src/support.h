/*
  Copyright (c) 2011-2012 Sascha Kastens <sascha.kastens@studium.uni-hamburg.de>
  Copyright (c) 2011-2012 Center for Bioinformatics, University of Hamburg

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

#ifndef SUPPORT_H
#define SUPPORT_H

#include "ltrsift.h"

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
          *new_nodes,
          *regions;
  GtError *err;
  GtHashmap *sel_features,
            *features;
  GtRDB *rdb;
  GtAnnoDBSchema *adb;
  GtFeatureIndex *fi;
  gboolean classification,
           projectw,
           save,
           save_as,
           open,
           orf,
           match,
           bakfile,
           use_paramset;
  gchar *current_state,
        *filename,
        *tmp_filename,
        *projectfile,
        *gff3file,
        *projectdir,
        *fam_prefix;
  const gchar *fullname;
  gfloat ltrtolerance,
         lentolerance;
  int had_err;
  unsigned long progress,
                n_features,
                set_id;
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

void          export_annotation(GtArray *nodes, GtArray *regions, gchar *filen, gboolean flcands,
                                GtkWidget *toplevel);

void          export_sequences(GtArray *nodes, gchar *filen,
                               const gchar *indexname, gboolean flcands,
                               GtkWidget *toplevel);

void          remove_node_from_array(GtArray *nodes, GtGenomeNode *gn);

void          remove_row(GtkTreeRowReference *rowref);

unsigned long determine_full_length_candidates(GtArray *nodes,
                                               gfloat ltrtolerance,
                                               gfloat lentolerance);

gchar*        double_underscores(const gchar *str);

#endif
