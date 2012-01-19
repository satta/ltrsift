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

#include <math.h>
#include "error.h"
#include "support.h"

gboolean entry_in_list_view(GtkTreeModel *model, const gchar *entry,
                            gint column_no)
{
  GtkTreeIter iter;
  gchar *name;
  gboolean valid;

  valid = gtk_tree_model_get_iter_first(model, &iter);
  if (valid) {
    gtk_tree_model_get(model, &iter, column_no, &name, -1);
    if (g_strcmp0(name, entry) == 0) {
      g_free(name);
      return TRUE;
    }
    while (gtk_tree_model_iter_next(model, &iter)) {
      gtk_tree_model_get(model, &iter, column_no, &name, -1);
      if (g_strcmp0(name, entry) == 0) {
        g_free(name);
        return TRUE;
      }
      g_free(name);
    }
  }
  return FALSE;
}

void create_recently_used_resource(const gchar *filename)
{
  GtkRecentManager *manager;
  GtkRecentData *data;
  static gchar *groups[2] = {GUI_RECENT_GROUP, NULL};
  gchar *uri;

  /* Create a new recently used resource. */
  data = g_slice_new(GtkRecentData);
  data->display_name = NULL;
  data->description = NULL;
  data->mime_type = "text/plain";
  data->app_name = (gchar*) g_get_application_name();
  data->app_exec = g_strjoin(" ", g_get_prgname(), "%u", NULL);
  data->groups = groups;
  data->is_private = FALSE;
  uri = g_filename_to_uri(filename, NULL, NULL);
  manager = gtk_recent_manager_get_default();
  gtk_recent_manager_add_full(manager, uri, data);
  g_free(uri);
  g_free(data->app_exec);
  g_slice_free(GtkRecentData, data);
}

void free_gt_hash_elem(void *elem)
{
  gt_free(elem);
}

void reset_progressbar(GtkWidget *progressbar)
{
  gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progressbar), "");
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressbar), 0.0);
  gtk_widget_hide(progressbar);
}

GtkWidget* unsaved_changes_dialog(GUIData *ltrgui, const gchar *text)
{
  GtkWidget *dialog,
            *label,
            *image,
            *hbox;

  dialog = gtk_dialog_new_with_buttons("Close project - LTRGui",
                                       GTK_WINDOW(ltrgui->main_window),
                                       GTK_DIALOG_MODAL |
                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                       GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                       GTK_STOCK_DISCARD, GTK_RESPONSE_REJECT,
                                       GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                       NULL);
  label = gtk_label_new(text);
  image = gtk_image_new_from_stock(GTK_STOCK_DIALOG_INFO,
                                   GTK_ICON_SIZE_DIALOG);
  hbox = gtk_hbox_new(FALSE, 5);
  gtk_container_set_border_width(GTK_CONTAINER(hbox), 10);
  gtk_box_pack_start_defaults(GTK_BOX(hbox), image);
  gtk_box_pack_start_defaults(GTK_BOX(hbox), label);
  gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG (dialog)->vbox), hbox);
  gtk_widget_show_all(dialog);

  return dialog;
}

static gboolean update_progress_dialog(gpointer data)
{
  ThreadData *threaddata = (ThreadData*) data;

  if (threaddata->classification) {
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(threaddata->progressbar),
                              threaddata->current_state);
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(threaddata->progressbar),
                                  (gdouble) threaddata->progress /
                                  (2 * gt_array_size(threaddata->old_nodes)));
  } else if (threaddata->save || threaddata->save_as) {
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(threaddata->progressbar),
                                  (gdouble) threaddata->progress /
                                  gt_array_size(threaddata->nodes));
  } else if (threaddata->projectw) {
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(threaddata->progressbar),
                              threaddata->current_state);
    gtk_progress_bar_pulse(GTK_PROGRESS_BAR(threaddata->progressbar));
  } else
    gtk_progress_bar_pulse(GTK_PROGRESS_BAR(threaddata->progressbar));

  return TRUE;
}

