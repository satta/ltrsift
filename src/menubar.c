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

#include <stdio.h>
#include "error.h"
#include "menubar.h"
#include "message_strings.h"
#include "project_wizard.h"
#include "statusbar.h"
#include "support.h"

static int get_keys(void *key, GT_UNUSED void *values, void *cptns,
                    GT_UNUSED GtError *err)
{
  GtStrArray *captions = (GtStrArray*) cptns;
  const char *caption = (const char*) key;

  gt_str_array_add_cstr(captions, caption);

  return 0;
}

static void activate_menuitems(GUIData *ltrgui)
{
  gtk_widget_set_sensitive(ltrgui->menubar_save, TRUE);
  gtk_widget_set_sensitive(ltrgui->menubar_save_as, TRUE);
  gtk_widget_set_sensitive(ltrgui->menubar_export, TRUE);
  gtk_widget_set_sensitive(ltrgui->menubar_close, TRUE);
  gtk_widget_set_sensitive(ltrgui->menubar_view_columns, TRUE);
  gtk_widget_set_sensitive(ltrgui->menubar_project, TRUE);
}

static void view_columns_toggled(GtkCheckMenuItem *menuitem,
                                 gpointer user_data)
{
  GtkWidget *sw;
  GtkTreeView *list_view;
  GtkTreeViewColumn *column;
  GtkNotebook *notebook;
  GList *children,
        *columns;
  const gchar *caption;
  gchar *file;
  gint i, j, nop;
  guint noc;

  caption =
           double_underscores(gtk_menu_item_get_label(GTK_MENU_ITEM(menuitem)));
  notebook = gtk_ltr_families_get_notebook(GTK_LTR_FAMILIES(user_data));
  nop = gtk_notebook_get_n_pages(notebook);
  for (i = 0; i < nop; i++) {
    sw = gtk_notebook_get_nth_page(notebook, i);
    children = gtk_container_get_children(GTK_CONTAINER(sw));
    list_view = GTK_TREE_VIEW(g_list_first(children)->data);
    columns = gtk_tree_view_get_columns(list_view);
    noc = g_list_length(columns);
    for (j = 0; j < noc; j++) {
      column = gtk_tree_view_get_column(list_view, j);
      if (g_strcmp0(caption, gtk_tree_view_column_get_title(column)) == 0)
        gtk_tree_view_column_set_visible(column,
                                      gtk_check_menu_item_get_active(menuitem));
    }
    g_list_free(children);
    g_list_free(columns);
  }
  file = gtk_ltr_families_get_projectfile(GTK_LTR_FAMILIES(user_data));
   if (file) {
    GtRDB *rdb = gtk_ltr_families_get_rdb(GTK_LTR_FAMILIES(user_data));
    GtRDBStmt *stmt = NULL;
    GtError *err = gt_error_new();
    gint had_err = 0;
    gchar query[BUFSIZ];

    if (!gtk_check_menu_item_get_active(menuitem)) {
      g_snprintf(query, BUFSIZ,
                 "INSERT INTO invisible_columns (name) values (\"%s\")",
                 caption);
      had_err = gt_rdb_prepare(rdb, query, -1, &stmt, err);
      if (!had_err)
        had_err = gt_rdb_stmt_exec(stmt, err);
    } else {
      g_snprintf(query, BUFSIZ,
                 "DELETE FROM invisible_columns WHERE name = \"%s\"", caption);
      had_err = gt_rdb_prepare(rdb, query, -1, &stmt, err);
      if (!had_err)
        had_err = gt_rdb_stmt_exec(stmt, err);
    }
    if (had_err == -1)
      error_handle(gtk_widget_get_toplevel(GTK_WIDGET(user_data)), err);
    gt_rdb_stmt_delete(stmt);
    gt_error_delete(err);
  }
}

static void view_columns_set_submenu(GUIData *ltrgui, GtHashmap *features,
                                     GtRDB *rdb, GtError *err, gboolean sqlt)
{
  GtkWidget *menu,
            *menuitem;
  GtStrArray *captions;
  GtRDBStmt *stmt;
  gchar buffer[BUFSIZ];
  gint had_err = 0;
  unsigned long i;

  captions = gt_str_array_new();
  menu = gtk_menu_new();

  gt_hashmap_foreach(features, get_keys, (void*) captions, err);

  for (i = 0; i < gt_str_array_size(captions); i++) {
    menuitem = gtk_check_menu_item_new_with_label(gt_str_array_get(captions,
                                                                   i));
    if (sqlt && rdb) {
      g_signal_connect(G_OBJECT(menuitem), "toggled",
                       G_CALLBACK(view_columns_toggled),
                       ltrgui->ltrfams);
      g_snprintf(buffer, BUFSIZ,
                 "SELECT name FROM invisible_columns WHERE name = \"%s\"",
                 double_underscores(gt_str_array_get(captions, i)));
      had_err = gt_rdb_prepare(rdb, buffer, -1, &stmt, err);
      if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), TRUE);
        error_handle(ltrgui->main_window, err);
      } else if (had_err == 0) {
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), TRUE);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), FALSE);
        gt_rdb_stmt_delete(stmt);
      } else {
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), TRUE);
      }
    } else {
      gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), TRUE);
      g_signal_connect(G_OBJECT(menuitem), "toggled",
                       G_CALLBACK(view_columns_toggled),
                       ltrgui->ltrfams);
    }
    if (g_strcmp0(gt_str_array_get(captions, i), FNT_LLTR) == 0)
      gtk_menu_shell_insert(GTK_MENU_SHELL(menu), menuitem, 0);
    else if (g_strcmp0(gt_str_array_get(captions, i), FNT_RLTR) == 0)
      gtk_menu_shell_insert(GTK_MENU_SHELL(menu), menuitem, 1);
    else
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
  }
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(ltrgui->menubar_view_columns), menu);
  gtk_widget_show_all(menu);
  gtk_widget_set_sensitive(ltrgui->menubar_view_columns, TRUE);
  gt_str_array_delete(captions);
}

static int
insert_param_set(GT_UNUSED void *key, void *val, void *r, GtError *err)
{
  GtRDB *rdb = (GtRDB*) r;
  GtRDBStmt *stmt;
  gchar *query = (gchar*) val;
  gint had_err = 0;

  had_err = gt_rdb_prepare(rdb, query, -1, &stmt, err);
  if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    gt_rdb_stmt_delete(stmt);
    return 1;
  }
  gt_rdb_stmt_delete(stmt);
  return 0;
}

