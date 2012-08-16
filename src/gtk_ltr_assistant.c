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

#include <string.h>
#include "gtk_ltr_assistant.h"
#include "message_strings.h"
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

gint gtk_ltr_assistant_get_xgapped(GtkLTRAssistant *ltrassi)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ltrassi->checkb_xgapped)))
    return GT_UNDEF_INT;
  return gtk_spin_button_get_value(GTK_SPIN_BUTTON(ltrassi->spinb_xgapped));
}

gint gtk_ltr_assistant_get_xgapless(GtkLTRAssistant *ltrassi)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ltrassi->checkb_xgapless)))
    return GT_UNDEF_INT;
  return gtk_spin_button_get_value(GTK_SPIN_BUTTON(ltrassi->spinb_xgapless));
}

gint gtk_ltr_assistant_get_xfinal(GtkLTRAssistant *ltrassi)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ltrassi->checkb_xfinal)))
    return GT_UNDEF_INT;
  return gtk_spin_button_get_value(GTK_SPIN_BUTTON(ltrassi->spinb_xfinal));
}

gint gtk_ltr_assistant_get_matchscore(GtkLTRAssistant *ltrassi)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ltrassi->checkb_matchscore)))
    return GT_UNDEF_INT;
  return
      gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ltrassi->spinb_matchscore));
}

gint gtk_ltr_assistant_get_mismatchcost(GtkLTRAssistant *ltrassi)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ltrassi->checkb_mismatchcost)))
    return GT_UNDEF_INT;
  return
      gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ltrassi->spinb_mismatchcost));
}

gint gtk_ltr_assistant_get_stepsize(GtkLTRAssistant *ltrassi)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ltrassi->checkb_stepsize)))
    return GT_UNDEF_INT;
  return
      gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ltrassi->spinb_stepsize));
}

gint gtk_ltr_assistant_get_mscoregapped(GtkLTRAssistant *ltrassi)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ltrassi->checkb_mscoregapped)))
    return GT_UNDEF_INT;
  return
      gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ltrassi->spinb_mscoregapped));
}

gint gtk_ltr_assistant_get_mscoregapless(GtkLTRAssistant *ltrassi)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ltrassi->checkb_mscoregapless)))
    return GT_UNDEF_INT;
  return
      gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ltrassi->spinb_mscoregapless));
}

const gchar* gtk_ltr_assistant_get_morelast(GtkLTRAssistant *ltrassi)
{
  return gtk_label_get_text(GTK_LABEL(ltrassi->label_morelast));
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

GtArray* gtk_ltr_assistant_get_regions(GtkLTRAssistant *ltrassi)
{
  return ltrassi->regions;
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

gboolean select_defaults(GtkTreeModel *model,
                         GT_UNUSED GtkTreePath  *path,
                         GT_UNUSED GtkTreeIter  *iter,
                         gpointer      user_data)
{
  char *feature;
  GtkLTRAssistant *ltrassi = (GtkLTRAssistant*) user_data;
  GT_UNUSED GtkTreeSelection *selection =
      gtk_tree_view_get_selection(GTK_TREE_VIEW(
                                   ltrassi->list_view_features));
  gtk_tree_model_get (model, iter, 0, &feature, -1);

  if ((g_strcmp0(feature, "rLTR") == 0) ||
      (g_strcmp0(feature, "lLTR") == 0) ||
      (g_strcmp0(feature, "RR_tract") == 0) ||
      (g_strcmp0(feature, "primer_binding_site") == 0) ||
      (g_strcmp0(feature, "RVT_1") == 0) ||
      (g_strcmp0(feature, "RVT_2") == 0)) {
    gtk_tree_selection_select_iter(selection, iter);
  }

  g_free(feature);
  return FALSE;
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

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrassi->list_view_features));
  if (!had_err) {
    gtk_list_store_clear(GTK_LIST_STORE(model));
    gt_hashmap_foreach(features, fill_feature_list,
                       (void*) ltrassi->list_view_features, err);
  }
  ltrassi->regions = ltrgui_preprocess_stream_get_region_nodes(
                                   (LTRGuiPreprocessStream*) preprocess_stream);
  gt_error_delete(err);
  gt_hashmap_delete(features);
  gt_node_stream_delete(gff3_in_stream);
  gt_node_stream_delete(preprocess_stream);
  for (i = 0; i < num_of_files; i++)
    g_free((gpointer) gff3_files[i]);
  g_free(gff3_files);
  ltrassi->added_features = TRUE;
}