void progress_dialog_init(ThreadData *threaddata, GtkWidget *toplevel)
{
  GtkWidget *label,
            *vbox;
  guint sid;

  /* create the modal window which warns the user to wait */
  threaddata->window = gtk_window_new(GTK_WINDOW_POPUP);
  gtk_window_set_transient_for(GTK_WINDOW(threaddata->window),
                               GTK_WINDOW(toplevel));
  gtk_window_set_position(GTK_WINDOW(threaddata->window),
                          GTK_WIN_POS_CENTER_ON_PARENT);
  gtk_window_set_modal(GTK_WINDOW(threaddata->window), TRUE);
  gtk_window_set_title(GTK_WINDOW(threaddata->window), "Progress");
  gtk_window_resize(GTK_WINDOW(threaddata->window), 200, 50);
  gtk_container_set_border_width(GTK_CONTAINER(threaddata->window), 5);
  g_signal_connect(threaddata->window, "delete_event",
                   G_CALLBACK(gtk_true), NULL);
  vbox = gtk_vbox_new(FALSE, 5);
  /* create label */
  label = gtk_label_new("Please wait...");
  gtk_container_add(GTK_CONTAINER(vbox), label);
  /* add vbox to dialog */
  gtk_container_add(GTK_CONTAINER(threaddata->window), vbox);
  gtk_widget_show_all(threaddata->window);
  /* refresh the progress dialog */
  sid = g_timeout_add(100, update_progress_dialog, (gpointer) threaddata);
  g_object_set_data(G_OBJECT(threaddata->window),
                    "source_id", GINT_TO_POINTER(sid));
  gtk_widget_show(threaddata->progressbar);
}

void threaddata_delete(ThreadData *threaddata)
{
  if (threaddata->classification) {
    g_list_foreach(threaddata->references,
                   (GFunc) gtk_tree_row_reference_free,
                   NULL);
    g_list_free(threaddata->references);
    gt_array_delete(threaddata->old_nodes);
    gt_error_delete(threaddata->err);
    gt_free(threaddata->current_state);
    g_free(threaddata->fam_prefix);
    gt_hashmap_delete(threaddata->sel_features);
  } else if (threaddata->open) {
    gt_error_delete(threaddata->err);
  } else if (threaddata->save_as || threaddata->save) {
    gt_error_delete(threaddata->err);
    g_free(threaddata->tmp_filename);
  } else if (threaddata->projectw) {
    g_free(threaddata->projectfile);
    g_free(threaddata->projectdir);
    gt_free(threaddata->current_state);
    gt_error_delete(threaddata->err);
  }
  g_slice_free(ThreadData, threaddata);
}

ThreadData* threaddata_new()
{
  ThreadData *threaddata;

  threaddata = g_slice_new(ThreadData);
  threaddata->ltrgui = NULL;
  threaddata->ltrfams = NULL;
  threaddata->window = NULL;
  threaddata->progressbar = NULL;
  threaddata->list_view = NULL;
  threaddata->references = NULL;
  threaddata->nodes = NULL;
  threaddata->old_nodes = NULL;
  threaddata->new_nodes = NULL;
  threaddata->err = NULL;
  threaddata->sel_features = NULL;
  threaddata->features = NULL;
  threaddata->classification = FALSE;
  threaddata->projectw = FALSE;
  threaddata->save = FALSE;
  threaddata->save_as = FALSE;
  threaddata->open = FALSE;
  threaddata->bakfile = FALSE;
  threaddata->current_state = NULL;
  threaddata->filename = NULL;
  threaddata->tmp_filename = NULL;
  threaddata->projectfile = NULL;
  threaddata->projectdir = NULL;
  threaddata->fullname = NULL;
  threaddata->ltrtolerance = 0.0;
  threaddata->lentolerance = 0.0;
  threaddata->fam_prefix = NULL;
  threaddata->had_err = 0;
  threaddata->progress = 0;
  threaddata->n_features = 0;

  return threaddata;
}

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
              *indexname,
              *moreblast;
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
  if (!g_str_has_suffix(projectfile, SQLITE_PATTERN)) {
    projectfile = g_strconcat(projectfile, SQLITE_PATTERN, NULL);
  }
  list_view = gtk_ltr_assistant_get_list_view_gff3files(ltrassi);
  sel = gtk_tree_view_get_selection(list_view);
  gtk_tree_selection_select_all(sel);
  num_of_gff3files = gtk_tree_selection_count_selected_rows(sel);
  rows = gtk_tree_selection_get_selected_rows(sel, &model);
  gff3files = g_malloc((size_t) (num_of_gff3files + 1) * sizeof (gchar*));
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
  moreblast = gtk_ltr_assistant_get_moreblast(ltrassi);
  psmall = gtk_ltr_assistant_get_psmall(ltrassi);
  plarge = gtk_ltr_assistant_get_plarge(ltrassi);
  classification = gtk_ltr_assistant_get_classification(ltrassi);

  ltrtol = gtk_ltr_assistant_get_ltrtol(ltrassi);
  candtol = gtk_ltr_assistant_get_lentol(ltrassi);
  list_view = gtk_ltr_assistant_get_list_view_features(ltrassi);
  sel = gtk_tree_view_get_selection(list_view);
  num_of_features = gtk_tree_selection_count_selected_rows(sel);
  rows = gtk_tree_selection_get_selected_rows(sel, &model);
  features = g_malloc((size_t) (num_of_features + 1) * sizeof (gchar*));
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
                                seqid, moreblast, psmall, plarge,
                                classification, ltrtol, candtol, features);

  for (i = 0; i < num_of_gff3files; i++)
    g_free(gff3files[i]);
  g_free(gff3files);
  for (i = 0; i < num_of_features; i++)
    g_free(features[i]);
  g_free(features);
}

