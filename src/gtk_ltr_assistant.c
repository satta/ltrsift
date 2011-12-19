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

#include <string.h>
#include "gtk_ltr_assistant.h"
#include "support.h"

typedef struct {
GtkWidget *widget;
gint index;
const gchar *title;
GtkAssistantPageType type;
gboolean complete;
} LTRAssistantPageInfo;

const gchar* gtk_ltr_assistant_get_projectfile(GtkLTRAssistant *ltrassi)
{
  return gtk_label_get_text(GTK_LABEL(ltrassi->label_projectfile));
}

GtkTreeView* gtk_ltr_assistant_get_list_view_gff3files(GtkLTRAssistant *ltrassi)
{
  return GTK_TREE_VIEW(ltrassi->list_view_gff3files);
}

const gchar* gtk_ltr_assistant_get_indexname(GtkLTRAssistant *ltrassi)
{
  return gtk_label_get_text(GTK_LABEL(ltrassi->label_indexname));
}

gboolean gtk_ltr_assistant_get_clustering(GtkLTRAssistant *ltrassi)
{
  return
    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ltrassi->checkb_clustering));
}

gdouble gtk_ltr_assistant_get_evalue(GtkLTRAssistant *ltrassi)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ltrassi->checkb_evalue)))
    return GT_UNDEF_DOUBLE;
  return gtk_spin_button_get_value(GTK_SPIN_BUTTON(ltrassi->spinb_evalue));
}

gboolean gtk_ltr_assistant_get_dust(GtkLTRAssistant *ltrassi)
{
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ltrassi->checkb_dust));
}

gint gtk_ltr_assistant_get_gapopen(GtkLTRAssistant *ltrassi)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ltrassi->checkb_gapopen)))
    return GT_UNDEF_INT;
  return
      gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ltrassi->spinb_gapopen));
}

gint gtk_ltr_assistant_get_gapextend(GtkLTRAssistant *ltrassi)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(
                                     ltrassi->checkb_gapextend)))
    return GT_UNDEF_INT;
  return
    gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ltrassi->spinb_gapextend));
}

gdouble gtk_ltr_assistant_get_xdrop(GtkLTRAssistant *ltrassi)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ltrassi->checkb_xdrop)))
    return GT_UNDEF_DOUBLE;
  return gtk_spin_button_get_value(GTK_SPIN_BUTTON(ltrassi->spinb_xdrop));
}

gint gtk_ltr_assistant_get_penalty(GtkLTRAssistant *ltrassi)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ltrassi->checkb_penalty)))
    return GT_UNDEF_INT;
  return
      gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ltrassi->spinb_penalty));
}

gint gtk_ltr_assistant_get_reward(GtkLTRAssistant *ltrassi)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ltrassi->checkb_reward)))
    return GT_UNDEF_INT;
  return
       gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ltrassi->spinb_reward));
}

gint gtk_ltr_assistant_get_threads(GtkLTRAssistant *ltrassi)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ltrassi->checkb_threads)))
    return GT_UNDEF_INT;
  return
      gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ltrassi->spinb_threads));
}

gint gtk_ltr_assistant_get_wordsize(GtkLTRAssistant *ltrassi)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ltrassi->checkb_words)))
    return GT_UNDEF_INT;
  return
      gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ltrassi->spinb_words));
}

gdouble gtk_ltr_assistant_get_seqid(GtkLTRAssistant *ltrassi)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ltrassi->checkb_seqid)))
    return GT_UNDEF_DOUBLE;
  return gtk_spin_button_get_value(GTK_SPIN_BUTTON(ltrassi->spinb_seqid));
}

const gchar* gtk_ltr_assistant_get_moreblast(GtkLTRAssistant *ltrassi)
{
  return gtk_label_get_text(GTK_LABEL(ltrassi->label_moreblast));
}

gint gtk_ltr_assistant_get_psmall(GtkLTRAssistant *ltrassi)
{
  return
      gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ltrassi->spinb_psmall));
}

gint gtk_ltr_assistant_get_plarge(GtkLTRAssistant *ltrassi)
{
  return
      gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ltrassi->spinb_plarge));
}

gboolean gtk_ltr_assistant_get_classification(GtkLTRAssistant *ltrassi)
{
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(
                                        ltrassi->checkb_classification));
}

gdouble gtk_ltr_assistant_get_ltrtol(GtkLTRAssistant *ltrassi)
{
  return gtk_spin_button_get_value(GTK_SPIN_BUTTON(ltrassi->spinb_ltrtol));
}

gdouble gtk_ltr_assistant_get_lentol(GtkLTRAssistant *ltrassi)
{
  return gtk_spin_button_get_value(GTK_SPIN_BUTTON(ltrassi->spinb_candtol));
}

const gchar* gtk_ltr_assistant_get_fam_prefix(GtkLTRAssistant *ltrassi)
{
  return gtk_entry_get_text(GTK_ENTRY(ltrassi->entry_famprefix));
}

GtkTreeView* gtk_ltr_assistant_get_list_view_features(GtkLTRAssistant *ltrassi)
{
  return GTK_TREE_VIEW(ltrassi->list_view_features);
}

static void show_hide_tab(GtkNotebook *notebook, int pagenumber, gboolean show)
{
  GtkWidget *page;

  page = gtk_notebook_get_nth_page(notebook, pagenumber);
  show ? gtk_widget_show(page) : gtk_widget_hide(page);
}

static void check_complete_page_general(GtkLTRAssistant *ltrassi)
{
  GtkWidget *page;
  GtkTreeModel *model;
  GtkTreeIter iter;
  gboolean complete = TRUE;

  if (g_strcmp0(gtk_label_get_text(GTK_LABEL(ltrassi->label_projectfile)),
                "") == 0) {
    complete &= FALSE;
  }
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrassi->list_view_gff3files));
  complete &= gtk_tree_model_get_iter_first(model, &iter);
  if (g_strcmp0(gtk_label_get_text(GTK_LABEL(ltrassi->label_indexname)),
                "") == 0) {
    complete &= FALSE;
  }
  page = gtk_assistant_get_nth_page(GTK_ASSISTANT(ltrassi), PAGE_GENERAL);
  gtk_assistant_set_page_complete(GTK_ASSISTANT(ltrassi), page, complete);
}

static void check_complete_page_classification(GtkLTRAssistant *ltrassi)
{
  GtkWidget *page;
  GtkTreeSelection *sel;
  const gchar *text;
  gboolean complete = TRUE;

  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(ltrassi->list_view_features));
  if (gtk_tree_selection_count_selected_rows(sel) == 0)
    complete &= FALSE;
  text = gtk_entry_get_text(GTK_ENTRY(ltrassi->entry_famprefix));
  if (g_strcmp0(text, "") == 0)
    complete &= FALSE;

  page = gtk_assistant_get_nth_page(GTK_ASSISTANT(ltrassi),
                                    PAGE_CLASSIFICATION);
  gtk_assistant_set_page_complete(GTK_ASSISTANT(ltrassi), page, complete);
  gtk_label_set_text(GTK_LABEL(ltrassi->label_famprefix), text);
}