static void update_cluster_overview(GtkLTRAssistant *ltrassi)
{
  gint psmall,
       plarge,
       gapopen,
       gapextend,
       mscoregapped,
       mscoregapless,
       stepsize,
       matchscore,
       mismatchcost,
       xgapped,
       xfinal,
       xgapless;
  gchar buffer[BUFSIZ];


  gapopen = gtk_ltr_assistant_get_gapopen(ltrassi);
  gapextend = gtk_ltr_assistant_get_gapextend(ltrassi);
  xgapped = gtk_ltr_assistant_get_xgapped(ltrassi);
  xfinal = gtk_ltr_assistant_get_xfinal(ltrassi);
  xgapless = gtk_ltr_assistant_get_xgapless(ltrassi);
  matchscore = gtk_ltr_assistant_get_matchscore(ltrassi);
  mismatchcost = gtk_ltr_assistant_get_mismatchcost(ltrassi);
  stepsize = gtk_ltr_assistant_get_stepsize(ltrassi);
  mscoregapped = gtk_ltr_assistant_get_mscoregapped(ltrassi);
  mscoregapless = gtk_ltr_assistant_get_mscoregapless(ltrassi);
  psmall = gtk_ltr_assistant_get_psmall(ltrassi);
  plarge = gtk_ltr_assistant_get_plarge(ltrassi);

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
  if (xgapped == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(ltrassi->label_xgapped), USE_DEFAULT_A);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", xgapped);
    gtk_label_set_text(GTK_LABEL(ltrassi->label_xgapped), buffer);
  }
  if (xfinal == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(ltrassi->label_xfinal), USE_DEFAULT_A);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", xfinal);
    gtk_label_set_text(GTK_LABEL(ltrassi->label_xfinal), buffer);
  }
  if (xgapless == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(ltrassi->label_xgapless), USE_DEFAULT_A);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", xgapless);
    gtk_label_set_text(GTK_LABEL(ltrassi->label_xgapless), buffer);
  }
  if (matchscore == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(ltrassi->label_matchscore), USE_DEFAULT_A);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", matchscore);
    gtk_label_set_text(GTK_LABEL(ltrassi->label_matchscore), buffer);
  }
  if (mismatchcost == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(ltrassi->label_mismatchcost), USE_DEFAULT_A);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", mismatchcost);
    gtk_label_set_text(GTK_LABEL(ltrassi->label_mismatchcost), buffer);
  }
  if (stepsize == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(ltrassi->label_stepsize), USE_DEFAULT_A);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", stepsize);
    gtk_label_set_text(GTK_LABEL(ltrassi->label_stepsize), buffer);
  }
  if (mscoregapless == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(ltrassi->label_mscoregapless), USE_DEFAULT_A);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", mscoregapless);
    gtk_label_set_text(GTK_LABEL(ltrassi->label_mscoregapless), buffer);
  }
  if (mscoregapped == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(ltrassi->label_mscoregapped), USE_DEFAULT_A);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", mscoregapped);
    gtk_label_set_text(GTK_LABEL(ltrassi->label_mscoregapped), buffer);
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
  gchar *text = NULL,
        *old_text = NULL,
        *buffer = NULL;

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

static void
list_view_features_selection_changed(GT_UNUSED GtkTreeSelection *sel,
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

  g_list_foreach(references, (GFunc) remove_row, NULL);
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
  gchar *file = NULL;

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
      file = (gchar*) filenames->data;
      if (!entry_in_list_view(model, file, 0)) {
        gtk_list_store_append(GTK_LIST_STORE(model), &iter);
        gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                           0, file,
                           -1);
        ltrassi->added_features = FALSE;
      }
      filenames = filenames->next;
    }
    if (ltrassi->last_dir)
      g_free(ltrassi->last_dir);
    ltrassi->last_dir = g_path_get_dirname(file);
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
  if (!active)
    gtk_toggle_button_set_active(
                      GTK_TOGGLE_BUTTON(ltrassi->checkb_classification), FALSE);
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