static gint extract_match_param_sets(GUIData *ltrgui)
{
  GtRDB *rdb;
  GtRDBStmt *stmt;
  GtError *err;
  gint had_err = 0;

  err = gt_error_new();
  rdb = gtk_ltr_families_get_rdb(GTK_LTR_FAMILIES(ltrgui->ltrfams));
  if (!rdb) {
    gt_error_set(ltrgui->err,
                "Could not retrieve parameter sets: %s",
                gt_error_get(err));
    gt_error_delete(err);
    return -1;
  }
  had_err = gt_rdb_prepare(rdb, "SELECT * FROM refseq_match_params", -1,
                           &stmt, err);
  if (had_err) {
    gt_error_set(ltrgui->err, "Could not retrieve parameter sets: %s",
                 gt_error_get(err));
    gt_error_delete(err);
    return -1;
  }
  ltrgui->refseq_paramsets = gt_hashmap_new(GT_HASH_STRING, free_gt_hash_elem,
                                            free_gt_hash_elem);
  while ((had_err = gt_rdb_stmt_exec(stmt, err)) == 0) {
    GtStr *result = gt_str_new();
    gint dust;
    gchar *gapopen, *gapextend, *penalty, *reward, *threads, *wordsize, *xdrop,
          *evalue, *seqid, *moreblast, *set_id, query[BUFSIZ];
    gdouble mlen;

    gt_rdb_stmt_get_string(stmt, 1, result, err);
    set_id = gt_cstr_dup(gt_str_get(result));
    gt_str_delete(result);
    result = gt_str_new();
    gt_rdb_stmt_get_string(stmt, 2, result, err);
    evalue = gt_cstr_dup(gt_str_get(result));
    gt_str_delete(result);
    result = gt_str_new();
    gt_rdb_stmt_get_int(stmt, 3, &dust, err);
    gapopen = gt_cstr_dup(gt_str_get(result));
    gt_str_delete(result);
    result = gt_str_new();
    gt_rdb_stmt_get_string(stmt, 5, result, err);
    gapextend = gt_cstr_dup(gt_str_get(result));
    gt_str_delete(result);
    result = gt_str_new();
    gt_rdb_stmt_get_string(stmt, 6, result, err);
    xdrop = gt_cstr_dup(gt_str_get(result));
    gt_str_delete(result);
    result = gt_str_new();
    gt_rdb_stmt_get_string(stmt, 7, result, err);
    penalty = gt_cstr_dup(gt_str_get(result));
    gt_str_delete(result);
    result = gt_str_new();
    gt_rdb_stmt_get_string(stmt, 8, result, err);
    reward = gt_cstr_dup(gt_str_get(result));
    gt_str_delete(result);
    result = gt_str_new();
    gt_rdb_stmt_get_string(stmt, 9, result, err);
    threads = gt_cstr_dup(gt_str_get(result));
    gt_str_delete(result);
    result = gt_str_new();
    gt_rdb_stmt_get_string(stmt, 10, result, err);
    wordsize = gt_cstr_dup(gt_str_get(result));
    gt_str_delete(result);
    result = gt_str_new();
    gt_rdb_stmt_get_string(stmt, 11, result, err);
    seqid = gt_cstr_dup(gt_str_get(result));
    gt_str_delete(result);
    result = gt_str_new();
    gt_rdb_stmt_get_string(stmt, 12, result, err);
    moreblast = gt_cstr_dup(gt_str_get(result));
    gt_str_delete(result);
    gt_rdb_stmt_get_double(stmt, 13, &mlen, err);

    g_snprintf(query, BUFSIZ,
               "INSERT INTO refseq_match_params ("
                "set_id, evalue, dust, gapopen, "
                "gapextend, xdrop, penalty, reward, threads, wordsize, "
                "identity, moreblast, "
                "minlen) values (\"%s\", \"%s\", "
                "\"%d\", \"%s\", \"%s\", "
                "\"%s\", \"%s\", \"%s\", "
                "\"%s\", \"%s\", \"%s\", \"%s\", "
                "\"%.2f\")", set_id,
               evalue, dust, gapopen, gapextend, xdrop,
               penalty, reward, threads, wordsize, seqid,
               moreblast, mlen);
    gt_hashmap_add(ltrgui->refseq_paramsets, (void*) gt_cstr_dup(set_id),
                   (void*) gt_cstr_dup(query));
    g_free(gapopen);
    g_free(gapextend);
    g_free(penalty);
    g_free(reward);
    g_free(threads);
    g_free(wordsize);
    g_free(xdrop);
    g_free(evalue);
    g_free(seqid);
    g_free(moreblast);
    g_free(set_id);
  }
  if (had_err < 0) {
    gt_hashmap_delete(ltrgui->refseq_paramsets);
    ltrgui->refseq_paramsets = NULL;
  }
  gt_rdb_stmt_delete(stmt);
  return 0;
}

static gint save_match_param_sets(GUIData *ltrgui)
{
  GtRDB *rdb;
  GtRDBStmt *stmt;
  GtError *err;
  gint had_err = 0;

  err = gt_error_new();

  rdb = gtk_ltr_families_get_rdb(GTK_LTR_FAMILIES(ltrgui->ltrfams));
  if (!rdb) {
    gt_error_set(ltrgui->err,
                "Could not retrieve parameter sets: %s",
                gt_error_get(err));
    gt_error_delete(err);
    return -1;
  }

  had_err = gt_rdb_prepare(rdb,
                           "CREATE TABLE IF NOT EXISTS refseq_match_params "
                           "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "set_id INTEGER, "
                            "evalue TEXT, "
                            "dust INTEGER, "
                            "gapopen TEXT, "
                            "gapextend TEXT, "
                            "xdrop TEXT, "
                            "penalty TEXT, "
                            "reward TEXT, "
                            "threads TEXT, "
                            "wordsize TEXT, "
                            "identity TEXT, "
                            "moreblast TEXT, "
                            "minlen REAL)",
                           -1, &stmt, err);

  if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    gt_error_set(ltrgui->err, "Could not save parameter sets: %s",
                 gt_error_get(err));
    gt_error_delete(err);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);

  if (ltrgui->refseq_paramsets) {
    had_err = gt_hashmap_foreach(ltrgui->refseq_paramsets,
                                 insert_param_set, (void*) rdb, ltrgui->err);
  }

  gt_hashmap_delete(ltrgui->refseq_paramsets);
  ltrgui->refseq_paramsets = NULL;
  gt_error_delete(err);
  if (had_err)
      return -1;
  else
    return 0;
}

static gboolean save_project_data_finished(gpointer data)
{
  ThreadData *threaddata = (ThreadData*) data;
  GtkWidget *ltrfams = threaddata->ltrgui->ltrfams;
  g_source_remove(GPOINTER_TO_INT(
                                 g_object_get_data(G_OBJECT(threaddata->window),
                                                   "source_id")));
  gtk_widget_destroy(threaddata->window);
  reset_progressbar(threaddata->progressbar);
  if (!threaddata->had_err) {
    gtk_ltr_families_set_modified(GTK_LTR_FAMILIES(ltrfams), FALSE);
    threaddata->had_err = save_gui_settings(threaddata->ltrgui);
    if (!threaddata->had_err)
      threaddata->had_err =
                  gtk_project_settings_save_data(GTK_PROJECT_SETTINGS(
                                                  threaddata->ltrgui->projset),
                                                  threaddata->ltrgui->err);

    if (!threaddata->had_err)
      threaddata->had_err = gtk_ltr_filter_save_data(
                                    GTK_LTR_FILTER(threaddata->ltrgui->ltrfilt),
                                                   threaddata->ltrgui->err);
    if (!threaddata->had_err)
      threaddata->had_err = save_match_param_sets(threaddata->ltrgui);
  } else
    g_rename(threaddata->tmp_filename, threaddata->filename);
  if (threaddata->had_err) {
    gdk_threads_enter();
    error_handle(threaddata->ltrgui->main_window, threaddata->ltrgui->err);
    gdk_threads_leave();
  }
  threaddata_delete(threaddata);
  return FALSE;
}