GtArray* create_region_nodes_from_node_array(GtArray *nodes)
{
  GtArray *region_nodes;
  GtGenomeNode *gn, *rn, *tmp_gn;
  GtStr *current_seqid = NULL,
        *new_seqid;
  unsigned long i, start, end;

  region_nodes = gt_array_new(sizeof (GtGenomeNode*));
  gt_genome_nodes_sort_stable(nodes);

  for (i = 0; i < gt_array_size(nodes); i++)
  {
    gn = *(GtGenomeNode**) gt_array_get(nodes, i);
    if (i == 0) {
      current_seqid = gt_genome_node_get_seqid(gn);
      start = gt_genome_node_get_start(gn);
      end = gt_genome_node_get_end(gn);
    }
    new_seqid = gt_genome_node_get_seqid(gn);
    if (gt_str_cmp(current_seqid, new_seqid) != 0) {
      tmp_gn = *(GtGenomeNode**) gt_array_get(nodes, i - 1);
      end = gt_genome_node_get_end(tmp_gn);
      rn = gt_region_node_new(current_seqid, start, end);
      gt_array_add(region_nodes, rn);
      start = gt_genome_node_get_start(gn);
      end = gt_genome_node_get_end(gn);
      current_seqid = gt_genome_node_get_seqid(gn);
    }
    if (i == (gt_array_size(nodes) - 1)) {
      end = gt_genome_node_get_end(gn);
      rn = gt_region_node_new(current_seqid, start, end);
      gt_array_add(region_nodes, rn);
    }
  }
  return region_nodes;
}

