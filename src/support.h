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

#define FILE_EXISTS_DIALOG "A file named \"%s\" already exists.\n\nDo you "\
                           "want to replace it?"

#define DEFAULT_STYLE  "../style/default.style"
#define GUI_NAME       "LTRGui"
#define FAS_PATTERN    ".fas"
#define GFF3_PATTERN   ".gff3"
#define ESQ_PATTERN   ".esq"
#define SQLITE_PATTERN ".sqlite"

#define GFF3_FILTER_PATTERN "*.gff3"
#define ESQ_FILTER_PATTERN  "*.esq"

#define NO_INDEX_DIALOG    "No indexname found. Do you want to select the "\
                           "indename?"
#define SELECT_INDEX       "Select indexname..."
#define EXPORT_ANNO_TEXT   "Export annotation..."
#define EXPORT_SEQS_TEXT   "Export sequences..."
#define CHOOSE_FILEN_ANNO  "Choose filename for annotation file..."
#define CHOOSE_FILEN_SEQS  "Choose filename for sequence file..."
#define CHOOSE_PREFIX_ANNO "Choose prefix for annotation files..."
#define CHOOSE_PREFIX_SEQS "Choose prefix for sequence files..."

#define ATTR_RID       "ID"
#define ATTR_CLUSTID   "clid"
#define ATTR_PFAMN     "name"
#define ATTR_FULLLEN   "flcand"
#define ATTR_LTRFAM   "ltrfam"
#define FNT_PROTEINM   "protein_match"
#define FNT_LTR        "long_terminal_repeat"
#define FNT_REPEATR    "repeat_region"
#define FNT_LLTR       "lLTR"
#define FNT_RLTR       "rLTR"
#define FNT_LTRRETRO   "LTR_retrotransposon"

#define LTR_TOLERANCE  "Allowed LTR length deviation from group median:"
#define LEN_TOLERANCE  "Allowed candidate length deviation from group median:"
#define FLCAND_RESULT  "Found %lu full length candidates (marked with *)"

typedef struct _ThreadData    ThreadData;
typedef struct _CandidateData CandidateData;

struct _ThreadData {
  GUIData *ltrgui;
  GtkLTRFamilies *ltrfams;
  GtkWidget *window,
            *progressbar;
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
                n_features;
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