gboolean save_as_project_data_finished(gpointer data)
{
  ThreadData *threaddata = (ThreadData*) data;
  GtkWidget *ltrfams = threaddata->ltrgui->ltrfams,
            *projset = threaddata->ltrgui->projset;

  g_source_remove(GPOINTER_TO_INT(
                               g_object_get_data(G_OBJECT(threaddata->window),
                                                 "source_id")));
  gtk_widget_destroy(threaddata->window);
  reset_progressbar(threaddata->progressbar);
  if (threaddata->had_err) {
    gt_error_set(threaddata->ltrgui->err,
                "Error while saving data: %s",
                gt_error_get(threaddata->err));
  }
  if (!threaddata->had_err) {
    threaddata->had_err = save_gui_settings(threaddata->ltrgui);
    gtk_project_settings_update_projectfile(GTK_PROJECT_SETTINGS(projset),
                                            threaddata->filename);
    if (!threaddata->had_err)
      threaddata->had_err =
                  gtk_project_settings_save_data(GTK_PROJECT_SETTINGS(projset),
                                                 threaddata->ltrgui->err);

    if (!threaddata->had_err)
      threaddata->had_err = save_match_param_sets(threaddata->ltrgui);
  }
  if (!threaddata->had_err) {
    gtk_ltr_families_set_projectfile(GTK_LTR_FAMILIES(ltrfams),
                                     threaddata->filename);
    threaddata->had_err = gtk_ltr_filter_save_data(
                                    GTK_LTR_FILTER(threaddata->ltrgui->ltrfilt),
                                                   threaddata->ltrgui->err);
    if (threaddata->had_err) {
      gdk_threads_enter();
      error_handle(threaddata->ltrgui->main_window, threaddata->ltrgui->err);
      gdk_threads_leave();
    }
    gtk_ltr_families_set_modified(GTK_LTR_FAMILIES(ltrfams), FALSE);
    create_recently_used_resource(threaddata->filename);
  } else {
    gdk_threads_enter();
    error_handle(threaddata->ltrgui->main_window, threaddata->ltrgui->err);
    gdk_threads_leave();
  }
  gdk_threads_enter();
  threaddata_delete(threaddata);
  gdk_threads_leave();

  return FALSE;
}

static gboolean open_project_data_finished(gpointer data)
{
  ThreadData *threaddata = (ThreadData*) data;
  GtkWidget *ltrfams = threaddata->ltrgui->ltrfams;

  g_source_remove(GPOINTER_TO_INT(
                               g_object_get_data(G_OBJECT(threaddata->window),
                                                 "source_id")));
  gtk_widget_destroy(threaddata->window);
  reset_progressbar(threaddata->progressbar);
  if (!threaddata->had_err) {
    gtk_widget_destroy(ltrfams);
    gtk_widget_destroy(threaddata->ltrgui->ltrfilt);
    gtk_widget_destroy(threaddata->ltrgui->projset);
    threaddata->ltrgui->projset = gtk_project_settings_new(threaddata->rdb);
    threaddata->ltrgui->ltrfams =
                           gtk_ltr_families_new(threaddata->ltrgui->statusbar,
                                                threaddata->progressbar,
                                                threaddata->ltrgui->projset,
                                                threaddata->ltrgui->style_file,
                                                threaddata->ltrgui->err);
    ltrfams = threaddata->ltrgui->ltrfams;
    gtk_ltr_families_set_rdb(threaddata->rdb, GTK_LTR_FAMILIES(ltrfams));
    gtk_ltr_families_set_adb(threaddata->adb, GTK_LTR_FAMILIES(ltrfams));
    gtk_ltr_families_set_fi(threaddata->fi, GTK_LTR_FAMILIES(ltrfams));
    threaddata->ltrgui->ltrfilt = gtk_ltr_filter_new(ltrfams);
    gtk_ltr_families_set_filter_widget(GTK_LTR_FAMILIES(ltrfams),
                                       threaddata->ltrgui->ltrfilt);
    gtk_box_pack_start(GTK_BOX(threaddata->ltrgui->vbox), ltrfams, TRUE, TRUE,
                       0);
    gtk_ltr_families_fill_with_data(GTK_LTR_FAMILIES(ltrfams),
                                    threaddata->nodes,
                                    threaddata->features,
                                    threaddata->n_features);
    gtk_ltr_families_set_projectfile(GTK_LTR_FAMILIES(ltrfams),
                                     threaddata->filename);
    threaddata->had_err = gtk_project_settings_set_data_from_sqlite(
                                                           GTK_PROJECT_SETTINGS(
                                                   threaddata->ltrgui->projset),
                                                   threaddata->filename,
                                                   threaddata->ltrgui->err);
    if (!threaddata->had_err)
      threaddata->had_err = gtk_ltr_filter_get_filter_files_from_sql(
                                    GTK_LTR_FILTER(threaddata->ltrgui->ltrfilt),
                                    threaddata->ltrgui->err);
    view_columns_set_submenu(threaddata->ltrgui, threaddata->features,
                             threaddata->rdb, threaddata->err, TRUE);
    activate_menuitems(threaddata->ltrgui);
    gtk_ltr_families_set_modified(GTK_LTR_FAMILIES(ltrfams), FALSE);
    create_recently_used_resource(threaddata->filename);
  }
  if (!threaddata->had_err) {
    threaddata->had_err = apply_gui_settings(threaddata->ltrgui);
  }

  if (threaddata->had_err) {
    gdk_threads_enter();
    error_handle(threaddata->ltrgui->main_window, threaddata->ltrgui->err);
    gdk_threads_leave();
  }

  threaddata_delete(threaddata);

  return FALSE;
}

static gpointer save_project_data_start(gpointer data)
{
  ThreadData *threaddata = (ThreadData*) data;
  GtkWidget *ltrfams = threaddata->ltrgui->ltrfams;
  GtFeatureIndex *fi = NULL;

  gtk_progress_bar_set_text(GTK_PROGRESS_BAR(threaddata->progressbar),
                            "Saving data");
  (void) extract_match_param_sets(threaddata->ltrgui);
  fi = gtk_ltr_families_get_fi(GTK_LTR_FAMILIES(ltrfams));
  if (!fi)
    threaddata->had_err = -1;
  if (!threaddata->had_err)
    gt_feature_index_save(fi, NULL);
  g_idle_add(save_project_data_finished, data);
  return NULL;
}

static gpointer open_project_data_start(gpointer data)
{
  ThreadData *threaddata = (ThreadData*) data;
  GtNodeStream *preprocess_stream = NULL,
               *array_in_stream = NULL;
  GtHashmap *features;
  GtArray *nodes,
          *tmp_nodes;
  GtStrArray *seqids = NULL;
  unsigned long i, n_features;

  gtk_progress_bar_set_text(GTK_PROGRESS_BAR(threaddata->progressbar),
                            "Reading data from file");
  threaddata->rdb = gt_rdb_sqlite_new(threaddata->filename,
                                      threaddata->ltrgui->err);
  if (!threaddata->rdb)
    threaddata->had_err = -1;
  if (!threaddata->had_err) {
    threaddata->adb = gt_anno_db_gfflike_new();
    if (!threaddata->adb)
      threaddata->had_err = -1;
  }
  if (!threaddata->had_err) {
    threaddata->fi = gt_anno_db_schema_get_feature_index(threaddata->adb,
                                                         threaddata->rdb,
                                                       threaddata->ltrgui->err);
    threaddata->had_err = threaddata->fi ? 0 : -1;
  }
  if (!threaddata->had_err) {
    seqids = gt_feature_index_get_seqids(threaddata->fi,
                                         threaddata->ltrgui->err);
    if (seqids) {
      nodes = gt_array_new(sizeof(GtFeatureNode*));
      for (i = 0; i < gt_str_array_size(seqids); i++) {
        tmp_nodes =
            gt_feature_index_get_features_for_seqid(threaddata->fi,
                                                    gt_str_array_get(seqids, i),
                                                    threaddata->ltrgui->err);
        gt_array_add_array(nodes, tmp_nodes);
        gt_array_delete(tmp_nodes);
        tmp_nodes = NULL;
      }
      gt_str_array_delete(seqids);
    } else threaddata->had_err = -1;
  }
  if (!threaddata->had_err) {
    GtGenomeNode *gn;
    gdk_threads_enter();
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(threaddata->progressbar),
                              "Preprocessing candidates");
    features = gt_hashmap_new(GT_HASH_STRING, free_gt_hash_elem, NULL);
    n_features = LTRFAMS_LV_N_COLUMS;

    array_in_stream = gt_array_in_stream_new(nodes, NULL,
                                             threaddata->ltrgui->err);
    preprocess_stream = ltrgui_preprocess_stream_new(array_in_stream, features,
                                                     &n_features, FALSE,
                                                     threaddata->ltrgui->err);
    while (!(threaddata->had_err = gt_node_stream_next(preprocess_stream, &gn,
                                             threaddata->ltrgui->err)) && gn);
    gdk_threads_leave();
  }
  if (!threaddata->had_err) {
    threaddata->nodes = nodes;
    threaddata->features = features;
    threaddata->n_features = n_features;
  }

  gt_node_stream_delete(array_in_stream);
  gt_node_stream_delete(preprocess_stream);
  g_idle_add(open_project_data_finished, data);
  return NULL;
}