void export_annotation(GtArray *nodes, gchar *filen, gboolean flcands,
                       GtkWidget *toplevel, GError *gerr)
{
  GtkWidget *dialog;
  GtArray *region_nodes,
          *export_nodes;
  GtGenomeNode *gn;
  GtFeatureNode *curnode;
  GtFeatureNodeIterator *fni;
  GtNodeStream *array_in_stream = NULL,
               *gff3_out_stream = NULL;
  GtFile *outfp;
  GtError *err;
  gchar *filename,
        tmp_filename[BUFSIZ];
  const char *attr;
  int had_err = 0;
  unsigned long i;
  gboolean bakfile = FALSE;

  if (!g_str_has_suffix(filen, GFF3_PATTERN))
    filename = g_strconcat(filen, GFF3_PATTERN, NULL);
  else
    filename = filen;

  if (g_file_test(filename, G_FILE_TEST_EXISTS)) {
    gchar buffer[BUFSIZ];
    g_snprintf(buffer, BUFSIZ, FILE_EXISTS_DIALOG,
               g_path_get_basename(filename));
    dialog = gtk_message_dialog_new(NULL,
                                    GTK_DIALOG_MODAL |
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
                                    "%s",
                                    buffer);
    gtk_window_set_title(GTK_WINDOW(dialog), "Attention!");
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) != GTK_RESPONSE_YES) {
      gtk_widget_destroy(dialog);
      return;
    } else {
      g_snprintf(tmp_filename, BUFSIZ, "%s.bak", filename);
      g_rename(filename, tmp_filename);
      bakfile = TRUE;
    }
    gtk_widget_destroy(dialog);
  }

  err = gt_error_new();
  outfp = gt_file_new(filename, "w", err);

  if (flcands) {
    GtArray *tmp = gt_array_new(sizeof (GtGenomeNode*));
    for (i = 0; i < gt_array_size(nodes); i++) {
      gn = *(GtGenomeNode**) gt_array_get(nodes, i);
      fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);
      curnode = gt_feature_node_iterator_next(fni);
      attr = gt_feature_node_get_attribute(curnode, ATTR_FULLLEN);
      if (attr)
        gt_array_add(tmp, gn);
      gt_feature_node_iterator_delete(fni);
    }
    region_nodes = create_region_nodes_from_node_array(tmp);
    export_nodes = gt_array_new(sizeof (GtGenomeNode*));
    gt_array_add_array(export_nodes, region_nodes);
    gt_array_add_array(export_nodes, tmp);
    gt_array_delete(tmp);
  } else {
    region_nodes = create_region_nodes_from_node_array(nodes);
    export_nodes = gt_array_new(sizeof (GtGenomeNode*));
    gt_array_add_array(export_nodes, region_nodes);
    gt_array_add_array(export_nodes, nodes);
  }

  for (i = 0; i < gt_array_size(export_nodes); i++) {
    GtGenomeNode *gn;
    gn = gt_genome_node_ref(*(GtGenomeNode**) gt_array_get(export_nodes, i));
  }

  array_in_stream = gt_array_in_stream_new(export_nodes, NULL, err);
  gff3_out_stream = gt_gff3_out_stream_new(array_in_stream, outfp);

  had_err = gt_node_stream_pull(gff3_out_stream, err);
  gt_file_delete(outfp);

  if (had_err) {
    if (bakfile)
      g_rename(tmp_filename, filename);
    g_set_error(&gerr,
                G_FILE_ERROR,
                0,
                "Error while exporting data: %s",
                gt_error_get(err));
   error_handle(toplevel, gerr);
  }
  gt_node_stream_delete(array_in_stream);
  gt_node_stream_delete(gff3_out_stream);
  gt_array_delete(export_nodes);
  for (i = 0; i < gt_array_size(region_nodes); i++)
    gt_genome_node_delete(*(GtGenomeNode**) gt_array_get(region_nodes, i));
  gt_array_delete(region_nodes);
  gt_error_delete(err);
}

void export_sequences(GtArray *nodes, gchar *filen, const gchar *indexname,
                      gboolean flcands, GtkWidget *toplevel, GError *gerr)
{
  GtkWidget *dialog;
  GtStr *seqid;
  GtGenomeNode *gn;
  GtFeatureNode *curnode;
  GtFeatureNodeIterator *fni;
  GtRange range;
  GtFile *outfp;
  GtEncseqLoader *el = NULL;
  GtEncseq *encseq = NULL;
  GtError *err;
  gchar *filename,
        tmp_filename[BUFSIZ];
  char *buffer, header[BUFSIZ];
  const char *attr;
  gint had_err = 0;
  unsigned long i,
                seqnum,
                startpos;
  gboolean bakfile = FALSE;

  if (!g_str_has_suffix(filen, FAS_PATTERN)) {
    filename = g_strconcat(filen, FAS_PATTERN, NULL);
  } else
    filename = filen;

  if (g_file_test(filename, G_FILE_TEST_EXISTS)) {
    gchar buffer[BUFSIZ];
    g_snprintf(buffer, BUFSIZ, FILE_EXISTS_DIALOG,
               g_path_get_basename(filename));
    dialog = gtk_message_dialog_new(GTK_WINDOW(toplevel),
                                    GTK_DIALOG_MODAL |
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
                                    "%s",
                                    buffer);
    gtk_window_set_title(GTK_WINDOW(dialog), "Attention!");
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_NO) {
      gtk_widget_destroy(dialog);
      return;
    } else {
      g_snprintf(tmp_filename, BUFSIZ, "%s.bak", filename);
      g_rename(filename, tmp_filename);
      bakfile = TRUE;
    }
    gtk_widget_destroy(dialog);
  }

  err = gt_error_new();
  outfp = gt_file_new(filename, "w", err);
  if (!outfp)
    had_err = -1;

  if (!had_err) {
    el = gt_encseq_loader_new();
    encseq = gt_encseq_loader_load(el, indexname, err);
    if (!encseq)
      had_err = -1;
  }

  if (!had_err) {
    for (i = 0; i < gt_array_size(nodes); i++) {
      gn = *(GtGenomeNode**) gt_array_get(nodes, i);
      fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);
      curnode = gt_feature_node_iterator_next(fni);
      seqid = gt_genome_node_get_seqid((GtGenomeNode*) curnode);
      range = gt_genome_node_get_range((GtGenomeNode*) curnode);
      if (flcands) {
        attr = gt_feature_node_get_attribute(curnode, ATTR_FULLLEN);
        if (!attr) {
          gt_feature_node_iterator_delete(fni);
          continue;
        }
      }
      attr = gt_feature_node_get_attribute(curnode, "ltrfam");
      if (attr)
        g_snprintf(header, BUFSIZ, "%s_%s_%lu_%lu", attr, gt_str_get(seqid),
                   range.start, range.end);
      else
        g_snprintf(header, BUFSIZ, "%s_%lu_%lu", gt_str_get(seqid),
                   range.start, range.end);
      sscanf(gt_str_get(seqid), "seq%lu", &seqnum);
      buffer = gt_calloc((size_t) gt_range_length(&range) + 1, sizeof (char));
      startpos = gt_encseq_seqstartpos(encseq, seqnum);
      gt_encseq_extract_decoded(encseq, buffer, startpos + range.start - 1,
                                startpos + range.end - 1);
      gt_fasta_show_entry(header, buffer, gt_range_length(&range), 50, outfp);
      gt_free(buffer);
      gt_feature_node_iterator_delete(fni);
    }
    gt_file_delete(outfp);
  }

  if (had_err) {
    if (bakfile)
      g_rename(tmp_filename, filename);
    g_set_error(&gerr,
                G_FILE_ERROR,
                0,
                "Error while exporting data: %s",
                gt_error_get(err));
    error_handle(toplevel, gerr);
  }
  gt_error_delete(err);
}