static int fill_feature_list(void *key, GT_UNUSED void *value,
                             void *lv, GT_UNUSED GtError *err)
{
  GtkTreeView *list_view;
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *feature = (gchar*) key;

  list_view = (GtkTreeView*) lv;
  model = gtk_tree_view_get_model(list_view);
  if ((g_strcmp0(feature, "repeat_region") != 0) &&
      (g_strcmp0(feature, "LTR_retrotransposon") != 0)) {
    gtk_list_store_append(GTK_LIST_STORE(model), &iter);
    gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                       0, feature,
                       -1);
  }
  return 0;
}

static void get_feature_list(GtkLTRAssistant *ltrassi)
{
  GtkTreeView *list_view;
  GtkTreeIter iter;
  GtkTreeSelection *sel;
  GtkTreeModel *model;
  GList *rows, *tmp;
  GtNodeStream *gff3_in_stream = NULL,
               *preprocess_stream = NULL;
  GtHashmap *features;
  GtError *err;
  const char **gff3_files;
  gint num_of_files, had_err = 0, i = 0;
  unsigned long n = 0;

  if (ltrassi->added_features)
    return;

  err = gt_error_new();
  list_view = GTK_TREE_VIEW(ltrassi->list_view_gff3files);
  sel = gtk_tree_view_get_selection(list_view);
  gtk_tree_selection_select_all(sel);
  num_of_files = gtk_tree_selection_count_selected_rows(sel);
  if (num_of_files == 0)
    return;
  rows = gtk_tree_selection_get_selected_rows(sel, &model);
  tmp = rows;
  gff3_files = g_malloc((size_t) num_of_files * sizeof (const char*));
  while (tmp != NULL) {
    gtk_tree_model_get_iter(model, &iter, (GtkTreePath*) tmp->data);
    gtk_tree_model_get(model, &iter,
                       0, &gff3_files[i],
                       -1);
    i++;
    tmp = tmp->next;
  }
  g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
  g_list_free(rows);

  gff3_in_stream = gt_gff3_in_stream_new_unsorted(num_of_files, gff3_files);
  features = gt_hashmap_new(GT_HASH_STRING, free_gt_hash_elem, NULL);
  preprocess_stream = ltrgui_preprocess_stream_new(gff3_in_stream,
                                                   features,
                                                   &n,
                                                   TRUE,
                                                   err);
  had_err = gt_node_stream_pull(preprocess_stream, err);

  gtk_list_store_clear(GTK_LIST_STORE(
                         gtk_tree_view_get_model(
                                        GTK_TREE_VIEW(
                                                ltrassi->list_view_features))));
  gt_hashmap_foreach(features, fill_feature_list,
                     (void*) ltrassi->list_view_features, err);

  gt_error_delete(err);
  gt_hashmap_delete(features);
  gt_node_stream_delete(gff3_in_stream);
  gt_node_stream_delete(preprocess_stream);
  for (i = 0; i < num_of_files; i++)
    g_free((gpointer) gff3_files[i]);
  g_free(gff3_files);
  ltrassi->added_features = TRUE;
}

static void remove_list_view_row(GtkTreeRowReference *rowref,
                                 GtkTreeModel *model)
{
  GtkTreeIter iter;
  GtkTreePath *path;

  path = gtk_tree_row_reference_get_path(rowref);
  gtk_tree_model_get_iter(model, &iter, path);

  gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
  gtk_tree_path_free(path);
}

static void update_cluster_overview(GtkLTRAssistant *ltrassi)
{
  gint psmall,
       plarge,
       gapopen,
       gapextend,
       wordsize,
       penalty,
       reward,
       num_threads;
  gboolean dust;
  gdouble evalue,
          seqid,
          xdrop;
  gchar buffer[BUFSIZ];

  evalue = gtk_ltr_assistant_get_evalue(ltrassi);
  dust =  gtk_ltr_assistant_get_dust(ltrassi);
  gapopen = gtk_ltr_assistant_get_gapopen(ltrassi);
  gapextend = gtk_ltr_assistant_get_gapextend(ltrassi);
  xdrop = gtk_ltr_assistant_get_xdrop(ltrassi);
  penalty = gtk_ltr_assistant_get_penalty(ltrassi);
  reward = gtk_ltr_assistant_get_reward(ltrassi);
  num_threads = gtk_ltr_assistant_get_threads(ltrassi);
  wordsize = gtk_ltr_assistant_get_wordsize(ltrassi);
  seqid = gtk_ltr_assistant_get_seqid(ltrassi);
  psmall = gtk_ltr_assistant_get_psmall(ltrassi);
  plarge = gtk_ltr_assistant_get_plarge(ltrassi);

  if (evalue == GT_UNDEF_DOUBLE)
    gtk_label_set_text(GTK_LABEL(ltrassi->label_evalue), USE_DEFAULT_A);
  else {
    g_snprintf(buffer, BUFSIZ, "%.6f", evalue);
    gtk_label_set_text(GTK_LABEL(ltrassi->label_evalue), buffer);
  }
  gtk_label_set_text(GTK_LABEL(ltrassi->label_dust), dust ? "Yes" : "No");
  if (gapopen == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(ltrassi->label_gapopen), USE_DEFAULT_A);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", gapopen);
    gtk_label_set_text(GTK_LABEL(ltrassi->label_gapopen), buffer);
  }
  if (gapextend == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(ltrassi->label_gapextend), USE_DEFAULT_A);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", gapextend);
    gtk_label_set_text(GTK_LABEL(ltrassi->label_gapextend), buffer);
  }
  if (xdrop == GT_UNDEF_DOUBLE)
    gtk_label_set_text(GTK_LABEL(ltrassi->label_xdrop), USE_DEFAULT_A);
  else {
    g_snprintf(buffer, BUFSIZ, "%.2f", xdrop);
    gtk_label_set_text(GTK_LABEL(ltrassi->label_xdrop), buffer);
  }
  if (penalty == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(ltrassi->label_penalty), USE_DEFAULT_A);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", penalty);
    gtk_label_set_text(GTK_LABEL(ltrassi->label_penalty), buffer);
  }
  if (reward == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(ltrassi->label_reward), USE_DEFAULT_A);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", reward);
    gtk_label_set_text(GTK_LABEL(ltrassi->label_reward), buffer);
  }
  if (num_threads == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(ltrassi->label_threads), USE_DEFAULT_A);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", num_threads);
    gtk_label_set_text(GTK_LABEL(ltrassi->label_threads), buffer);
  }
  if (wordsize == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(ltrassi->label_wordsize), USE_DEFAULT_A);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", wordsize);
    gtk_label_set_text(GTK_LABEL(ltrassi->label_wordsize), buffer);
  }
  if (seqid == GT_UNDEF_DOUBLE)
    gtk_label_set_text(GTK_LABEL(ltrassi->label_seqidentity), USE_DEFAULT_A);
  else {
    g_snprintf(buffer, BUFSIZ, "%.2f", seqid);
    gtk_label_set_text(GTK_LABEL(ltrassi->label_seqidentity), buffer);
  }
  g_snprintf(buffer, BUFSIZ, "%d", psmall);
  gtk_label_set_text(GTK_LABEL(ltrassi->label_psmall), buffer);
  g_snprintf(buffer, BUFSIZ, "%d", plarge);
  gtk_label_set_text(GTK_LABEL(ltrassi->label_plarge), buffer);
}

