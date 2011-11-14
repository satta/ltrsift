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

#include "error.h"
#include "menubar_main.h"
#include "project_wizard.h"
#include "support.h"

void extract_project_settings(GUIData *ltrgui)
{
  GtkLTRAssistant *ltrassi = GTK_LTR_ASSISTANT(ltrgui->assistant);
  gint psmall,
       plarge,
       gapopen,
       gapextend,
       wordsize,
       penalty,
       reward,
       num_threads,
       num_of_gff3files,
       num_of_features;
  const gchar *projectfile,
              *indexname;
  gchar **gff3files,
        **features;
  GtkTreeView *list_view;
  GtkTreeSelection *sel;
  GtkTreeIter iter;
  GtkTreeModel *model;
  gboolean dust, clustering, classification;
  gdouble evalue,
          seqid,
          xdrop,
          ltrtol,
          candtol;
  GList *rows,
        *tmp;
  gint i = 0;

  projectfile = gtk_ltr_assistant_get_projectfile(ltrassi);
  list_view = gtk_ltr_assistant_get_list_view_gff3files(ltrassi);
  sel = gtk_tree_view_get_selection(list_view);
  gtk_tree_selection_select_all(sel);
  num_of_gff3files = gtk_tree_selection_count_selected_rows(sel);
  rows = gtk_tree_selection_get_selected_rows(sel, &model);
  gff3files = g_malloc((size_t) (num_of_gff3files + 1) * sizeof(gchar*));
  tmp = rows;
  while (tmp != NULL) {
    gtk_tree_model_get_iter(model, &iter, (GtkTreePath*) tmp->data);
    gtk_tree_model_get(model, &iter,
                       0, &gff3files[i],
                       -1);
    i++;
    tmp = tmp->next;
  }
  gff3files[i] = NULL;
  g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
  g_list_free(rows);
  i = 0;
  indexname = gtk_ltr_assistant_get_indexname(ltrassi);
  clustering = gtk_ltr_assistant_get_clustering(ltrassi);

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
  classification = gtk_ltr_assistant_get_classification(ltrassi);

  ltrtol = gtk_ltr_assistant_get_ltrtol(ltrassi);
  candtol = gtk_ltr_assistant_get_candtol(ltrassi);
  list_view = gtk_ltr_assistant_get_list_view_features(ltrassi);
  sel = gtk_tree_view_get_selection(list_view);
  num_of_features = gtk_tree_selection_count_selected_rows(sel);
  rows = gtk_tree_selection_get_selected_rows(sel, &model);
  features = g_malloc((size_t) (num_of_features + 1) * sizeof(gchar*));
  tmp = rows;
  while (tmp != NULL) {
    gtk_tree_model_get_iter(model, &iter, (GtkTreePath*) tmp->data);
    gtk_tree_model_get(model, &iter,
                       0, &features[i],
                       -1);
    i++;
    tmp = tmp->next;
  }
  features[i] = NULL;
  g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
  g_list_free(rows);

  gtk_project_settings_set_data(GTK_PROJECT_SETTINGS(ltrgui->projset),
                                projectfile, gff3files, indexname, clustering,
                                evalue, dust, gapopen, gapextend, xdrop,
                                penalty, reward, num_threads, wordsize,
                                seqid, psmall, plarge, classification, ltrtol,
                                candtol, features);

  for (i = 0; i < num_of_gff3files; i++)
    g_free(gff3files[i]);
  g_free(gff3files);
  for (i = 0; i < num_of_features; i++)
    g_free(features[i]);
  g_free(features);
}

gboolean pw_update_progress_dialog(gpointer data)
{
  PWThreadData *threaddata = (PWThreadData*) data;
  gtk_progress_bar_set_text(GTK_PROGRESS_BAR(threaddata->ltrgui->progressbar),
                            threaddata->current_state);
  gtk_progress_bar_pulse(GTK_PROGRESS_BAR(threaddata->ltrgui->progressbar));
  return TRUE;
}