static int compare_gfloat(const void *a, const void *b)
{
  gfloat num1 = *(gfloat*) a,
        num2 = *(gfloat*) b;

  if (num1 < num2)
    return -1;
  else if (num1 > num2)
    return 1;
  else
    return 0;
}

static gfloat calculate_median(GtArray *gfloat_array)
{
  GtArray *gfloat_array_clone;
  gfloat median;
  unsigned long n = gt_array_size(gfloat_array);

  gfloat_array_clone = gt_array_clone(gfloat_array);
  gt_array_sort_stable(gfloat_array_clone, (GtCompare) compare_gfloat);

  if (n % 2 == 0)
    median = (((*(gfloat*) gt_array_get(gfloat_array_clone, n / 2)) +
               (*(gfloat*) gt_array_get(gfloat_array_clone, (n / 2) - 1)))
              / 2.0);
  else
    median = (*(gfloat*) gt_array_get(gfloat_array_clone, n / 2));

  gt_array_delete(gfloat_array_clone);

  return median;
}

static void fl_cands_ltr_and_elem_length_median(GtArray *nodes,
                                                GtArray *fl_cands_ltr_length,
                                                GtArray *fl_cands_elem_length,
                                                gfloat *fl_cands_ltrlen_median,
                                                gfloat *fl_cands_elemlen_median)
{
  GtGenomeNode *gn;
  GtFeatureNode *curnode;
  GtFeatureNodeIterator *fni;
  gfloat length;
  const char *fnt;
  unsigned long i;

  for (i = 0; i < gt_array_size(nodes); i++) {
    gn = *(GtGenomeNode**) gt_array_get(nodes, i);
    fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);
    while ((curnode = gt_feature_node_iterator_next(fni))) {
      fnt = gt_feature_node_get_type(curnode);
      if (g_strcmp0(fnt, FNT_LTRRETRO) == 0) {
        length = (gfloat) gt_genome_node_get_length((GtGenomeNode*) curnode);
        gt_array_add(fl_cands_ltr_length, length);
      } else if (g_strcmp0(fnt, FNT_LTR) == 0) {
        length = (gfloat) gt_genome_node_get_length((GtGenomeNode*) curnode);
        gt_array_add(fl_cands_elem_length, length);
        break;
      }
    }
    gt_feature_node_iterator_delete(fni);
  }
  *fl_cands_ltrlen_median = calculate_median(fl_cands_ltr_length);
  *fl_cands_elemlen_median = calculate_median(fl_cands_elem_length);
}