static void update_classification_overview(GtkLTRAssistant *ltrassi)
{
  gdouble ltrtol,
          candtol;
  gchar buffer[BUFSIZ];

  ltrtol = gtk_ltr_assistant_get_ltrtol(ltrassi);
  candtol = gtk_ltr_assistant_get_lentol(ltrassi);
  g_snprintf(buffer, BUFSIZ, "%.1f", ltrtol);
  gtk_label_set_text(GTK_LABEL(ltrassi->label_ltrtolerance), buffer);
  g_snprintf(buffer, BUFSIZ, "%.1f", candtol);
  gtk_label_set_text(GTK_LABEL(ltrassi->label_candtolerance), buffer);
}

static void
cluster_settings_dust_toggled(GT_UNUSED GtkToggleButton *togglebutton,
                              GtkLTRAssistant *ltrassi)
{
  update_cluster_overview(ltrassi);
}

static void
cluster_settings_adjustment_value_changed(GT_UNUSED GtkAdjustment *adj,
                                          GtkLTRAssistant *ltrassi)
{
  update_cluster_overview(ltrassi);
}

static void
cassification_settings_adjustment_value_changed(GT_UNUSED GtkAdjustment *adj,
                                                GtkLTRAssistant *ltrassi)
{
  update_classification_overview(ltrassi);
}

static void update_gff3_label(GtkTreeView *list_view, GtkLabel *gff3_label)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  gboolean valid;
  gchar *text,
        *old_text,
        *buffer;

  model = gtk_tree_view_get_model(list_view);
  valid = gtk_tree_model_get_iter_first(model, &iter);
  if (valid) {
    gtk_tree_model_get(model, &iter,
                       0, &buffer,
                       -1);
    text = g_path_get_basename(buffer);
    g_free(buffer);
    while (gtk_tree_model_iter_next(model, &iter)) {
      gtk_tree_model_get(model, &iter,
                         0, &buffer,
                         -1);
      old_text = g_strdup(text);
      g_free(text);
      text = g_strjoin("\n", old_text, g_path_get_basename(buffer), NULL);
      g_free(buffer);
      g_free(old_text);
    }
    gtk_label_set_label(gff3_label, text);
    g_free(text);
  } else
    gtk_label_set_label(gff3_label, "");
}

static void update_features_label(GtkTreeView *list_view,
                                  GtkLabel *feature_label)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreeSelection *sel;
  GList *rows, *tmp;
  gboolean first = TRUE;
  gchar *text,
        *old_text,
        *buffer;

  sel = gtk_tree_view_get_selection(list_view);
  if (gtk_tree_selection_count_selected_rows(sel) == 0) {
    gtk_label_set_label(feature_label, "");
    return;
  }
  rows = gtk_tree_selection_get_selected_rows(sel, &model);
  tmp = rows;
  while (tmp != NULL) {
    gtk_tree_model_get_iter(model, &iter, (GtkTreePath*) tmp->data);
    gtk_tree_model_get(model, &iter,
                       0, &buffer,
                       -1);
    if (first) {
      text = g_strdup(buffer);
      first = FALSE;
    } else {
      old_text = g_strdup(text);
      g_free(text);
      text = g_strjoin("\n", old_text, buffer, NULL);
      g_free(old_text);
    }
    g_free(buffer);
    tmp = tmp->next;
  }
  gtk_label_set_label(feature_label, text);
  g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
  g_list_free(rows);
  g_free(text);
}

void list_view_features_selection_changed(GT_UNUSED GtkTreeSelection *sel,
                                          GtkLTRAssistant *ltrassi)
{
  check_complete_page_classification(ltrassi);
  update_features_label(GTK_TREE_VIEW(ltrassi->list_view_features),
                        GTK_LABEL(ltrassi->label_usedfeatures));
}

static void remove_gff3_button_clicked(GT_UNUSED GtkButton *button,
                                       GtkLTRAssistant *ltrassi)
{
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  GtkTreeRowReference *rowref;
  GList *rows, *tmp, *references = NULL;

  sel =
       gtk_tree_view_get_selection(GTK_TREE_VIEW(ltrassi->list_view_gff3files));

  if (!gtk_tree_selection_count_selected_rows(sel))
    return;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrassi->list_view_gff3files));
  rows = gtk_tree_selection_get_selected_rows(sel, &model);

  tmp = rows;

  while (tmp != NULL) {
    rowref = gtk_tree_row_reference_new(model, (GtkTreePath*) tmp->data);
    references =
                g_list_prepend(references, gtk_tree_row_reference_copy(rowref));
    gtk_tree_row_reference_free(rowref);
    tmp = tmp->next;
  }

  g_list_foreach(references, (GFunc) remove_list_view_row, model);
  check_complete_page_general(ltrassi);
  update_gff3_label(GTK_TREE_VIEW(ltrassi->list_view_gff3files),
                    GTK_LABEL(ltrassi->label_gff3files));
  g_list_foreach(references, (GFunc) gtk_tree_row_reference_free, NULL);
  g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
  g_list_free(references);
  g_list_free(rows);
}

void add_gff3_button_clicked(GT_UNUSED GtkButton *button,
                             GtkLTRAssistant *ltrassi)
{
  GtkWidget *filechooser;
  GtkFileFilter *gff3_file_filter;
  GtkTreeIter iter;
  GtkTreeModel *model;
  GSList *filenames;

  filechooser = gtk_file_chooser_dialog_new(SELECT_GFF3_FILES,
                                            GTK_WINDOW(ltrassi),
                                            GTK_FILE_CHOOSER_ACTION_OPEN,
                                            GTK_STOCK_CANCEL,
                                            GTK_RESPONSE_CANCEL,
                                            GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                            NULL);

  gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(filechooser), TRUE);
  if (ltrassi->last_dir)
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(filechooser),
                                        ltrassi->last_dir);
  gff3_file_filter = gtk_file_filter_new();
  gtk_file_filter_set_name(gff3_file_filter, GFF3_FILTER_PATTERN);
  gtk_file_filter_add_pattern(gff3_file_filter, GFF3_FILTER_PATTERN);
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filechooser), gff3_file_filter);
  gint result = gtk_dialog_run(GTK_DIALOG(filechooser));

  if (result == GTK_RESPONSE_ACCEPT) {
    model =
           gtk_tree_view_get_model(GTK_TREE_VIEW(ltrassi->list_view_gff3files));
    filenames = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(filechooser));
    while (filenames != NULL) {
      gchar *file = (gchar*) filenames->data;

      if (!entry_in_list_view(model, file, 0)) {
        gtk_list_store_append(GTK_LIST_STORE(model), &iter);
        gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                           0, file,
                           -1);
        if (ltrassi->last_dir)
          g_free(ltrassi->last_dir);
        ltrassi->last_dir = g_path_get_dirname(file);
        ltrassi->added_features = FALSE;
      }

      filenames = filenames->next;
    }
    update_gff3_label(GTK_TREE_VIEW(ltrassi->list_view_gff3files),
                      GTK_LABEL(ltrassi->label_gff3files));
    g_slist_foreach(filenames, (GFunc) g_free, NULL);
    g_slist_free(filenames);
  }
  gtk_widget_destroy(filechooser);
  check_complete_page_general(ltrassi);
}