static void pw_progress_dialog_init(PWThreadData *threaddata)
{
  GtkWidget *label,
            *vbox;
  guint sid;

  /* create the modal window which warns the user to wait */
  threaddata->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_modal(GTK_WINDOW(threaddata->window), TRUE);
  gtk_window_set_title(GTK_WINDOW(threaddata->window), "Progress");
  gtk_window_resize(GTK_WINDOW(threaddata->window), 200, 50);
  gtk_container_set_border_width(GTK_CONTAINER(threaddata->window), 12);
  g_signal_connect(threaddata->window, "delete_event",
                   G_CALLBACK(gtk_true), NULL);
  vbox = gtk_vbox_new(FALSE, 12);
  /* create label */
  label = gtk_label_new("Please wait...");
  gtk_container_add(GTK_CONTAINER(vbox), label);
  /* add vbox to dialog */
  gtk_container_add(GTK_CONTAINER(threaddata->window), vbox);
  gtk_widget_show_all(threaddata->window);
  /* refresh the progress dialog */
  sid = g_timeout_add(100, pw_update_progress_dialog,
                      (gpointer) threaddata);
  g_object_set_data(G_OBJECT(threaddata->window),
                    "source_id", GINT_TO_POINTER(sid));
  gtk_widget_show(threaddata->ltrgui->progressbar);
}

static gboolean assistant_finish(gpointer data)
{
  PWThreadData *threaddata = (PWThreadData*) data;
  GtkWidget *ltrfams = threaddata->ltrgui->ltr_families;

  g_source_remove(GPOINTER_TO_INT(
                               g_object_get_data(G_OBJECT(threaddata->window),
                                                 "source_id")));
  gtk_widget_destroy(threaddata->window);
  reset_progressbar(threaddata->ltrgui->progressbar);
  if (!threaddata->had_err) {
    gtk_widget_destroy(ltrfams);
    threaddata->ltrgui->ltr_families =
                              gtk_ltr_families_new(threaddata->ltrgui->sb_main);
    ltrfams = threaddata->ltrgui->ltr_families;
    gtk_box_pack_start(GTK_BOX(threaddata->ltrgui->vbox1_main), ltrfams,
                       TRUE, TRUE, 0);
    gtk_ltr_families_fill_with_data(GTK_LTR_FAMILIES(ltrfams),
                                    threaddata->nodes,
                                    threaddata->features,
                                    threaddata->n_features);
    mb_main_view_columns_set_submenu(threaddata->ltrgui, threaddata->features,
                                     threaddata->err, FALSE);
    mb_main_activate_menuitems(threaddata->ltrgui);
    gtk_ltr_families_set_projectfile(GTK_LTR_FAMILIES(ltrfams),
                                     g_strdup(threaddata->fullname));
    extract_project_settings(threaddata->ltrgui);
    mb_main_file_save_activate(NULL, threaddata->ltrgui);
    gtk_ltr_families_set_modified(GTK_LTR_FAMILIES(ltrfams), FALSE);
  }
  g_free(threaddata->projectfile);
  g_free(threaddata->projectdir);
  gt_free(threaddata->current_state);
  gt_error_delete(threaddata->err);
  g_slice_free(PWThreadData, threaddata);

  return FALSE;
}