static gpointer save_and_reload_data_start(gpointer data)
{
  ThreadData *threaddata = (ThreadData*) data;
  GT_UNUSED GtkWidget *ltrfams = threaddata->ltrgui->ltrfams;
  GtNodeStream *array_in_stream = NULL,
               *feature_stream = NULL;
  GtRDB *rdb = NULL;
  GtAnnoDBSchema *adb = NULL;
  GtFeatureIndex *fi = NULL;
  GT_UNUSED unsigned long i;

  gtk_progress_bar_set_text(GTK_PROGRESS_BAR(threaddata->progressbar),
                            "Saving data");
  rdb = gt_rdb_sqlite_new(threaddata->filename, threaddata->ltrgui->err);
  if (!rdb)
    threaddata->had_err = -1;
  if (!threaddata->had_err) {
    adb = gt_anno_db_gfflike_new();
    if (!adb)
      threaddata->had_err = -1;
  }
  if (!threaddata->had_err) {
    fi = gt_anno_db_schema_get_feature_index(adb, rdb, threaddata->ltrgui->err);
    if (!fi)
      threaddata->had_err = -1;
  }
  if (!threaddata->had_err) {
    array_in_stream = gt_array_in_stream_new(threaddata->nodes,
                                             &threaddata->progress,
                                             threaddata->ltrgui->err);
    gt_assert(array_in_stream);

    feature_stream = gt_feature_stream_new(array_in_stream, fi);
    gdk_threads_enter();
    threaddata->had_err = gt_node_stream_pull(feature_stream,
                                              threaddata->ltrgui->err);
    gdk_threads_leave();
  }
  gt_node_stream_delete(feature_stream);
  gt_node_stream_delete(array_in_stream);
  gt_feature_index_delete(fi);
  gt_anno_db_schema_delete(adb);
  gt_array_delete(threaddata->nodes);
  rdb = NULL;
  fi = NULL;
  adb = NULL;

  if (!threaddata->had_err) {
    GtNodeStream *preprocess_stream = NULL,
                 *array_in_stream = NULL;
    GtHashmap *features = NULL;
    GtArray *nodes = NULL;
    unsigned long n_features;

    threaddata->had_err =
                  gtk_project_settings_save_data(GTK_PROJECT_SETTINGS(
                                                   threaddata->ltrgui->projset),
                                                 threaddata->ltrgui->err);
    if (!threaddata->had_err)
      threaddata->had_err = save_gui_settings(threaddata->ltrgui);
    if (!threaddata->had_err) {
      gtk_progress_bar_set_text(GTK_PROGRESS_BAR(threaddata->progressbar),
                                "Reloading data from database");
      threaddata->rdb = gt_rdb_sqlite_new(threaddata->filename,
                                          threaddata->ltrgui->err);
    }
    if (!threaddata->rdb)
      threaddata->had_err = -1;
    if (!threaddata->had_err) {
      threaddata->adb = gt_anno_db_gfflike_new();
      if (!threaddata->adb)
        threaddata->had_err = -1;
    }
    if (!threaddata->had_err) {
      threaddata->fi = gt_anno_db_schema_get_feature_index(threaddata->adb,
                                                           threaddata->rdb,
                                                       threaddata->ltrgui->err);
      threaddata->had_err = threaddata->fi ? 0 : -1;
    }
    if (!threaddata->had_err) {
      GtStrArray *seqids;
      nodes = gt_array_new(sizeof(GtFeatureNode*));
      seqids = gt_feature_index_get_seqids(threaddata->fi,
                                           threaddata->ltrgui->err);
      if (seqids) {
        GtArray *tmp_nodes;
        for (i = 0; i < gt_str_array_size(seqids); i++) {
          tmp_nodes = gt_feature_index_get_features_for_seqid(threaddata->fi,
                                                        gt_str_array_get(seqids,
                                                                         i),
                                                       threaddata->ltrgui->err);
          gt_array_add_array(nodes, tmp_nodes);
          gt_array_delete(tmp_nodes);
          tmp_nodes = NULL;
        }
        gt_str_array_delete(seqids);
      }
    }
    if (!threaddata->had_err) {
      GtGenomeNode *gn;
      gdk_threads_enter();
      gtk_progress_bar_set_text(GTK_PROGRESS_BAR(threaddata->progressbar),
                                "Preprocessing candidates");
      features = gt_hashmap_new(GT_HASH_STRING, free_gt_hash_elem, NULL);
      n_features = LTRFAMS_LV_N_COLUMS;

      array_in_stream = gt_array_in_stream_new(nodes, NULL,
                                               threaddata->ltrgui->err);
      preprocess_stream = ltrgui_preprocess_stream_new(array_in_stream,
                                                       features, &n_features,
                                                       FALSE,
                                                       threaddata->ltrgui->err);
      while (!(threaddata->had_err = gt_node_stream_next(preprocess_stream, &gn,
                                             threaddata->ltrgui->err)) && gn);
      gdk_threads_leave();
    }
    if (!threaddata->had_err) {
      threaddata->nodes = nodes;
      threaddata->features = features;
      threaddata->n_features = n_features;
    }

    gt_node_stream_delete(array_in_stream);
    gt_node_stream_delete(preprocess_stream);
  } else
    if (g_file_test(threaddata->tmp_filename, G_FILE_TEST_EXISTS))
      g_rename(threaddata->tmp_filename, threaddata->filename);
  gt_free(threaddata->tmp_filename);

  g_idle_add(open_project_data_finished, data);
  return NULL;
}

void first_save_and_reload(GUIData *ltrgui, GtArray *nodes,
                           const gchar *projectfile)
{
  ThreadData *threaddata;

  threaddata = threaddata_new();
  threaddata->filename = g_strdup(projectfile);
  if (g_file_test(threaddata->filename, G_FILE_TEST_EXISTS)) {
    threaddata->tmp_filename = g_strdup_printf("%s.bak", threaddata->filename);
    g_rename(threaddata->filename, threaddata->tmp_filename);
  }
  threaddata->ltrgui = ltrgui;
  threaddata->progressbar = ltrgui->progressbar;
  threaddata->progress = 0;
  threaddata->had_err = 0;
  threaddata->nodes = nodes;
  progress_dialog_init(threaddata, ltrgui->main_window);

  if (!g_thread_create(save_and_reload_data_start, (gpointer) threaddata,
                       FALSE, NULL)) {
    gt_error_set(ltrgui->err,
                "Could not create new thread.");
    error_handle(ltrgui->main_window, ltrgui->err);
  }
}