static void browse_button_clicked(GtkButton *button, GtkLTRAssistant *ltrassi)
{
  GtkWidget *filechooser;
  gchar *filename = NULL;

  filechooser = gtk_file_chooser_dialog_new("Select...",
                                            GTK_WINDOW(ltrassi),
                                            GTK_FILE_CHOOSER_ACTION_SAVE,
                                            GTK_STOCK_CANCEL,
                                            GTK_RESPONSE_CANCEL, GTK_STOCK_OK,
                                            GTK_RESPONSE_ACCEPT, NULL);
  if (ltrassi->last_dir)
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(filechooser),
                                        ltrassi->last_dir);
  if (g_strcmp0(gtk_button_get_label(button), BROWSE_PROJECT) != 0) {
    GtkFileFilter *esq_file_filter;
    esq_file_filter = gtk_file_filter_new();
    gtk_file_filter_set_name(esq_file_filter, ESQ_FILTER_PATTERN);
    gtk_file_filter_add_pattern(esq_file_filter, ESQ_FILTER_PATTERN);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filechooser), esq_file_filter);
    gtk_file_chooser_set_create_folders(GTK_FILE_CHOOSER(filechooser), FALSE);
  }
  if (gtk_dialog_run(GTK_DIALOG(filechooser)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser));
    if (g_strcmp0(gtk_button_get_label(button), BROWSE_PROJECT) == 0) {
      gtk_label_set_label(GTK_LABEL(ltrassi->label_projectfile), filename);
      gtk_label_set_label(GTK_LABEL(ltrassi->label_projectfile2),
                          g_path_get_basename(filename));
    } else {
      gchar *tmp = g_strndup(filename, strlen(filename) - 4);
      gtk_label_set_label(GTK_LABEL(ltrassi->label_indexname), tmp);
      gtk_label_set_label(GTK_LABEL(ltrassi->label_indexname2),
                          g_path_get_basename(tmp));
      g_free(tmp);
    }
    if (ltrassi->last_dir)
      g_free(ltrassi->last_dir);
    ltrassi->last_dir = g_path_get_dirname(filename);
    g_free(filename);
  }
  gtk_widget_destroy(filechooser);
  check_complete_page_general(ltrassi);
}

static void edit_general_settings_clicked(GT_UNUSED GtkButton *button,
                                                 GtkLTRAssistant *ltrassi)
{
  gtk_assistant_set_current_page(GTK_ASSISTANT(ltrassi), PAGE_GENERAL);
}

static void edit_clustering_settings_clicked(GT_UNUSED GtkButton *button,
                                                 GtkLTRAssistant *ltrassi)
{
  gtk_assistant_set_current_page(GTK_ASSISTANT(ltrassi), PAGE_CLUSTERING);
}

static void edit_classification_settings_clicked(GT_UNUSED GtkButton *button,
                                                 GtkLTRAssistant *ltrassi)
{
  gtk_assistant_set_current_page(GTK_ASSISTANT(ltrassi), PAGE_CLASSIFICATION);
}

static void checkb_clustering_toggled(GtkToggleButton *togglebutton,
                                      GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_label_set_label(GTK_LABEL(ltrassi->label_doclustering),
                      active ? "Yes" : "No");
  show_hide_tab(GTK_NOTEBOOK(ltrassi->notebook), 1, active);
  update_cluster_overview(ltrassi);
}

static void checkb_classification_toggled(GtkToggleButton *togglebutton,
                                   GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_label_set_label(GTK_LABEL(ltrassi->label_doclassification),
                      active ? "Yes" : "No");
  show_hide_tab(GTK_NOTEBOOK(ltrassi->notebook), 2, active);
  update_classification_overview(ltrassi);
}

static void checkb_evalue_toggled(GtkToggleButton *togglebutton,
                                   GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(ltrassi->spinb_evalue, !active);
  update_cluster_overview(ltrassi);
}

static void checkb_gapopen_toggled(GtkToggleButton *togglebutton,
                                   GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(ltrassi->spinb_gapopen, !active);
  update_cluster_overview(ltrassi);
}

static void checkb_gapextend_toggled(GtkToggleButton *togglebutton,
                                   GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(ltrassi->spinb_gapextend, !active);
  update_cluster_overview(ltrassi);
}

static void checkb_xdrop_toggled(GtkToggleButton *togglebutton,
                                 GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(ltrassi->spinb_xdrop, !active);
  update_cluster_overview(ltrassi);
}

static void checkb_penalty_toggled(GtkToggleButton *togglebutton,
                                   GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(ltrassi->spinb_penalty, !active);
  update_cluster_overview(ltrassi);
}

static void checkb_reward_toggled(GtkToggleButton *togglebutton,
                                   GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(ltrassi->spinb_reward, !active);
  update_cluster_overview(ltrassi);
}

static void checkb_threads_toggled(GtkToggleButton *togglebutton,
                                   GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(ltrassi->spinb_threads, !active);
  update_cluster_overview(ltrassi);
}

static void checkb_words_toggled(GtkToggleButton *togglebutton,
                                   GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(ltrassi->spinb_words, !active);
  update_cluster_overview(ltrassi);
}

static void checkb_seqid_toggled(GtkToggleButton *togglebutton,
                                   GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(ltrassi->spinb_seqid, !active);
  update_cluster_overview(ltrassi);
}

static gint compare_strings_from_list(gconstpointer a, gconstpointer b)
{
  const gchar *key1 = (const gchar*) a,
              *key2 = (const gchar*) b;
  return g_strcmp0(key1, key2);
}

gchar* gtk_ltr_assistant_get_match_params(GtkLTRAssistant *ltrassi)
{
  GList *keys;
  gint gapopen,
       gapextend,
       wordsize,
       penalty,
       reward,
       num_threads;
  gchar blast_call[BUFSIZ];
  gchar *moreblast = NULL;
  gboolean dust,
           first = TRUE;
  gdouble evalue,
          seqid,
          xdrop;

  evalue = gtk_ltr_assistant_get_evalue(ltrassi);
  dust =  gtk_ltr_assistant_get_dust(ltrassi);
  gapopen = gtk_ltr_assistant_get_gapopen(ltrassi);
  gapextend = gtk_ltr_assistant_get_gapextend(ltrassi);
  xdrop = gtk_ltr_assistant_get_xdrop(ltrassi);
  penalty = gtk_ltr_assistant_get_penalty(ltrassi);
  reward = gtk_ltr_assistant_get_reward(ltrassi);
  num_threads = gtk_ltr_assistant_get_threads(ltrassi);
  wordsize = gtk_ltr_assistant_get_wordsize(ltrassi);
  seqid = gtk_ltr_assistant_get_seqid(ltrassi);

  if (evalue != GT_UNDEF_DOUBLE)
    sprintf(blast_call, "%s -evalue %.4f", blast_call, evalue);
  if (dust)
    sprintf(blast_call, "%s -dust yes", blast_call);
  if (wordsize != GT_UNDEF_INT)
    sprintf(blast_call, "%s -word_size %d", blast_call, wordsize);
  if (gapopen != GT_UNDEF_INT)
    sprintf(blast_call, "%s -gapopen %d", blast_call, gapopen);
  if (gapextend != GT_UNDEF_INT)
    sprintf(blast_call, "%s -gapextend %d", blast_call, gapextend);
  if (penalty != GT_UNDEF_INT)
    sprintf(blast_call, "%s -penalty %d", blast_call, penalty);
  if (reward != GT_UNDEF_INT)
    sprintf(blast_call, "%s -reward %d", blast_call, reward);
  if (seqid != GT_UNDEF_DOUBLE)
    sprintf(blast_call, "%s -perc_identity %.2f", blast_call, seqid);
  if (num_threads != GT_UNDEF_INT)
    sprintf(blast_call, "%s -num_threads %d", blast_call, num_threads);
  if (xdrop != GT_UNDEF_DOUBLE)
    sprintf(blast_call, "%s -xdrop_gap_final %.2f", blast_call, xdrop);

  keys = g_hash_table_get_keys(ltrassi->hasht_blastparams);
  keys = g_list_sort(keys, compare_strings_from_list);
  while (keys != NULL) {
    gpointer key = keys->data;
    gpointer value;

    value = g_hash_table_lookup(ltrassi->hasht_blastparams, key);
    if (first) {
      moreblast = g_strdup_printf("%s %s", (gchar*) key, (gchar*) value);
      first = FALSE;
    } else
      moreblast = g_strjoin(" ", moreblast, (gchar*) key, (gchar*) value, NULL);
    keys = keys->next;
  }
  if (moreblast)
    sprintf(blast_call, "%s %s", blast_call, moreblast);

  /*g_free(moreblast);*/

  return g_strdup(blast_call);
}