void mark_genomenode_as_flcand(GtGenomeNode *gn)
{
  GtFeatureNode *curnode;
  GtFeatureNodeIterator *fni;

  fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);
  curnode = gt_feature_node_iterator_next(fni);
  gt_feature_node_set_attribute(curnode, ATTR_FULLLEN, "yes");
  gt_feature_node_iterator_delete(fni);
}

unsigned long determine_full_length_candidates(GtArray *nodes,
                                               gfloat ltrtolerance,
                                               gfloat lentolerance)
{
  GtGenomeNode *gn;
  GtFeatureNode *curnode;
  GtFeatureNodeIterator *fni;
  GtArray *num_domains_fam,
          *fl_cands,
          *fl_cands_ltr_length,
          *fl_cands_elem_length;
  const char *fnt;
  gfloat fl_cands_ltrlen_median,
         fl_cands_elemlen_median,
         fl_cand_elem_length,
         fl_cand_ltr_length;
  unsigned long i,
                num_domains_cand,
                max_num_domains = 0,
                most_freq_num_domains = 0,
                cur_num_domains,
                old_num_domains = 0,
                flcands = 0;

  num_domains_fam = gt_array_new(sizeof (unsigned long));

  for (i = 0; i < gt_array_size(nodes); i++) {
    gn = *(GtGenomeNode**) gt_array_get(nodes, i);
    fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);
    while ((curnode = gt_feature_node_iterator_next(fni))) {
      fnt = gt_feature_node_get_type(curnode);
      if (g_strcmp0(fnt, FNT_REPEATR) == 0) {
        const char *attr = gt_feature_node_get_attribute(curnode,
                                                         ATTR_FULLLEN);
        if (attr)
          gt_feature_node_remove_attribute(curnode, ATTR_FULLLEN);
      } else if (g_strcmp0(fnt, FNT_LTRRETRO) == 0) {
        num_domains_cand = gt_feature_node_number_of_children(curnode);
        if (num_domains_cand > max_num_domains)
          max_num_domains = num_domains_cand;
        break;
      }
    }
    gt_feature_node_iterator_delete(fni);
    gt_array_add(num_domains_fam, num_domains_cand);
  }

  unsigned long num_domains_freq[max_num_domains];

  for (i = 0; i < max_num_domains; i++) {
    num_domains_freq[i] = 0;
  }

  for (i = 0; i < gt_array_size(num_domains_fam); i++) {
    num_domains_cand = *(unsigned long*) gt_array_get(num_domains_fam, i);
    num_domains_freq[num_domains_cand - 1]++;
  }

  for (i = 0; i < max_num_domains; i++) {
    cur_num_domains = num_domains_freq[i];
    if (cur_num_domains >= old_num_domains) {
      most_freq_num_domains = i + 1;
      old_num_domains = num_domains_freq[i];
    }
  }

  fl_cands = gt_array_new(sizeof (GtGenomeNode*));
  for (i = 0; i < gt_array_size(num_domains_fam); i++) {
    num_domains_cand = *(unsigned long*) gt_array_get(num_domains_fam, i);
    if (num_domains_cand == most_freq_num_domains) {
      gn = *(GtGenomeNode**) gt_array_get(nodes, i);
      gt_array_add(fl_cands, gn);
    }
  }
  gt_array_delete(num_domains_fam);

  fl_cands_ltr_length = gt_array_new(sizeof (gfloat));
  fl_cands_elem_length = gt_array_new(sizeof (gfloat));
  fl_cands_ltr_and_elem_length_median(fl_cands,
                                      fl_cands_ltr_length,
                                      fl_cands_elem_length,
                                      &fl_cands_ltrlen_median,
                                      &fl_cands_elemlen_median);

  for (i = 0; i < gt_array_size(fl_cands); i++) {
    fl_cand_ltr_length = *(gfloat*) gt_array_get(fl_cands_ltr_length, i);
    fl_cand_elem_length = *(gfloat*) gt_array_get(fl_cands_elem_length, i);
    if ((fabsf(fl_cand_ltr_length - fl_cands_ltrlen_median) <= ltrtolerance)
        && (fabsf(fl_cand_elem_length - fl_cands_elemlen_median) <=
            lentolerance)) {
      gn = *(GtGenomeNode**) gt_array_get(fl_cands, i);
      mark_genomenode_as_flcand(gn);
      flcands++;
    }
  }

  gt_array_delete(fl_cands_elem_length);
  gt_array_delete(fl_cands_ltr_length);
  gt_array_delete(fl_cands);

  return flcands;
}