/* static void save_as_activate(GT_UNUSED GtkMenuItem *menuitem, GUIData *ltrgui)
{
  ThreadData *threaddata;
  GtkWidget *filechooser,
            *dialog;
  gchar *filename;
  const gchar *projectfile;
  gboolean bakfile = FALSE;

  filechooser = gtk_file_chooser_dialog_new(GUI_DIALOG_SAVE_AS,
                                            GTK_WINDOW(ltrgui->main_window),
                                            GTK_FILE_CHOOSER_ACTION_SAVE,
                                            GTK_STOCK_CANCEL,
                                            GTK_RESPONSE_CANCEL,
                                            GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                            NULL);
  projectfile =
       gtk_ltr_families_get_projectfile(GTK_LTR_FAMILIES(ltrgui->ltrfams));
  if (projectfile != NULL) {
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(filechooser), projectfile);
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(filechooser),
                                      g_path_get_basename(projectfile));
  } else {
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(filechooser),
                                        g_get_home_dir());
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(filechooser),
                                      UNTITLED_PROJECT);
  }

  if (gtk_dialog_run(GTK_DIALOG(filechooser)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser));
    if (!g_str_has_suffix(filename, SQLITE_PATTERN)) {
      gchar *tmp = g_strdup(filename);
      g_free(filename);
      filename = g_strconcat(tmp, SQLITE_PATTERN, NULL);
      g_free(tmp);
    }
  } else {
    gtk_widget_destroy(filechooser);
    return;
  }
  gtk_widget_destroy(filechooser);

  threaddata = threaddata_new();
  threaddata->tmp_filename = NULL;
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
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_NO) {
      gtk_widget_destroy(dialog);
      g_slice_free(ThreadData, threaddata);
      return;
    } else {
      threaddata->tmp_filename = g_strdup_printf("%s.bak", filename);
      g_rename(filename, threaddata->tmp_filename);
      bakfile = TRUE;
    }
    gtk_widget_destroy(dialog);
  }
  threaddata->filename = filename;
  threaddata->ltrgui = ltrgui;
  threaddata->progressbar = ltrgui->progressbar;
  threaddata->err = gt_error_new();
  threaddata->save_as = TRUE;
  threaddata->bakfile = bakfile;
  threaddata->had_err = 0;
  progress_dialog_init(threaddata, ltrgui->main_window);

  if (!g_thread_create(save_project_data_start, (gpointer) threaddata,
                       FALSE, NULL)) {
    gt_error_set(ltrgui->err,
                "Could not create new thread.");
    error_handle(ltrgui->main_window, ltrgui->err);
  }
} */

void menubar_save_activate(GT_UNUSED GtkMenuItem *menuitem, GUIData *ltrgui)
{
  ThreadData *threaddata;
  gchar *projectfile;

  projectfile =
       gtk_ltr_families_get_projectfile(GTK_LTR_FAMILIES(ltrgui->ltrfams));
  gt_assert(projectfile);

  threaddata = threaddata_new();
  threaddata->filename = projectfile;
  threaddata->ltrgui = ltrgui;
  threaddata->progressbar = ltrgui->progressbar;
  threaddata->save = TRUE;
  threaddata->err = gt_error_new();
  threaddata->progress = 0;
  threaddata->had_err = 0;
  progress_dialog_init(threaddata, ltrgui->main_window);

  if (!g_thread_create(save_project_data_start, (gpointer) threaddata,
                       FALSE, NULL)) {
    gt_error_set(ltrgui->err,
                "Could not create new thread.");
    error_handle(ltrgui->main_window, ltrgui->err);
  }
}

static void export_gff3_activate(GT_UNUSED GtkMenuItem *menuitem,
                                 GUIData *ltrgui)
{
  GtkWidget *dialog;
  GtArray *nodes;
  gchar *filename;
  const gchar *projectfile;

  nodes = gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrgui->ltrfams));

  dialog = gtk_file_chooser_dialog_new(CHOOSE_FILEN_ANNO,
                                       GTK_WINDOW(ltrgui->main_window),
                                       GTK_FILE_CHOOSER_ACTION_SAVE,
                                       GTK_STOCK_CANCEL,
                                       GTK_RESPONSE_CANCEL, "E_xport",
                                       GTK_RESPONSE_ACCEPT, NULL);

  projectfile =
            gtk_ltr_families_get_projectfile(GTK_LTR_FAMILIES(ltrgui->ltrfams));
  if (projectfile != NULL)
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),
                                        g_path_get_dirname(projectfile));
  else
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),
                                        g_get_home_dir());

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    gtk_widget_destroy(dialog);
    export_annotation(nodes, filename, FALSE, ltrgui->main_window);
    g_free(filename);
  } else
    gtk_widget_destroy(dialog);
}

static void open_activate(GT_UNUSED GtkMenuItem *menuitem, GUIData *ltrgui)
{
  ThreadData *threaddata;
  GtkWidget *filechooser,
            *dialog;
  gchar *filename = NULL;
  gint response = GTK_RESPONSE_REJECT;

  if (gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrgui->ltrfams))) {
    if (!gtk_ltr_families_get_projectfile(
                                        GTK_LTR_FAMILIES(ltrgui->ltrfams))) {
      dialog = unsaved_changes_dialog(ltrgui, NO_PROJECT_DIALOG);
      response = gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
    } else if (gtk_ltr_families_get_modified(
                                           GTK_LTR_FAMILIES(ltrgui->ltrfams))) {
      dialog = unsaved_changes_dialog(ltrgui, UNSAVED_CHANGES_DIALOG);
      response = gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
    }
  }

  switch (response) {
    case GTK_RESPONSE_CANCEL:
      return;
      break;
    case GTK_RESPONSE_ACCEPT:
      menubar_save_activate(NULL, ltrgui);
      return;
      break;
    case GTK_RESPONSE_REJECT:
      break;
    default:
      return;
      break;
  }

  filechooser = gtk_file_chooser_dialog_new(GUI_DIALOG_OPEN,
                                            GTK_WINDOW(ltrgui->main_window),
                                            GTK_FILE_CHOOSER_ACTION_OPEN,
                                            GTK_STOCK_CANCEL,
                                            GTK_RESPONSE_CANCEL,
                                            GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                            NULL);

  if (gtk_dialog_run(GTK_DIALOG(filechooser)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser));
    gtk_widget_destroy(filechooser);
  } else {
    gtk_widget_destroy(filechooser);
    return;
  }

  threaddata = threaddata_new();
  threaddata->ltrgui = ltrgui;
  threaddata->progressbar = ltrgui->progressbar;
  threaddata->filename = filename;
  threaddata->err = gt_error_new();
  threaddata->open = TRUE;
  progress_dialog_init(threaddata, ltrgui->main_window);
  if (!g_thread_create(open_project_data_start, (gpointer) threaddata,
                       FALSE, NULL)) {
    gt_error_set(ltrgui->err,
                "Could not create new thread.");
    error_handle(ltrgui->main_window, ltrgui->err);
  }
}