static void build_string(gpointer k, gpointer v, gpointer data)
{
  GString *string = (GString*) data;
  gchar *key = (gchar*) k,
        *value = (gchar*) v,
        *buffer;

  g_string_append(string, " ");
  buffer = g_strjoin(" ", key, value, NULL);
  g_string_append(string, buffer);
  g_free(buffer);
}

static void update_label_moreblast(GtkLTRAssistant *ltrassi)
{
  GString *string;
  gchar *buffer;

  string = g_string_new("");

  g_hash_table_foreach(ltrassi->hasht_blastparams, (GHFunc) build_string,
                       (gpointer) string);
  buffer = g_string_free(string, FALSE);
  gtk_label_set_text(GTK_LABEL(ltrassi->label_moreblast), buffer);
  gtk_label_set_text(GTK_LABEL(ltrassi->label_moreblast2), buffer);
  g_free(buffer);
}

static void gtk_ltr_assistant_combob_blast_changed(GtkComboBox *combob,
                                                   GtkLTRAssistant *ltrassi)
{
  gchar *param;
  gpointer value;

  param = gtk_combo_box_get_active_text(combob);
  if (g_strcmp0(param, "") == 0) {
    gtk_widget_set_sensitive(ltrassi->entry_blastvalues, FALSE);
    gtk_widget_set_sensitive(ltrassi->button_addblastparam, FALSE);
    gtk_widget_set_sensitive(ltrassi->button_rmblastparam, FALSE);
    g_free(param);
    return;
  }
  gtk_widget_set_sensitive(ltrassi->entry_blastvalues, TRUE);
  gtk_widget_set_sensitive(ltrassi->button_addblastparam, TRUE);
  gtk_widget_set_sensitive(ltrassi->button_rmblastparam, TRUE);

  value = g_hash_table_lookup(ltrassi->hasht_blastparams,
                              (gconstpointer) param);
  if (value)
    gtk_entry_set_text(GTK_ENTRY(ltrassi->entry_blastvalues), (gchar*) value);
  else
    gtk_entry_set_text(GTK_ENTRY(ltrassi->entry_blastvalues), "");
  g_free(param);
}

static void
gtk_ltr_assistant_add_blast_param_clicked(GT_UNUSED GtkButton *button,
                                          GtkLTRAssistant *ltrassi)
{
  gchar *key;
  const gchar *value;

  key =
      gtk_combo_box_get_active_text(GTK_COMBO_BOX(ltrassi->combob_blastparams));
  value = gtk_entry_get_text(GTK_ENTRY(ltrassi->entry_blastvalues));
  if ((g_strcmp0(key, BLASTN_NO_GREEDY) == 0) ||
      (g_strcmp0(key, BLASTN_UNGAPPED) == 0))
    value = "";
  else if (g_strcmp0(value, "") == 0) {
    g_free(key);
    return;
  }
  g_hash_table_insert(ltrassi->hasht_blastparams,
                      (gpointer) key, (gpointer) g_strdup(value));
  update_label_moreblast(ltrassi);
}

static void
gtk_ltr_assistant_rm_blast_param_clicked(GT_UNUSED GtkButton *button,
                                         GtkLTRAssistant *ltrassi)
{
  gchar *key;

  key =
      gtk_combo_box_get_active_text(GTK_COMBO_BOX(ltrassi->combob_blastparams));
  if (!g_hash_table_remove(ltrassi->hasht_blastparams, (gconstpointer) key)) {
    g_free(key);
    return;
  }
  gtk_entry_set_text(GTK_ENTRY(ltrassi->entry_blastvalues), "");
  update_label_moreblast(ltrassi);
  g_free(key);
}

static void gtk_ltr_assistant_fam_prefix_changed(GT_UNUSED GtkEditable *edit,
                                                 GtkLTRAssistant *ltrassi)
{
  check_complete_page_classification(ltrassi);
}

static gboolean gtk_ltr_assistant_destroy(GtkWidget *widget,
                                          GT_UNUSED GdkEvent *event,
                                          GT_UNUSED gpointer user_data)
{
  GtkLTRAssistant *ltrassi;
  ltrassi = GTK_LTR_ASSISTANT(widget);
  if (ltrassi->last_dir)
    g_free(ltrassi->last_dir);
  g_hash_table_destroy(ltrassi->hasht_blastparams);

  return FALSE;
}