static gpointer assistant_start(gpointer data)
{
  PWThreadData *threaddata = (PWThreadData*) data;
  GtkWidget *ltrassi = threaddata->ltrgui->assistant;
  GtkTreeSelection *sel;
  GtkTreeView *list_view;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GList *rows,
        *tmp;
  GtStr *tmpdirprefix;
  GtNodeStream *last_stream = NULL,
               *gff3_in_stream = NULL,
               *ltr_cluster_stream = NULL,
               *ltr_classify_stream = NULL,
               *ltrgui_preprocess_stream = NULL,
               *ltrgui_array_out_stream = NULL;
  GtEncseqLoader *el = NULL;
  GtEncseq *encseq = NULL;
  GtArray *nodes;
  GtHashmap *features,
            *sel_features = NULL;
  gint psmall,
       plarge,
       gapopen,
       gapextend,
       wordsize,
       penalty,
       reward,
       num_threads,
       i = 0,
       num_of_files;
  char buf[BUFSIZ];
  const char **gff3_files,
             *indexname;
  gboolean dust;
  gdouble evalue,
          seqid,
          xdrop;

  g_snprintf(buf, BUFSIZ, "%s/tmp/%s", threaddata->projectdir,
             threaddata->projectfile);
  tmpdirprefix = gt_str_new_cstr(buf);

  evalue = gtk_ltr_assistant_get_evalue(GTK_LTR_ASSISTANT(ltrassi));
  dust = gtk_ltr_assistant_get_dust(GTK_LTR_ASSISTANT(ltrassi));
  gapopen = gtk_ltr_assistant_get_gapopen(GTK_LTR_ASSISTANT(ltrassi));
  if (gapopen == 0)
    gapopen = GT_UNDEF_INT;
  gapextend = gtk_ltr_assistant_get_gapextend(GTK_LTR_ASSISTANT(ltrassi));
  if (gapextend == 0)
    gapextend = GT_UNDEF_INT;
  xdrop = gtk_ltr_assistant_get_xdrop(GTK_LTR_ASSISTANT(ltrassi));
  penalty = gtk_ltr_assistant_get_penalty(GTK_LTR_ASSISTANT(ltrassi));
  /* according to BLASTN help -penalty must be <=0 but BLASTN shows an error:
     BLASTN penalty must be negative. Therefore -penalty will be omitted when
     the value is zero */
  if (penalty == 0)
    penalty = GT_UNDEF_INT;
  reward = gtk_ltr_assistant_get_reward(GTK_LTR_ASSISTANT(ltrassi));
  if (reward == 0)
    reward = GT_UNDEF_INT;
  num_threads = gtk_ltr_assistant_get_threads(GTK_LTR_ASSISTANT(ltrassi));
  wordsize = gtk_ltr_assistant_get_wordsize(GTK_LTR_ASSISTANT(ltrassi));
  seqid = gtk_ltr_assistant_get_seqid(GTK_LTR_ASSISTANT(ltrassi));

  psmall = gtk_ltr_assistant_get_psmall(GTK_LTR_ASSISTANT(ltrassi));
  plarge = gtk_ltr_assistant_get_plarge(GTK_LTR_ASSISTANT(ltrassi));

  list_view =
          gtk_ltr_assistant_get_list_view_gff3files(GTK_LTR_ASSISTANT(ltrassi));
  sel = gtk_tree_view_get_selection(list_view);
  gtk_tree_selection_select_all(sel);
  num_of_files = gtk_tree_selection_count_selected_rows(sel);
  rows = gtk_tree_selection_get_selected_rows(sel, &model);
  tmp = rows;
  gff3_files = g_malloc((size_t) num_of_files * sizeof(const char*));
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

  last_stream = gff3_in_stream = gt_gff3_in_stream_new_unsorted(num_of_files,
                                                                gff3_files);

  if (gtk_ltr_assistant_get_clustering(GTK_LTR_ASSISTANT(ltrassi))) {
    indexname = gtk_ltr_assistant_get_indexname(GTK_LTR_ASSISTANT(ltrassi));
    el = gt_encseq_loader_new();
    encseq = gt_encseq_loader_load(el, indexname, threaddata->err);
    if (!encseq)
      threaddata->had_err = -1;
    if (!threaddata->had_err)
      last_stream = ltr_cluster_stream = gt_ltr_cluster_stream_new(last_stream,
                                                                   encseq,
                                                                   tmpdirprefix,
                                                                   plarge,
                                                                   psmall,
                                                                   evalue,
                                                                   dust,
                                                                   wordsize,
                                                                   gapopen,
                                                                   gapextend,
                                                                   penalty,
                                                                   reward,
                                                                   num_threads,
                                                                   xdrop,
                                                                   seqid,
                                                                   &threaddata->current_state,
                                                                   threaddata->err);
  }
  if (!threaddata->had_err &&
      gtk_ltr_assistant_get_clustering(GTK_LTR_ASSISTANT(ltrassi))) {
    GtkTreeView *list_view;
    GtkTreeModel *model;
    GtkTreeSelection *sel;
    GtkTreeIter iter;
    GList *rows;
    gchar *feature_name;
    sel_features = gt_hashmap_new(GT_HASH_STRING, free_gt_hash_elem, NULL);
    list_view =
           gtk_ltr_assistant_get_list_view_features(GTK_LTR_ASSISTANT(ltrassi));
    sel = gtk_tree_view_get_selection(list_view);
    rows = gtk_tree_selection_get_selected_rows(sel, &model);
    tmp = rows;
    while (tmp != NULL) {
      gtk_tree_model_get_iter(model, &iter, (GtkTreePath*) tmp->data);
      gtk_tree_model_get(model, &iter,
                         0, &feature_name,
                         -1);
      gt_hashmap_add(sel_features, (void*) gt_cstr_dup(feature_name),
                     (void*) 1);
      g_free(feature_name);
      tmp = tmp->next;
    }
    g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
    g_list_free(rows);

    last_stream = ltr_classify_stream = gt_ltr_classify_stream_new(last_stream,
                                                                   sel_features,
                                                                   &threaddata->current_state,
                                                                   NULL,
                                                                   threaddata->err);
  }
  if (!threaddata->had_err) {
    nodes = gt_array_new(sizeof(GtGenomeNode*));
    features = gt_hashmap_new(GT_HASH_STRING, free_gt_hash_elem, NULL);
    last_stream = ltrgui_preprocess_stream =
                                    gt_ltrgui_preprocess_stream_new(last_stream,
                                                                    features,
                                                                    &threaddata->n_features,
                                                                    FALSE,
                                                                    threaddata->err);
    last_stream = ltrgui_array_out_stream = gt_array_out_stream_new(last_stream,
                                                                    nodes,
                                                                    threaddata->err);
  }
  if (!ltrgui_array_out_stream)
    threaddata->had_err = -1;
  if (!threaddata->had_err)
    threaddata->had_err = gt_node_stream_pull(last_stream, threaddata->err);

  if (!threaddata->had_err) {
    threaddata->nodes = nodes;
    threaddata->features = features;
  }
  gt_node_stream_delete(ltr_classify_stream);
  gt_node_stream_delete(ltr_cluster_stream);
  gt_node_stream_delete(gff3_in_stream);
  gt_node_stream_delete(ltrgui_array_out_stream);
  gt_node_stream_delete(ltrgui_preprocess_stream);
  gt_encseq_loader_delete(el);
  gt_encseq_delete(encseq);
  gt_hashmap_delete(sel_features);

  for (i = 0; i < num_of_files; i++)
    g_free((gpointer) gff3_files[i]);
  g_free(gff3_files);

  g_idle_add(assistant_finish, data);
  return NULL;
}