static void new_activate(GT_UNUSED GtkMenuItem *menuitem, GUIData *ltrgui)
{
  GtkWidget *dialog;
  gint response = 0;

  if (gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrgui->ltrfams))) {
    if (!gtk_ltr_families_get_projectfile(GTK_LTR_FAMILIES(ltrgui->ltrfams))) {
      dialog = unsaved_changes_dialog(ltrgui, NO_PROJECT_DIALOG);
      response = gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
    } else if (gtk_ltr_families_get_modified(
                                           GTK_LTR_FAMILIES(ltrgui->ltrfams))) {
      dialog = unsaved_changes_dialog(ltrgui, UNSAVED_CHANGES_DIALOG);
      response = gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
    }
  }

  switch (response) {
    case GTK_RESPONSE_CANCEL:
      return;
      break;
    case GTK_RESPONSE_ACCEPT:
      menubar_save_activate(NULL, ltrgui);
      break;
    case GTK_RESPONSE_REJECT:
      break;
    default:
      break;
  }

  if (ltrgui->assistant)
    gtk_widget_destroy(ltrgui->assistant);
  ltrgui->assistant = gtk_ltr_assistant_new();
  g_signal_connect(G_OBJECT(ltrgui->assistant), "apply",
                   G_CALLBACK(project_wizard_apply), (gpointer) ltrgui);
  gtk_window_set_transient_for(GTK_WINDOW(ltrgui->assistant),
                               GTK_WINDOW(ltrgui->main_window));

  gtk_widget_show(ltrgui->assistant);
}

static void close_activate(GT_UNUSED GtkMenuItem *menuitem, GUIData *ltrgui)
{
  GtkWidget *dialog;
  gint response = GTK_RESPONSE_REJECT;

  if (!gtk_ltr_families_get_projectfile(
                                      GTK_LTR_FAMILIES(ltrgui->ltrfams))) {
    dialog = unsaved_changes_dialog(ltrgui, NO_PROJECT_DIALOG);
    response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  } else if (gtk_ltr_families_get_modified(GTK_LTR_FAMILIES(ltrgui->ltrfams))) {
    dialog = unsaved_changes_dialog(ltrgui, UNSAVED_CHANGES_DIALOG);
    response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  }

  switch (response) {
    case GTK_RESPONSE_CANCEL:
      return;
      break;
    case GTK_RESPONSE_ACCEPT:
      menubar_save_activate(NULL, ltrgui);
      break;
    case GTK_RESPONSE_REJECT:
      gtk_widget_set_sensitive(ltrgui->menubar_view_columns, FALSE);
      gtk_widget_set_sensitive(ltrgui->menubar_save, FALSE);
      gtk_widget_set_sensitive(ltrgui->menubar_save_as, FALSE);
      gtk_widget_set_sensitive(ltrgui->menubar_export, FALSE);
      gtk_widget_set_sensitive(ltrgui->menubar_close, FALSE);
      gtk_widget_set_sensitive(ltrgui->menubar_project, FALSE);
      gtk_window_set_title(GTK_WINDOW(ltrgui->main_window), GUI_NAME);
      gtk_widget_destroy(ltrgui->ltrfams);
      gtk_widget_destroy(ltrgui->projset);
      ltrgui->projset = gtk_project_settings_new(NULL);
      ltrgui->ltrfams = gtk_ltr_families_new(ltrgui->statusbar,
                                             ltrgui->progressbar,
                                             ltrgui->projset,
                                             ltrgui->style_file,
                                             ltrgui->err);
      gtk_widget_destroy(ltrgui->ltrfilt);
      ltrgui->ltrfilt = gtk_ltr_filter_new(ltrgui->ltrfams);
      gtk_ltr_families_set_filter_widget(GTK_LTR_FAMILIES(ltrgui->ltrfams),
                                         ltrgui->ltrfilt);
      gtk_box_pack_start(GTK_BOX(ltrgui->vbox), ltrgui->ltrfams, TRUE, TRUE, 0);
      break;
    default:
      break;
  }
}

static void help_about_activate(GT_UNUSED GtkMenuItem *menuitem,
                                GUIData *ltrgui)
{
  GtkWidget *dialog;
  GdkPixbuf *logo;
  GError *err = NULL;

  dialog = gtk_about_dialog_new();
  gtk_window_set_transient_for(GTK_WINDOW(dialog),
                               GTK_WINDOW(ltrgui->main_window));

  logo = gdk_pixbuf_new_from_file(GUI_LOGO, &err);

  if (err == NULL)
    gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), logo);
  else {
    if (err->domain == GDK_PIXBUF_ERROR)
      g_warning("GdkPixbufError: %s\n", err->message);
    else if (err->domain == G_FILE_ERROR)
      g_warning("GFileError: %s\n", err->message);
    else
      g_warning("An error in the domain: %d has occured!\n", err->domain);

    g_error_free(err);
  }

  gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), GUI_NAME);
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), GUI_VERSION);
  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), GUI_COPYRIGHT);
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), GUI_COMMENTS);
  gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(dialog), license);
  gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), GUI_WEBSITE);

  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

static void open_recent_activated(GtkRecentChooser *rc, GUIData *ltrgui)
{
  ThreadData *threaddata;
  GtkWidget *dialog;
  gchar *filename;
  gint response = GTK_RESPONSE_REJECT;

  if (gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrgui->ltrfams))) {
    if (!gtk_ltr_families_get_projectfile(
                                        GTK_LTR_FAMILIES(ltrgui->ltrfams))) {
      dialog = unsaved_changes_dialog(ltrgui, NO_PROJECT_DIALOG);
      response = gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
    } else if (gtk_ltr_families_get_modified(
                                           GTK_LTR_FAMILIES(ltrgui->ltrfams))) {
      dialog = unsaved_changes_dialog(ltrgui, UNSAVED_CHANGES_DIALOG);
      response = gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
    }
  }

  switch (response) {
    case GTK_RESPONSE_CANCEL:
      return;
      break;
    case GTK_RESPONSE_ACCEPT:
      menubar_save_activate(NULL, ltrgui);
      return;
      break;
    case GTK_RESPONSE_REJECT:
      break;
    default:
      return;
      break;
  }
  filename = g_filename_from_uri(gtk_recent_chooser_get_current_uri(rc),
                                 NULL, NULL);

  threaddata = threaddata_new();
  threaddata->ltrgui = ltrgui;
  threaddata->progressbar = ltrgui->progressbar;
  threaddata->err = gt_error_new();
  threaddata->filename = filename;
  threaddata->open = TRUE;
  progress_dialog_init(threaddata, ltrgui->main_window);

  if (!g_thread_create(open_project_data_start, (gpointer) threaddata,
                       FALSE, NULL)) {
    gt_error_set(ltrgui->err,
                "Could not create new thread.");
    error_handle(ltrgui->main_window, ltrgui->err);
  }
}

static void quit_activate(GT_UNUSED GtkMenuItem *menuitem, GUIData *ltrgui)
{
  GtkWidget *dialog;
  gint response = GTK_RESPONSE_REJECT;

  if (gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrgui->ltrfams))) {
    if (!gtk_ltr_families_get_projectfile(
                                        GTK_LTR_FAMILIES(ltrgui->ltrfams))) {
      dialog = unsaved_changes_dialog(ltrgui, NO_PROJECT_DIALOG);
      response = gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
    } else if (gtk_ltr_families_get_modified(
                                           GTK_LTR_FAMILIES(ltrgui->ltrfams))) {
      dialog = unsaved_changes_dialog(ltrgui, UNSAVED_CHANGES_DIALOG);
      response = gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
    }
  }

  switch (response) {
    case GTK_RESPONSE_CANCEL:
      return;
      break;
    case GTK_RESPONSE_ACCEPT:
      menubar_save_activate(NULL, ltrgui);
      return;
      break;
    case GTK_RESPONSE_REJECT:
      break;
    default:
      break;
  }
  gtk_main_quit();
}