static void gtk_ltr_assistant_init(GtkLTRAssistant *ltrassi)
{
  LTRAssistantPageInfo page_info[N_PAGES] = {
    {NULL, -1, "Introduction", GTK_ASSISTANT_PAGE_INTRO, TRUE},
    {NULL, -1, "General settings", GTK_ASSISTANT_PAGE_CONTENT, FALSE},
    {NULL, -1, "Matching/Clustering", GTK_ASSISTANT_PAGE_CONTENT, TRUE},
    {NULL, -1, "Classification", GTK_ASSISTANT_PAGE_CONTENT, FALSE},
    {NULL, -1, "Summary", GTK_ASSISTANT_PAGE_CONFIRM, TRUE}};

  GtkWidget *label,
            *hsep,
            *button,
            *hbox,
            *vbox,
            *vbox1,
            *vbox2,
            *sw,
            *align,
            *tmpbox,
            *checkb,
            *combob,
            *image;
  GtkTreeViewColumn *column;
  GtkTreeSelection *sel;
  GtkCellRenderer *renderer;
  GtkListStore *store;
  GtkObject *adjust;
  gint i;

  /* Introduction page */
  page_info[PAGE_INTRODUCTION].widget = gtk_label_new("PLACEHOLDER");

  /* General settings page */
  page_info[PAGE_GENERAL].widget = gtk_vbox_new(FALSE, 5);
  label = gtk_label_new("PLACEHOLDER");
  hsep = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_GENERAL].widget),
                     label, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_GENERAL].widget),
                     hsep, FALSE, FALSE, 1);
  label = gtk_label_new("Please choose a directory and a name for the project");
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_GENERAL].widget),
                     label, FALSE, FALSE, 1);
  hbox = gtk_hbox_new(FALSE, 5);
  button = gtk_button_new_with_mnemonic(BROWSE_PROJECT);
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(browse_button_clicked), (gpointer) ltrassi);
  ltrassi->label_projectfile = gtk_label_new("");
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), ltrassi->label_projectfile,
                     FALSE, FALSE, 1);
  hsep = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_GENERAL].widget),
                     hbox, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_GENERAL].widget),
                     hsep, FALSE, FALSE, 1);
  label = gtk_label_new("Please select GFF3 file(s)");
  sw = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
  ltrassi->list_view_gff3files = gtk_tree_view_new();
  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("GFF3 files",
                                                    renderer,
                                                    "text",
                                                    0,
                                                    NULL);
  gtk_tree_view_column_set_sort_column_id(column, 0);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrassi->list_view_gff3files),
                              column);
  store = gtk_list_store_new(1, G_TYPE_STRING);
  gtk_tree_view_set_model(GTK_TREE_VIEW(ltrassi->list_view_gff3files),
                          GTK_TREE_MODEL(store));
  g_object_unref(store);
  sel =
       gtk_tree_view_get_selection(GTK_TREE_VIEW(ltrassi->list_view_gff3files));
  gtk_tree_selection_set_mode(sel, GTK_SELECTION_MULTIPLE);
  gtk_container_add(GTK_CONTAINER(sw), ltrassi->list_view_gff3files);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_GENERAL].widget), sw,
                     TRUE, TRUE, 1);
  hbox = gtk_hbox_new(FALSE, 5);
  button = gtk_button_new_with_mnemonic("_Add files...");
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(add_gff3_button_clicked), (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 1);
  button = gtk_button_new_with_mnemonic("_Remove selected...");
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(remove_gff3_button_clicked), (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_GENERAL].widget),
                     hbox, FALSE, FALSE, 1);
  label = gtk_label_new("Please select the associated indexname");
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_GENERAL].widget),
                     label, FALSE, FALSE, 1);
  hbox = gtk_hbox_new(FALSE, 5);
  button = gtk_button_new_with_mnemonic(BROWSE_INDEX);
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(browse_button_clicked), (gpointer) ltrassi);
  ltrassi->label_indexname = gtk_label_new("");
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), ltrassi->label_indexname,
                     FALSE, FALSE, 1);
  hsep = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_GENERAL].widget),
                     hbox, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_GENERAL].widget),
                     hsep, FALSE, FALSE, 1);
  ltrassi->checkb_clustering =
                         gtk_check_button_new_with_label("Perform clustering?");
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_GENERAL].widget),
                     ltrassi->checkb_clustering, FALSE, FALSE, 1);

  /* Matching/Clustering settings page */
  page_info[PAGE_CLUSTERING].widget = gtk_vbox_new(FALSE, 5);
  label = gtk_label_new("Matches will be generated with BLASTN. Please set "
                        "the following parameters");
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLUSTERING].widget),
                     label, FALSE, FALSE, 1);
  hbox = gtk_hbox_new(FALSE, 5);
  vbox = gtk_vbox_new(TRUE, 1);
  label = gtk_label_new("Expectation value (E) threshold:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Filter query sequence with DUST?");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Cost to open a gap:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Cost to extend a gap:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("X-dropoff value (in bits):");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Penalty for a nucleotide mismatch:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Reward for a nucleotide match:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Number of threads (CPUs) used:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Word size for wordfinder algorithm:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Percent identity:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Select additional BLASTN parameter:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  combob = ltrassi->combob_blastparams = gtk_combo_box_new_text();
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), "");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_STRAND);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_CULLING);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_BESTHIT_OVERH);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_BESTHIT_SCORE);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_MAXT_SEQS);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_XDROP_UNG);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_XDROP_GAP);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_NO_GREEDY);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_MIN_RAW);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_UNGAPPED);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_WINDOWS);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_OFF_DIAG);
  g_signal_connect(G_OBJECT(combob), "changed",
                   G_CALLBACK(gtk_ltr_assistant_combob_blast_changed), ltrassi);
  gtk_box_pack_start(GTK_BOX(vbox), combob, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 1);

  vbox = gtk_vbox_new(TRUE, 1);
  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(10.0, 0.0001, 50.0, 0.0001, 0.1, 0.0);
  ltrassi->spinb_evalue = gtk_spin_button_new(GTK_ADJUSTMENT(adjust),
                                              0.0001, 4);
  gtk_widget_set_sensitive(ltrassi->spinb_evalue, FALSE);
  checkb =
          ltrassi->checkb_evalue = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_evalue_toggled), (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(tmpbox), ltrassi->spinb_evalue, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(FALSE, 1);
  ltrassi->checkb_dust = gtk_check_button_new();
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), ltrassi->checkb_dust);
  gtk_box_pack_start(GTK_BOX(tmpbox), align, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(0.0, 0.0, 100.0, 1.0, 10.0, 0.0);
  ltrassi->spinb_gapopen = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  gtk_widget_set_sensitive(ltrassi->spinb_gapopen, FALSE);
  checkb =
         ltrassi->checkb_gapopen = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_gapopen_toggled), (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(tmpbox), ltrassi->spinb_gapopen, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(0.0, 0.0, 100.0, 1.0, 10.0, 0.0);
  ltrassi->spinb_gapextend = gtk_spin_button_new(GTK_ADJUSTMENT(adjust),
                                                 1.0, 0);
  gtk_widget_set_sensitive(ltrassi->spinb_gapextend, FALSE);
  checkb =
       ltrassi->checkb_gapextend = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_gapextend_toggled),
                   (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(tmpbox), ltrassi->spinb_gapextend,
                     FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(20.0, 5.0, 100.0, 1.0, 10.0, 0.0);
  ltrassi->spinb_xdrop = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  gtk_widget_set_sensitive(ltrassi->spinb_xdrop, FALSE);
  checkb = ltrassi->checkb_xdrop = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_xdrop_toggled), (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(tmpbox), ltrassi->spinb_xdrop, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(0.0, -100.0, 0.0, -1.0, -10.0, 0.0);
  ltrassi->spinb_penalty = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), -1.0, 0);
  gtk_widget_set_sensitive(ltrassi->spinb_penalty, FALSE);
  checkb =
         ltrassi->checkb_penalty = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_penalty_toggled), (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(tmpbox), ltrassi->spinb_penalty, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(0.0, 0.0, 100.0, 1.0, 10.0, 0.0);
  ltrassi->spinb_reward = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  gtk_widget_set_sensitive(ltrassi->spinb_reward, FALSE);
  checkb =
          ltrassi->checkb_reward = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_reward_toggled), (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(tmpbox), ltrassi->spinb_reward, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(1.0, 1.0, 8.0, 1.0, 2.0, 0.0);
  ltrassi->spinb_threads = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  gtk_widget_set_sensitive(ltrassi->spinb_threads, FALSE);
  checkb =
         ltrassi->checkb_threads = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_threads_toggled), (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(tmpbox), ltrassi->spinb_threads, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(4.0, 4.0, 20.0, 1.0, 4.0, 0.0);
  ltrassi->spinb_words = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  gtk_widget_set_sensitive(ltrassi->spinb_words, FALSE);
  checkb = ltrassi->checkb_words = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_words_toggled), (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(tmpbox), ltrassi->spinb_words, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(80.0, 30.0, 100.0, 0.1, 1.0, 0.0);
  ltrassi->spinb_seqid = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 0.1, 1);
  gtk_widget_set_sensitive(ltrassi->spinb_seqid, FALSE);
  checkb = ltrassi->checkb_seqid = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_seqid_toggled), (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(tmpbox), ltrassi->spinb_seqid, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  label = gtk_label_new("Value for additional BLASTN paramter:");
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(FALSE, 1);
  ltrassi->entry_blastvalues = gtk_entry_new();
  gtk_widget_set_sensitive(ltrassi->entry_blastvalues, FALSE);
  gtk_box_pack_start(GTK_BOX(tmpbox), ltrassi->entry_blastvalues,
                     TRUE, TRUE, 1);
  image = gtk_image_new_from_stock(GTK_STOCK_ADD, GTK_ICON_SIZE_SMALL_TOOLBAR);
  button = ltrassi->button_addblastparam = gtk_button_new();
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(gtk_ltr_assistant_add_blast_param_clicked),
                   ltrassi);
  gtk_widget_set_sensitive(button, FALSE);
  gtk_button_set_image(GTK_BUTTON(button), image);
  gtk_box_pack_start(GTK_BOX(tmpbox), button, FALSE, FALSE, 1);
  image = gtk_image_new_from_stock(GTK_STOCK_REMOVE,
                                   GTK_ICON_SIZE_SMALL_TOOLBAR);
  button = ltrassi->button_rmblastparam = gtk_button_new();
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(gtk_ltr_assistant_rm_blast_param_clicked),
                   ltrassi);
  gtk_widget_set_sensitive(button, FALSE);
  gtk_button_set_image(GTK_BUTTON(button), image);
  gtk_box_pack_start(GTK_BOX(tmpbox), button, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 1);
  hsep = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLUSTERING].widget),
                     hbox, FALSE, FALSE, 1);
  ltrassi->label_moreblast = gtk_label_new("");
  gtk_label_set_line_wrap(GTK_LABEL(ltrassi->label_moreblast), TRUE);
  gtk_label_set_line_wrap_mode(GTK_LABEL(ltrassi->label_moreblast),
                               PANGO_WRAP_WORD_CHAR);
  gtk_label_set_justify(GTK_LABEL(ltrassi->label_moreblast), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), ltrassi->label_moreblast);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLUSTERING].widget),
                     align, TRUE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLUSTERING].widget),
                     hsep, FALSE, FALSE, 1);

  label = gtk_label_new("Please set the following paramters for clustering");
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLUSTERING].widget),
                     label, FALSE, FALSE, 1);
  hbox = gtk_hbox_new(FALSE, 5);
  vbox = gtk_vbox_new(TRUE, 1);
  label = gtk_label_new("Value for psmall:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Value for plarge:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 1);

  vbox = gtk_vbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(30.0, 30.0, 100.0, 1.0, 10.0, 0.0);
  ltrassi->spinb_psmall = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  adjust = gtk_adjustment_new(30.0, 30.0, 100.0, 1.0, 10.0, 0.0);
  ltrassi->spinb_plarge = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), ltrassi->spinb_psmall, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrassi->spinb_plarge, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 1);
  ltrassi->checkb_classification =
                     gtk_check_button_new_with_label("Perform classification?");
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLUSTERING].widget),
                     hbox, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLUSTERING].widget),
                     ltrassi->checkb_classification, FALSE, FALSE, 1);

  /* Classification settings page */
  page_info[PAGE_CLASSIFICATION].widget = gtk_vbox_new(FALSE, 5);
  label = gtk_label_new("PLACEHOLDER");
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLASSIFICATION].widget),
                     label, FALSE, FALSE, 1);
  hbox = gtk_hbox_new(FALSE, 5);
  vbox = gtk_vbox_new(TRUE, 1);
  label = gtk_label_new(LTR_TOLERANCE);
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new(LEN_TOLERANCE);
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Prefix for new LTR family:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 1);

  vbox = gtk_vbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(0.0, 0.0, 1000.0, 1.0, 10.0, 0.0);
  ltrassi->spinb_ltrtol = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 0.01, 1);
  adjust = gtk_adjustment_new(0.0, 0.0, 1000.0, 1.0, 10.0, 0.0);
  ltrassi->spinb_candtol = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 0.01, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrassi->spinb_ltrtol, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrassi->spinb_candtol, FALSE, FALSE, 1);
  ltrassi->entry_famprefix = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(ltrassi->entry_famprefix), 20);
  gtk_entry_set_text(GTK_ENTRY(ltrassi->entry_famprefix), NEW_FAM_PREFIX);
  g_signal_connect(G_OBJECT(ltrassi->entry_famprefix), "changed",
                   G_CALLBACK(gtk_ltr_assistant_fam_prefix_changed), ltrassi);
  gtk_box_pack_start(GTK_BOX(vbox), ltrassi->entry_famprefix, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLASSIFICATION].widget),
                     hbox, FALSE, FALSE, 1);
  hsep = gtk_hseparator_new();
  label = gtk_label_new("Select features for classification");
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLASSIFICATION].widget),
                     hsep, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLASSIFICATION].widget),
                     label, FALSE, FALSE, 1);
  sw = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
  ltrassi->list_view_features = gtk_tree_view_new();
  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Found features",
                                                    renderer,
                                                    "text",
                                                    0,
                                                    NULL);
  gtk_tree_view_column_set_sort_column_id(column, 0);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrassi->list_view_features),
                              column);
  store = gtk_list_store_new(1, G_TYPE_STRING);
  gtk_tree_view_set_model(GTK_TREE_VIEW(ltrassi->list_view_features),
                          GTK_TREE_MODEL(store));
  g_object_unref(store);
  sel =
       gtk_tree_view_get_selection(GTK_TREE_VIEW(ltrassi->list_view_features));
  gtk_tree_selection_set_mode(sel, GTK_SELECTION_MULTIPLE);
  g_signal_connect(G_OBJECT(sel), "changed",
                   G_CALLBACK(list_view_features_selection_changed), ltrassi);
  gtk_container_add(GTK_CONTAINER(sw), ltrassi->list_view_features);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLASSIFICATION].widget), sw,
                     TRUE, TRUE, 1);

  /* Summary page */
  ltrassi->notebook = page_info[PAGE_SUMMARY].widget = gtk_notebook_new();
  vbox1 = gtk_vbox_new(FALSE, 5);
  label = gtk_label_new("Projectfile:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Used GFF3 files:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Indexname:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Clustering?");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  vbox2 = gtk_vbox_new(FALSE, 5);
  label = ltrassi->label_projectfile2 = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_gff3files = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_indexname2 = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_doclustering = gtk_label_new("No");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  button = gtk_button_new_with_mnemonic("_Edit settings");
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(edit_general_settings_clicked),
                   (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, FALSE, 1);
  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, 1);
  label = gtk_label_new("General");
  gtk_notebook_append_page(GTK_NOTEBOOK(ltrassi->notebook), hbox, label);

  vbox1 = gtk_vbox_new(FALSE, 5);
  label = gtk_label_new("Expectation value (E) threshold:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Filter query sequence with DUST?");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Cost to open a gap:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  vbox2 = gtk_vbox_new(FALSE, 0);
  label = gtk_label_new("Cost to extend a gap:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("X-dropoff value (in bits):");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Penalty for a nucleotide mismatch:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Reward for a nucleotide match:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Number of threads (CPUs) used:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Word size for wordfinder algorithm:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Percent identity:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);

  label = gtk_label_new("Additional BLASTN parameter:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);

  label = gtk_label_new("Value for psmall:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Value for plarge:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Classification?");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  vbox2 = gtk_vbox_new(FALSE, 5);
  label = ltrassi->label_evalue = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_dust = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_gapopen = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_gapextend = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_xdrop = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_penalty = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_reward = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_threads = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_wordsize = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_seqidentity = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_moreblast2 = gtk_label_new("");
  gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
  gtk_label_set_line_wrap_mode(GTK_LABEL(label), PANGO_WRAP_WORD_CHAR);
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_psmall = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_plarge = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_doclassification = gtk_label_new("No");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  button = gtk_button_new_with_mnemonic("_Edit settings");
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(edit_clustering_settings_clicked),
                   (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, FALSE, 1);
  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, 1);
  label = gtk_label_new("Matching/Clustering");
  gtk_notebook_append_page(GTK_NOTEBOOK(ltrassi->notebook), hbox, label);

  vbox1 = gtk_vbox_new(FALSE, 5);
  label = gtk_label_new("Allowed LTR length deviation:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Allowed candidate length deviation:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Prefix for new LTR family:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Used features:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  vbox2 = gtk_vbox_new(FALSE, 5);
  label = ltrassi->label_ltrtolerance = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_candtolerance = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_famprefix = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_usedfeatures = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  button = gtk_button_new_with_mnemonic("_Edit settings");
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(edit_classification_settings_clicked),
                   (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, FALSE, 1);
  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, 1);
  label = gtk_label_new("Classification");
  gtk_notebook_append_page(GTK_NOTEBOOK(ltrassi->notebook), hbox, label);

  /* add pages to assistant */
  for (i = 0; i < N_PAGES; i++) {
    gtk_widget_show_all(page_info[i].widget);
    page_info[i].index = gtk_assistant_append_page(GTK_ASSISTANT(ltrassi),
                                                   page_info[i].widget);
    gtk_assistant_set_page_title(GTK_ASSISTANT(ltrassi), page_info[i].widget,
                                 page_info[i].title);
    gtk_assistant_set_page_type(GTK_ASSISTANT(ltrassi), page_info[i].widget,
                                page_info[i].type);
    gtk_assistant_set_page_complete(GTK_ASSISTANT(ltrassi), page_info[i].widget,
                                    page_info[i].complete);
  }

  /* connect signals */
  g_signal_connect(G_OBJECT(ltrassi->checkb_clustering), "toggled",
                   G_CALLBACK(checkb_clustering_toggled), (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_evalue), "value-changed",
                   G_CALLBACK(cluster_settings_adjustment_value_changed),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->checkb_dust), "toggled",
                   G_CALLBACK(cluster_settings_dust_toggled),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_gapopen), "value-changed",
                   G_CALLBACK(cluster_settings_adjustment_value_changed),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_gapextend), "value-changed",
                   G_CALLBACK(cluster_settings_adjustment_value_changed),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_xdrop), "value-changed",
                   G_CALLBACK(cluster_settings_adjustment_value_changed),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_penalty), "value-changed",
                   G_CALLBACK(cluster_settings_adjustment_value_changed),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_reward), "value-changed",
                   G_CALLBACK(cluster_settings_adjustment_value_changed),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_threads), "value-changed",
                   G_CALLBACK(cluster_settings_adjustment_value_changed),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_words), "value-changed",
                   G_CALLBACK(cluster_settings_adjustment_value_changed),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_seqid), "value-changed",
                   G_CALLBACK(cluster_settings_adjustment_value_changed),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_psmall), "value-changed",
                   G_CALLBACK(cluster_settings_adjustment_value_changed),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_plarge), "value-changed",
                   G_CALLBACK(cluster_settings_adjustment_value_changed),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->checkb_classification), "toggled",
                   G_CALLBACK(checkb_classification_toggled),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_ltrtol), "value-changed",
                   G_CALLBACK(cassification_settings_adjustment_value_changed),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_candtol), "value-changed",
                   G_CALLBACK(cassification_settings_adjustment_value_changed),
                   (gpointer) ltrassi);

  ltrassi->last_dir = NULL;
  ltrassi->added_features = FALSE;
  ltrassi->hasht_blastparams = g_hash_table_new_full(g_str_hash, g_str_equal,
                                                     g_free, g_free);
}