static void checkb_xgapped_toggled(GtkToggleButton *togglebutton,
                                   GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(ltrassi->spinb_xgapped, !active);
  update_cluster_overview(ltrassi);
}

static void checkb_xfinal_toggled(GtkToggleButton *togglebutton,
                                   GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(ltrassi->spinb_xfinal, !active);
  update_cluster_overview(ltrassi);
}

static void checkb_xgapless_toggled(GtkToggleButton *togglebutton,
                                   GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(ltrassi->spinb_xgapless, !active);
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

static void checkb_mscoregapped_toggled(GtkToggleButton *togglebutton,
                                 GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(ltrassi->spinb_mscoregapped, !active);
  update_cluster_overview(ltrassi);
}

static void checkb_mscoregapless_toggled(GtkToggleButton *togglebutton,
                                 GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(ltrassi->spinb_mscoregapless, !active);
  update_cluster_overview(ltrassi);
}

static void checkb_mismatchcost_toggled(GtkToggleButton *togglebutton,
                                   GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(ltrassi->spinb_mismatchcost, !active);
  update_cluster_overview(ltrassi);
}

static void checkb_matchscore_toggled(GtkToggleButton *togglebutton,
                                   GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(ltrassi->spinb_matchscore, !active);
  update_cluster_overview(ltrassi);
}

static void checkb_stepsize_toggled(GtkToggleButton *togglebutton,
                                   GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(ltrassi->spinb_stepsize, !active);
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
       mscoregapped,
       mscoregapless,
       stepsize,
       matchscore,
       mismatchcost,
       xgapped,
       xfinal,
       xgapless;
  gchar last_call[BUFSIZ];
  gchar *morelast = NULL;
  gboolean first = TRUE;

  gapopen = gtk_ltr_assistant_get_gapopen(ltrassi);
  gapextend = gtk_ltr_assistant_get_gapextend(ltrassi);
  xgapped = gtk_ltr_assistant_get_xgapped(ltrassi);
  xfinal = gtk_ltr_assistant_get_xfinal(ltrassi);
  xgapless = gtk_ltr_assistant_get_xgapless(ltrassi);
  matchscore = gtk_ltr_assistant_get_matchscore(ltrassi);
  mismatchcost = gtk_ltr_assistant_get_mismatchcost(ltrassi);
  stepsize = gtk_ltr_assistant_get_stepsize(ltrassi);
  mscoregapped = gtk_ltr_assistant_get_mscoregapped(ltrassi);
  mscoregapless = gtk_ltr_assistant_get_mscoregapless(ltrassi);

  if (xgapped != GT_UNDEF_INT)
    sprintf(last_call, "%s -x %d", last_call, xgapped);
  if (xfinal != GT_UNDEF_INT)
    sprintf(last_call, "%s -z %d", last_call, xfinal);
  if (xgapless != GT_UNDEF_INT)
    sprintf(last_call, "%s -y %d", last_call, xgapless);
  if (mscoregapped != GT_UNDEF_INT)
    sprintf(last_call, "%s -e %d", last_call, mscoregapped);
  if (mscoregapless != GT_UNDEF_INT)
    sprintf(last_call, "%s -d %d", last_call, mscoregapless);

  if (gapopen != GT_UNDEF_INT)
    sprintf(last_call, "%s -a %d", last_call, gapopen);
  if (gapextend != GT_UNDEF_INT)
    sprintf(last_call, "%s -b %d", last_call, gapextend);
  if (matchscore != GT_UNDEF_INT)
    sprintf(last_call, "%s -r %d", last_call, matchscore);
  if (mismatchcost != GT_UNDEF_INT)
    sprintf(last_call, "%s -q %d", last_call, mismatchcost);
  if (stepsize != GT_UNDEF_INT)
    sprintf(last_call, "%s -k %d", last_call, stepsize);

  keys = g_hash_table_get_keys(ltrassi->hasht_lastparams);
  keys = g_list_sort(keys, compare_strings_from_list);
  while (keys != NULL) {
    gpointer key = keys->data;
    gpointer value;

    value = g_hash_table_lookup(ltrassi->hasht_lastparams, key);
    if (first) {
      morelast = g_strdup_printf("%s %s", (gchar*) key, (gchar*) value);
      first = FALSE;
    } else
      morelast = g_strjoin(" ", morelast, (gchar*) key, (gchar*) value, NULL);
    keys = keys->next;
  }
  if (morelast)
    sprintf(last_call, "%s %s", last_call, morelast);

  return g_strdup(last_call);
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

static void update_label_morelast(GtkLTRAssistant *ltrassi)
{
  GString *string;
  gchar *buffer;

  string = g_string_new("");

  g_hash_table_foreach(ltrassi->hasht_lastparams, (GHFunc) build_string,
                       (gpointer) string);
  buffer = g_string_free(string, FALSE);
  gtk_label_set_text(GTK_LABEL(ltrassi->label_morelast), buffer);
  gtk_label_set_text(GTK_LABEL(ltrassi->label_morelast2), buffer);
  g_free(buffer);
}

GT_UNUSED static void combob_last_changed(GtkComboBox *combob,
                                           GtkLTRAssistant *ltrassi)
{
  gchar *param;
  gpointer value;

  param = gtk_combo_box_get_active_text(combob);
  if (g_strcmp0(param, "") == 0) {
    gtk_widget_set_sensitive(ltrassi->entry_lastvalues, FALSE);
    gtk_widget_set_sensitive(ltrassi->button_addlastparam, FALSE);
    gtk_widget_set_sensitive(ltrassi->button_rmlastparam, FALSE);
    g_free(param);
    return;
  }
  gtk_widget_set_sensitive(ltrassi->entry_lastvalues, TRUE);
  gtk_widget_set_sensitive(ltrassi->button_addlastparam, TRUE);
  gtk_widget_set_sensitive(ltrassi->button_rmlastparam, TRUE);

  value = g_hash_table_lookup(ltrassi->hasht_lastparams,
                              (gconstpointer) param);
  if (value)
    gtk_entry_set_text(GTK_ENTRY(ltrassi->entry_lastvalues), (gchar*) value);
  else
    gtk_entry_set_text(GTK_ENTRY(ltrassi->entry_lastvalues), "");
  g_free(param);
}

GT_UNUSED static void add_last_param_clicked(GT_UNUSED GtkButton *button,
                                              GtkLTRAssistant *ltrassi)
{
  gchar *key;
  const gchar *value;

  key =
      gtk_combo_box_get_active_text(GTK_COMBO_BOX(ltrassi->combob_lastparams));
  value = gtk_entry_get_text(GTK_ENTRY(ltrassi->entry_lastvalues));
  if ((g_strcmp0(key, BLASTN_NO_GREEDY) == 0) ||
      (g_strcmp0(key, BLASTN_UNGAPPED) == 0))
    value = "";
  else if (g_strcmp0(value, "") == 0) {
    g_free(key);
    return;
  }
  g_hash_table_insert(ltrassi->hasht_lastparams,
                      (gpointer) key, (gpointer) g_strdup(value));
  update_label_morelast(ltrassi);
}

GT_UNUSED static void remove_last_param_clicked(GT_UNUSED GtkButton *button,
                                                 GtkLTRAssistant *ltrassi)
{
  gchar *key;

  key =
      gtk_combo_box_get_active_text(GTK_COMBO_BOX(ltrassi->combob_lastparams));
  if (!g_hash_table_remove(ltrassi->hasht_lastparams, (gconstpointer) key)) {
    g_free(key);
    return;
  }
  gtk_entry_set_text(GTK_ENTRY(ltrassi->entry_lastvalues), "");
  update_label_morelast(ltrassi);
  g_free(key);
}

static void family_prefix_changed(GT_UNUSED GtkEditable *edit,
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
  g_hash_table_destroy(ltrassi->hasht_lastparams);

  return FALSE;
}

static gint page_forward(gint current_page, GtkLTRAssistant *ltrassi)
{
  gint next_page;
  gboolean active;
  switch (current_page) {
    /* case PAGE_INTRODUCTION:
      next_page = PAGE_GENERAL;
      break; */
    case PAGE_GENERAL:
      active =  gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(
                                               ltrassi->checkb_clustering));
      next_page = (active ? PAGE_CLUSTERING : PAGE_SUMMARY);
      break;
    case PAGE_CLUSTERING:
      active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(
                                              ltrassi->checkb_classification));
      if (active) {
        GtkTreeModel *model;
        model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrassi->list_view_features));
        get_feature_list(ltrassi);
        gtk_tree_model_foreach(GTK_TREE_MODEL(model), select_defaults, ltrassi);
      }
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