static void export_fasta_activate(GT_UNUSED GtkMenuItem *menuitem,
                                  GUIData *ltrgui)
{
  GtkWidget *dialog,
            *projset = ltrgui->projset;
  GtArray *nodes;
  gchar *filename,
        tmp_index[BUFSIZ];
  const gchar *projectfile,
              *indexname;

  nodes = gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrgui->ltrfams));
  indexname = gtk_project_settings_get_indexname(GTK_PROJECT_SETTINGS(projset));
  projectfile =
       gtk_ltr_families_get_projectfile(GTK_LTR_FAMILIES(ltrgui->ltrfams));
  g_snprintf(tmp_index, BUFSIZ, "%s%s", indexname, ESQ_PATTERN);
  if ((g_strcmp0(indexname, "") == 0) ||
      !g_file_test(tmp_index, G_FILE_TEST_EXISTS)) {
    dialog = gtk_message_dialog_new(GTK_WINDOW(ltrgui->main_window),
                                    GTK_DIALOG_MODAL ||
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
                                    "%s",
                                    NO_INDEX_DIALOG);
    gtk_window_set_title(GTK_WINDOW(dialog), "Information!");
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) != GTK_RESPONSE_YES) {
      gtk_widget_destroy(dialog);
      return;
    } else {
      gtk_widget_destroy(dialog);

      dialog = gtk_file_chooser_dialog_new(SELECT_INDEX,
                                           GTK_WINDOW(ltrgui->main_window),
                                           GTK_FILE_CHOOSER_ACTION_SAVE,
                                           GTK_STOCK_CANCEL,
                                           GTK_RESPONSE_CANCEL, "Sele_ct",
                                           GTK_RESPONSE_ACCEPT, NULL);
      if (projectfile != NULL)
        gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),
                                            g_path_get_dirname(projectfile));
      else
        gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),
                                            g_get_home_dir());
      GtkFileFilter *esq_file_filter;
      esq_file_filter = gtk_file_filter_new();
      gtk_file_filter_set_name(esq_file_filter, ESQ_FILTER_PATTERN);
      gtk_file_filter_add_pattern(esq_file_filter, ESQ_FILTER_PATTERN);
      gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), esq_file_filter);
      if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        gchar *tmpname, *tmp;
        tmpname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        tmp = g_strndup(tmpname, strlen(tmpname) - 4);

        gtk_widget_destroy(dialog);
        gtk_project_settings_update_indexname(GTK_PROJECT_SETTINGS(projset),
                                              tmp);
        g_free(tmp);
        g_free(tmpname);
        indexname =
              gtk_project_settings_get_indexname(GTK_PROJECT_SETTINGS(projset));
      } else {
        gtk_widget_destroy(dialog);
        return;
      }
    }
  }

  dialog = gtk_file_chooser_dialog_new(CHOOSE_FILEN_SEQS,
                                       GTK_WINDOW(ltrgui->main_window),
                                       GTK_FILE_CHOOSER_ACTION_SAVE,
                                       GTK_STOCK_CANCEL,
                                       GTK_RESPONSE_CANCEL, "E_xport",
                                       GTK_RESPONSE_ACCEPT, NULL);

  if (projectfile != NULL)
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),
                                        g_path_get_dirname(projectfile));
  else
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),
                                        g_get_home_dir());

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    gtk_widget_destroy(dialog);
    export_sequences(nodes, filename, indexname, FALSE, ltrgui->main_window);
    g_free(filename);
  } else
    gtk_widget_destroy(dialog);
}

static void project_settings_activate(GT_UNUSED GtkMenuItem *menuitem,
                                      GUIData *ltrgui)
{
  gtk_widget_show(ltrgui->projset);
}

static void project_filter_activate(GT_UNUSED GtkMenuItem *menuitem,
                                    GUIData *ltrgui)
{
  GtArray *nodes;
  gchar text[BUFSIZ];

  nodes = gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrgui->ltrfams));
  g_snprintf(text, BUFSIZ, LTR_FILTER_APPLY, gt_array_size(nodes));
  gtk_ltr_filter_set_range(GTK_LTR_FILTER(ltrgui->ltrfilt),
                           LTR_FILTER_RANGE_PROJECT);
  gtk_ltr_filter_set_apply_text(GTK_LTR_FILTER(ltrgui->ltrfilt), text);
  gtk_widget_show(ltrgui->ltrfilt);
}

static void project_match_activate(GT_UNUSED GtkMenuItem *menuitem,
                                   GUIData *ltrgui)
{
  GtArray *nodes;

  nodes =
    gt_array_ref(gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrgui->ltrfams)));
  gtk_ltr_families_refseq_match(nodes, GTK_LTR_FAMILIES(ltrgui->ltrfams));
}

static void project_orf_activate(GT_UNUSED GtkMenuItem *menuitem,
                                   GUIData *ltrgui)
{
  GtArray *nodes;

  nodes =
    gt_array_ref(gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrgui->ltrfams)));
  gtk_ltr_families_orffind(nodes, GTK_LTR_FAMILIES(ltrgui->ltrfams));
}