void pw_apply(GtkAssistant *assistant, GUIData *ltrgui)
{
  GtkWidget *dialog;

  const gchar *fullname;
  gchar *projectdir,
        *projectfile,
        projecttmpdir[BUFSIZ];
  gint had_err = 0;

  gtk_widget_hide(GTK_WIDGET(assistant));
  fullname = gtk_ltr_assistant_get_projectfile(GTK_LTR_ASSISTANT(assistant));
  projectfile = g_path_get_basename(fullname);
  projectdir = g_path_get_dirname(fullname);
  if (g_file_test(fullname, G_FILE_TEST_EXISTS)) {
    gchar buffer[BUFSIZ];
    g_snprintf(buffer, BUFSIZ, FILE_EXISTS_DIALOG, projectfile);
    dialog = gtk_message_dialog_new(NULL,
                                    GTK_DIALOG_MODAL |
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
                                    "%s",
                                    buffer);
    gtk_window_set_title(GTK_WINDOW(dialog), "Attention!");
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_NO) {
      gtk_widget_destroy(dialog);
      gtk_widget_show(GTK_WIDGET(assistant));
      gtk_assistant_set_current_page(assistant, 0);
      g_free(projectdir);
      g_free(projectfile);
      return;
    } else
      gtk_widget_destroy(dialog);
  }
  g_snprintf(projecttmpdir, BUFSIZ, "%s/tmp", projectdir);
  if (!g_file_test(projecttmpdir, G_FILE_TEST_EXISTS))
    had_err = g_mkdir(projecttmpdir, 0755);
  if (had_err != 0) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not make dir: %s",
                projecttmpdir);
    error_handle(ltrgui);
    return;
  }

  PWThreadData *threaddata;
  threaddata = g_slice_new(PWThreadData);
  threaddata->ltrgui = ltrgui;
  threaddata->had_err = 0;
  threaddata->fullname = fullname;
  threaddata->projectfile = projectfile;
  threaddata->projectdir = projectdir;
  threaddata->n_features = LTRFAMS_LV_N_COLUMS;
  threaddata->current_state = gt_cstr_dup("Starting...");
  threaddata->err = gt_error_new();
  pw_progress_dialog_init(threaddata);

  g_thread_create(assistant_start, (gpointer) threaddata, FALSE, NULL);
}