gint page_forward(gint current_page, GtkLTRAssistant *ltrassi)
{
  gint next_page;
  gboolean active;

  switch (current_page) {
    case PAGE_INTRODUCTION:
      next_page = PAGE_GENERAL;
      break;
    case PAGE_GENERAL:
      active =  gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(
                                               ltrassi->checkb_clustering));
      next_page = (active ? PAGE_CLUSTERING : PAGE_SUMMARY);
      break;
    case PAGE_CLUSTERING:
      active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(
                                              ltrassi->checkb_classification));
      if (active)
        get_feature_list(ltrassi);
      next_page = (active ? PAGE_CLASSIFICATION : PAGE_SUMMARY);
      break;
    case PAGE_CLASSIFICATION:
      next_page = PAGE_SUMMARY;
      break;
    default:
      next_page = -1;
  }
  return next_page;
}

static void gtk_ltr_assistant_cancel(GtkAssistant *assistant,
                                     GT_UNUSED gpointer data)

{
  gtk_widget_hide(GTK_WIDGET(assistant));
}

GType gtk_ltr_assistant_get_type(void)
{
  static GType ltr_assistant_type = 0;

  if (!ltr_assistant_type) {
    const GTypeInfo ltr_assistant_info =
    {
      sizeof (GtkLTRAssistantClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      NULL, /*(GClassInitFunc) */
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (GtkLTRAssistant),
      0, /* n_preallocs */
      (GInstanceInitFunc) gtk_ltr_assistant_init,
    };
    ltr_assistant_type = g_type_register_static(GTK_TYPE_ASSISTANT,
                                                "GtkLTRAssistant",
                                                &ltr_assistant_info, 0);
  }
  return ltr_assistant_type;
}

GtkWidget* gtk_ltr_assistant_new()
{
  GtkLTRAssistant *ltrassi;
  ltrassi = gtk_type_new(GTK_LTR_ASSISTANT_TYPE);
  gtk_assistant_set_forward_page_func(GTK_ASSISTANT(ltrassi),
                                      (GtkAssistantPageFunc) page_forward,
                                      ltrassi, NULL);
  gtk_window_set_position(GTK_WINDOW(ltrassi), GTK_WIN_POS_CENTER_ALWAYS);
  g_signal_connect(G_OBJECT(ltrassi), "destroy",
                   G_CALLBACK(gtk_ltr_assistant_destroy), NULL);
  g_signal_connect(G_OBJECT(ltrassi), "cancel",
                   G_CALLBACK(gtk_ltr_assistant_cancel), NULL);
  g_signal_connect(G_OBJECT(ltrassi), "close",
                   G_CALLBACK(gtk_ltr_assistant_cancel), NULL);
  show_hide_tab(GTK_NOTEBOOK(ltrassi->notebook), 1, FALSE);
  show_hide_tab(GTK_NOTEBOOK(ltrassi->notebook), 2, FALSE);

  return GTK_WIDGET(ltrassi);
}