void menubar_init(GUIData *ltrgui)
{
  GtkWidget *rc, *menu, *menuitem, *submenu;
  GtkAccelGroup *accelgroup;
  GtkRecentFilter *rf;

  ltrgui->menubar = gtk_menu_bar_new();
  accelgroup = gtk_accel_group_new();
  gtk_window_add_accel_group(GTK_WINDOW(ltrgui->main_window), accelgroup);

  /* File menu */
  menuitem = gtk_menu_item_new_with_mnemonic("_File");
  menu = gtk_menu_new();
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(ltrgui->menubar), menuitem);

  menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, accelgroup);
  gtk_menu_item_set_label(GTK_MENU_ITEM(menuitem), "_New...");
  g_object_set_data(G_OBJECT(menuitem),
                    STATUSBAR_MENU_HINT, (gpointer) STATUSBAR_MENU_HINT_NEW);
  g_signal_connect(G_OBJECT(menuitem), "enter-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(menuitem), "leave-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(menuitem), "activate",
                   G_CALLBACK(new_activate), ltrgui);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, accelgroup);
  gtk_menu_item_set_label(GTK_MENU_ITEM(menuitem), "_Open...");
  g_object_set_data(G_OBJECT(menuitem),
                    STATUSBAR_MENU_HINT, (gpointer) STATUSBAR_MENU_HINT_OPEN);
  g_signal_connect(G_OBJECT(menuitem), "enter-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(menuitem), "leave-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(menuitem), "activate",
                   G_CALLBACK(open_activate), ltrgui);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  menuitem = gtk_menu_item_new_with_mnemonic("Open _Recent");
  rc = gtk_recent_chooser_menu_new();
  g_signal_connect(G_OBJECT(rc), "item-activated",
                   G_CALLBACK(open_recent_activated), ltrgui);
  rf = gtk_recent_filter_new();
  gtk_recent_filter_add_group(rf, GUI_RECENT_GROUP);
  gtk_recent_chooser_add_filter(GTK_RECENT_CHOOSER(rc), rf);
  gtk_recent_chooser_set_show_not_found(GTK_RECENT_CHOOSER(rc), FALSE);
  gtk_recent_chooser_set_local_only(GTK_RECENT_CHOOSER(rc), TRUE);
  gtk_recent_chooser_set_limit(GTK_RECENT_CHOOSER(rc), 5);
  gtk_recent_chooser_set_sort_type(GTK_RECENT_CHOOSER(rc), GTK_RECENT_SORT_MRU);
  gtk_recent_chooser_menu_set_show_numbers(GTK_RECENT_CHOOSER_MENU(rc), TRUE);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), rc);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  menuitem = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  ltrgui->menubar_save = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE,
                                                            accelgroup);
  g_object_set_data(G_OBJECT(ltrgui->menubar_save),
                    STATUSBAR_MENU_HINT, (gpointer) STATUSBAR_MENU_HINT_SAVE);
  g_signal_connect(G_OBJECT(ltrgui->menubar_save), "enter-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(ltrgui->menubar_save), "leave-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(ltrgui->menubar_save), "activate",
                   G_CALLBACK(menubar_save_activate), ltrgui);
  gtk_widget_set_sensitive(ltrgui->menubar_save, FALSE);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), ltrgui->menubar_save);

  /* ltrgui->menubar_save_as =
              gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE_AS, accelgroup);
  g_object_set_data(G_OBJECT(ltrgui->menubar_save_as),
                    STATUSBAR_MENU_HINT,
                    (gpointer) STATUSBAR_MENU_HINT_SAVE_AS);
  g_signal_connect(G_OBJECT(ltrgui->menubar_save_as), "enter-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(ltrgui->menubar_save_as), "leave-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(ltrgui->menubar_save_as), "activate",
                   G_CALLBACK(save_as_activate), ltrgui);
  gtk_menu_item_set_label(GTK_MENU_ITEM(ltrgui->menubar_save_as),
                          "Save _As...");
  gtk_widget_set_sensitive(ltrgui->menubar_save_as, FALSE);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), ltrgui->menubar_save_as);

  menuitem = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem); */

  /* menuitem = gtk_menu_item_new_with_mnemonic("_Import...");
  g_object_set_data(G_OBJECT(menuitem),
                    STATUSBAR_MENU_HINT, (gpointer) STATUSBAR_MENU_HINT_IMPORT);
  g_signal_connect(G_OBJECT(menuitem), "enter-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(menuitem), "leave-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(menuitem), "activate",
                   G_CALLBACK(import_activate), ltrgui);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
  menuitem = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem); */

  ltrgui->menubar_export = gtk_menu_item_new_with_mnemonic("E_xport");
  gtk_widget_set_sensitive(ltrgui->menubar_export, FALSE);
  submenu = gtk_menu_new();
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(ltrgui->menubar_export), submenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), ltrgui->menubar_export);
  menuitem = gtk_menu_item_new_with_mnemonic("Sequences as FAS_TA...");
  g_object_set_data(G_OBJECT(menuitem),
                    STATUSBAR_MENU_HINT, (gpointer) STATUSBAR_MENU_HINT_FASTA);
  g_signal_connect(G_OBJECT(menuitem), "enter-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(menuitem), "leave-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(menuitem), "activate",
                   G_CALLBACK(export_fasta_activate), ltrgui);
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem);
  menuitem = gtk_menu_item_new_with_mnemonic("Annotation as GFF_3...");
  g_object_set_data(G_OBJECT(menuitem),
                    STATUSBAR_MENU_HINT, (gpointer) STATUSBAR_MENU_HINT_EGFF3);
  g_signal_connect(G_OBJECT(menuitem), "enter-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(menuitem), "leave-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(menuitem), "activate",
                   G_CALLBACK(export_gff3_activate), ltrgui);
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem);

  menuitem = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  ltrgui->menubar_close = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE,
                                                             accelgroup);
  g_object_set_data(G_OBJECT(ltrgui->menubar_close),
                    STATUSBAR_MENU_HINT, (gpointer) STATUSBAR_MENU_HINT_CLOSE);
  gtk_widget_set_sensitive(ltrgui->menubar_close, FALSE);
  g_signal_connect(G_OBJECT(ltrgui->menubar_close), "enter-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(ltrgui->menubar_close), "leave-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(ltrgui->menubar_close), "activate",
                   G_CALLBACK(close_activate), ltrgui);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), ltrgui->menubar_close);

  menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accelgroup);
  g_object_set_data(G_OBJECT(menuitem),
                    STATUSBAR_MENU_HINT, (gpointer) STATUSBAR_MENU_HINT_QUIT);
  g_signal_connect(G_OBJECT(menuitem), "enter-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(menuitem), "leave-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(menuitem), "activate",
                   G_CALLBACK(quit_activate), ltrgui);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  /* Project menu */
  ltrgui->menubar_project = gtk_menu_item_new_with_mnemonic("_Project");
  gtk_widget_set_sensitive(ltrgui->menubar_project, FALSE);
  menu = gtk_menu_new();
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(ltrgui->menubar_project), menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(ltrgui->menubar),
                        ltrgui->menubar_project);

  menuitem = gtk_menu_item_new_with_mnemonic("_Filter...");
  g_object_set_data(G_OBJECT(menuitem),
                    STATUSBAR_MENU_HINT, (gpointer) STATUSBAR_MENU_HINT_FILTER);
  g_signal_connect(G_OBJECT(menuitem), "enter-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(menuitem), "leave-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(menuitem), "activate",
                   G_CALLBACK(project_filter_activate), ltrgui);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  menuitem = gtk_menu_item_new_with_mnemonic("_Match...");
  g_object_set_data(G_OBJECT(menuitem),
                    STATUSBAR_MENU_HINT,
                    (gpointer) STATUSBAR_MENU_HINT_MATCH);
  g_signal_connect(G_OBJECT(menuitem), "enter-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(menuitem), "leave-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(menuitem), "activate",
                   G_CALLBACK(project_match_activate), ltrgui);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

    menuitem = gtk_menu_item_new_with_mnemonic("Detect _ORFs...");
  g_object_set_data(G_OBJECT(menuitem),
                    STATUSBAR_MENU_HINT,
                    (gpointer) STATUSBAR_MENU_HINT_MATCH);
  g_signal_connect(G_OBJECT(menuitem), "enter-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(menuitem), "leave-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(menuitem), "activate",
                   G_CALLBACK(project_orf_activate), ltrgui);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  menuitem = gtk_menu_item_new_with_mnemonic("_Settings...");
  g_object_set_data(G_OBJECT(menuitem),
                    STATUSBAR_MENU_HINT,
                    (gpointer) STATUSBAR_MENU_HINT_SETTINGS);
  g_signal_connect(G_OBJECT(menuitem), "enter-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(menuitem), "leave-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(menuitem), "activate",
                   G_CALLBACK(project_settings_activate), ltrgui);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  /* View menu */
  menuitem = gtk_menu_item_new_with_mnemonic("_View");
  menu = gtk_menu_new();
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(ltrgui->menubar), menuitem);

  ltrgui->menubar_view_columns = gtk_menu_item_new_with_mnemonic("_Columns");
  gtk_widget_set_sensitive(ltrgui->menubar_view_columns, FALSE);
  g_object_set_data(G_OBJECT(ltrgui->menubar_view_columns),
                    STATUSBAR_MENU_HINT,
                    (gpointer) STATUSBAR_MENU_HINT_COLUMNS);
  g_signal_connect(G_OBJECT(ltrgui->menubar_view_columns), "enter-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  g_signal_connect(G_OBJECT(ltrgui->menubar_view_columns), "leave-notify-event",
                   G_CALLBACK(statusbar_menuhints), ltrgui);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), ltrgui->menubar_view_columns);

  /* Help menu */
  menuitem = gtk_menu_item_new_with_mnemonic("_Help");
  menu = gtk_menu_new();
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(ltrgui->menubar), menuitem);

  menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT, accelgroup);
  gtk_menu_item_set_label(GTK_MENU_ITEM(menuitem), "_About...");
  g_signal_connect(G_OBJECT(menuitem), "activate",
                   G_CALLBACK(help_about_activate), ltrgui);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  gtk_widget_show_all(ltrgui->menubar);
}