static void cancel_clicked(GtkAssistant *assistant, GT_UNUSED gpointer data)

{
  gtk_widget_hide(GTK_WIDGET(assistant));
}

static void gtk_ltr_assistant_init(GtkLTRAssistant *ltrassi)
{
  LTRAssistantPageInfo page_info[N_PAGES] = {
    /* {NULL, -1, "Introduction", GTK_ASSISTANT_PAGE_INTRO, TRUE}, */
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
            *checkb;
  GT_UNUSED GtkWidget *image, *combob;
  GtkTreeViewColumn *column;
  GtkTreeSelection *sel;
  GtkCellRenderer *renderer;
  GtkListStore *store;
  GtkObject *adjust;
  gint i;

  /* Introduction page */
  /* page_info[PAGE_INTRODUCTION].widget = gtk_label_new("PLACEHOLDER"); */

  /* General settings page */
  page_info[PAGE_GENERAL].widget = gtk_vbox_new(FALSE, 5);
  /* label = gtk_label_new("This page let you select files");
  hsep = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_GENERAL].widget),
                     label, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_GENERAL].widget),
                     hsep, FALSE, FALSE, 1); */
  label = gtk_label_new("Choose a directory and a name for the project");
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
  label = gtk_label_new("Select GFF3 annotation file(s)");
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_GENERAL].widget),
                     label, FALSE, FALSE, 1);
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
  label = gtk_label_new("Select the associated indexname");
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
  label = gtk_label_new("Set LAST parameters");
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLUSTERING].widget),
                     label, FALSE, FALSE, 1);
  hbox = gtk_hbox_new(FALSE, 5);
  vbox = gtk_vbox_new(TRUE, 1);
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
  label = gtk_label_new("Maximum score drop for gapped alignments:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Maximum score drop for gapless alignments:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Maximum score drop for final gapped alignments:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Minimum score for gapless alignments:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Minimum score for gapped alignments:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Cost for a nucleotide mismatch:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Score for a nucleotide match:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Step size along the query sequence:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  /* label = gtk_label_new("Select additional LAST parameter:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);

  combob = ltrassi->combob_lastparams = gtk_combo_box_new_text();
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), "");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_OFF_DIAG);
  g_signal_connect(G_OBJECT(combob), "changed",
                   G_CALLBACK(combob_last_changed), ltrassi);
  gtk_box_pack_start(GTK_BOX(vbox), combob, FALSE, FALSE, 1);
  */
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 1);

  vbox = gtk_vbox_new(TRUE, 1);
  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(7.0, 0.0, 100.0, 1.0, 10.0, 0.0);
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
  adjust = gtk_adjustment_new(1.0, 0.0, 100.0, 1.0, 10.0, 0.0);
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
  ltrassi->spinb_xgapped = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  gtk_widget_set_sensitive(ltrassi->spinb_xgapped, FALSE);
  checkb = ltrassi->checkb_xgapped = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_xgapped_toggled), (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(tmpbox), ltrassi->spinb_xgapped, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);

  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(20.0, 5.0, 100.0, 1.0, 10.0, 0.0);
  ltrassi->spinb_xgapless = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  gtk_widget_set_sensitive(ltrassi->spinb_xgapless, FALSE);
  checkb = ltrassi->checkb_xgapless = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_xgapless_toggled), (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(tmpbox), ltrassi->spinb_xgapless, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);

  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(20.0, 5.0, 100.0, 1.0, 10.0, 0.0);
  ltrassi->spinb_xfinal = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  gtk_widget_set_sensitive(ltrassi->spinb_xfinal, FALSE);
  checkb = ltrassi->checkb_xfinal = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_xfinal_toggled), (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(tmpbox), ltrassi->spinb_xfinal, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);

  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(24.0, 5.0, 100.0, 1.0, 10.0, 0.0);
  ltrassi->spinb_mscoregapless = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  gtk_widget_set_sensitive(ltrassi->spinb_mscoregapless, FALSE);
  checkb = ltrassi->checkb_mscoregapless = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_mscoregapless_toggled), (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(tmpbox), ltrassi->spinb_mscoregapless, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);

  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(40.0, 5.0, 100.0, 1.0, 10.0, 0.0);
  ltrassi->spinb_mscoregapped = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  gtk_widget_set_sensitive(ltrassi->spinb_mscoregapped, FALSE);
  checkb = ltrassi->checkb_mscoregapped = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_mscoregapped_toggled), (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(tmpbox), ltrassi->spinb_mscoregapped, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);


  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(1.0, 0.0, 100.0, 1.0, 10.0, 0.0);
  ltrassi->spinb_mismatchcost = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  gtk_widget_set_sensitive(ltrassi->spinb_mismatchcost, FALSE);
  checkb =
         ltrassi->checkb_mismatchcost = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_mismatchcost_toggled), (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(tmpbox), ltrassi->spinb_mismatchcost, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);


  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(1.0, 0.0, 100.0, 1.0, 10.0, 0.0);
  ltrassi->spinb_matchscore = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  gtk_widget_set_sensitive(ltrassi->spinb_matchscore, FALSE);
  checkb =
          ltrassi->checkb_matchscore = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_matchscore_toggled), (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(tmpbox), ltrassi->spinb_matchscore, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);


  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(1.0, 1.0, 1000.0, 1.0, 2.0, 0.0);
  ltrassi->spinb_stepsize = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  gtk_widget_set_sensitive(ltrassi->spinb_stepsize, FALSE);
  checkb =
         ltrassi->checkb_stepsize = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_stepsize_toggled), (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(tmpbox), ltrassi->spinb_stepsize, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);

  /*label = gtk_label_new("Value for additional LAST paramter:");
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(FALSE, 1);
  ltrassi->entry_lastvalues = gtk_entry_new();
  gtk_widget_set_sensitive(ltrassi->entry_lastvalues, FALSE);
  gtk_box_pack_start(GTK_BOX(tmpbox), ltrassi->entry_lastvalues,
                     TRUE, TRUE, 1);
  image = gtk_image_new_from_stock(GTK_STOCK_ADD, GTK_ICON_SIZE_SMALL_TOOLBAR);
  button = ltrassi->button_addlastparam = gtk_button_new();
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(add_last_param_clicked),
                   ltrassi);
  gtk_widget_set_sensitive(button, FALSE);
  gtk_button_set_image(GTK_BUTTON(button), image);
  gtk_box_pack_start(GTK_BOX(tmpbox), button, FALSE, FALSE, 1);
  image = gtk_image_new_from_stock(GTK_STOCK_REMOVE,
                                   GTK_ICON_SIZE_SMALL_TOOLBAR);
  button = ltrassi->button_rmlastparam = gtk_button_new();
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(remove_last_param_clicked),
                   ltrassi);
  gtk_widget_set_sensitive(button, FALSE);
  gtk_button_set_image(GTK_BUTTON(button), image);
  gtk_box_pack_start(GTK_BOX(tmpbox), button, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);

  hsep = gtk_hseparator_new();
  ltrassi->label_morelast = gtk_label_new("");
  gtk_label_set_line_wrap(GTK_LABEL(ltrassi->label_morelast), TRUE);
  gtk_label_set_line_wrap_mode(GTK_LABEL(ltrassi->label_morelast),
                               PANGO_WRAP_WORD_CHAR);
  gtk_label_set_justify(GTK_LABEL(ltrassi->label_morelast), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), ltrassi->label_morelast); */

  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLUSTERING].widget),
                     hbox, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLUSTERING].widget),
                     align, TRUE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLUSTERING].widget),
                     hsep, FALSE, FALSE, 1);

  label = gtk_label_new("Criteria for clustering");
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLUSTERING].widget),
                     label, FALSE, FALSE, 1);
  hbox = gtk_hbox_new(FALSE, 5);
  vbox = gtk_vbox_new(TRUE, 1);
  label = gtk_label_new("% of the smaller seq. a match needs to cover:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("% of the larger seq. a match needs to cover:");
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
  /* label = gtk_label_new("PLACEHOLDER");
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLASSIFICATION].widget),
                     label, FALSE, FALSE, 1); */
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
  label = gtk_label_new("Prefix for new LTR candidate groups:");
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
                   G_CALLBACK(family_prefix_changed), ltrassi);
  gtk_box_pack_start(GTK_BOX(vbox), ltrassi->entry_famprefix, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLASSIFICATION].widget),
                     hbox, FALSE, FALSE, 1);
  hsep = gtk_hseparator_new();
  label = gtk_label_new("Select at least one feature for classification");
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
  label = gtk_label_new("Please hold <Ctrl> or <Command> while clicking to select "
                        "multiple features.");
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLASSIFICATION].widget),
                     label, FALSE, FALSE, 1);

  /* Summary page */
  ltrassi->notebook = page_info[PAGE_SUMMARY].widget = gtk_notebook_new();
  vbox1 = gtk_vbox_new(FALSE, 5);
  label = gtk_label_new("Project file to be created:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("GFF3 files:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Index name:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Run clustering component?");
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
  label = gtk_label_new("Maximum score drop for gapped alignments:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Maximum score drop for gapless alignments:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Maximum score drop for final gapped alignments:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Minimum score for gapless alignments:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Minimum score for gapped alignments:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Cost for a nucleotide mismatch:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Score for a nucleotide match:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Step size along the query sequence:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);

  label = gtk_label_new("Additional LAST parameters:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);

  label = gtk_label_new("% of the smaller seq. a match needs to cover:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("% of the larger seq. a match needs to cover:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Perform classification?");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  vbox2 = gtk_vbox_new(FALSE, 5);
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
  label = ltrassi->label_xgapped = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_xgapless = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_xfinal = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_mscoregapless = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_mscoregapped = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_mismatchcost = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_matchscore = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_stepsize = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_morelast2 = gtk_label_new("");
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
  label = gtk_label_new("Prefix for new LTR candidate groups:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Use features:");
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
  g_signal_connect(G_OBJECT(ltrassi->spinb_matchscore), "value-changed",
                   G_CALLBACK(cluster_settings_adjustment_value_changed),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_mismatchcost), "value-changed",
                   G_CALLBACK(cluster_settings_adjustment_value_changed),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_gapopen), "value-changed",
                   G_CALLBACK(cluster_settings_adjustment_value_changed),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_gapextend), "value-changed",
                   G_CALLBACK(cluster_settings_adjustment_value_changed),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_xgapped), "value-changed",
                   G_CALLBACK(cluster_settings_adjustment_value_changed),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_xgapless), "value-changed",
                   G_CALLBACK(cluster_settings_adjustment_value_changed),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_xfinal), "value-changed",
                   G_CALLBACK(cluster_settings_adjustment_value_changed),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_mscoregapless), "value-changed",
                   G_CALLBACK(cluster_settings_adjustment_value_changed),
                   (gpointer) ltrassi);
  g_signal_connect(G_OBJECT(ltrassi->spinb_mscoregapped), "value-changed",
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
  ltrassi->hasht_lastparams = g_hash_table_new_full(g_str_hash, g_str_equal,
                                                     g_free, g_free);
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
  gchar title[BUFSIZ];

  ltrassi = gtk_type_new(GTK_LTR_ASSISTANT_TYPE);
  g_snprintf(title, BUFSIZ, LTRASSI_WINDOW_TITLE, GUI_NAME);
  gtk_window_set_title(GTK_WINDOW(ltrassi), title);
  gtk_assistant_set_forward_page_func(GTK_ASSISTANT(ltrassi),
                                      (GtkAssistantPageFunc) page_forward,
                                      ltrassi, NULL);
  gtk_window_set_position(GTK_WINDOW(ltrassi), GTK_WIN_POS_CENTER_ALWAYS);
  g_signal_connect(G_OBJECT(ltrassi), "destroy",
                   G_CALLBACK(gtk_ltr_assistant_destroy), NULL);
  g_signal_connect(G_OBJECT(ltrassi), "cancel",
                   G_CALLBACK(cancel_clicked), NULL);
  g_signal_connect(G_OBJECT(ltrassi), "close",
                   G_CALLBACK(cancel_clicked), NULL);
  show_hide_tab(GTK_NOTEBOOK(ltrassi->notebook), 1, FALSE);
  show_hide_tab(GTK_NOTEBOOK(ltrassi->notebook), 2, FALSE);

  return GTK_WIDGET(ltrassi);
}
