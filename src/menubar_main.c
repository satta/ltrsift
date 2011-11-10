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
#include "menubar_main.h"
#include "statusbar_main.h"

static void reorder_tabs(gpointer key, gpointer value, gpointer user_data)
{

  gtk_notebook_reorder_child((GtkNotebook*) user_data,
                             (GtkWidget*) key,
                             GPOINTER_TO_INT(value));
}

static gint save_gui_settings(GUIData *ltrgui, const gchar *projectfile)
{
  GtkWidget *sw,
            *label_close;
  GtkNotebook *notebook;
  GtkTreeView *list_view;
  GtkTreeViewColumn *column;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtRDB *rdb;
  GtRDBStmt *stmt;
  GtArray *nodes;
  GtError *err;
  GList *children,
        *columns;
  gboolean valid;
  gchar *name,
        query[BUFSIZ];
  gint width,
       height,
       x, y, z, i, nop, had_err = 0;
  guint noc;

  err = gt_error_new();
  rdb = gt_rdb_sqlite_new(projectfile, err);
  if (!rdb) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not save gui settings: %s",
                gt_error_get(err));
    return -1;
  }

  had_err = gt_rdb_prepare(rdb,
                           "CREATE TABLE IF NOT EXISTS gui_settings "
                           "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "height INTEGER NOT NULL, "
                            "width INTEGER NOT NULL, "
                            "ltrfam INTEGER NOT NULL, "
                            "hpaned INTEGER NOT NULL, "
                            "vpaned INTEGER NOT NULL)",
                           -1, &stmt, err);  
  if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not save gui settings: %s",
                gt_error_get(err));
    gt_error_delete(err);
    gt_rdb_delete(rdb);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);

  had_err = gt_rdb_prepare(rdb,
                           "DELETE FROM gui_settings",
                           -1, &stmt, err);  
  if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not save gui settings: %s",
                gt_error_get(err));
    gt_error_delete(err);
    gt_rdb_delete(rdb);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);

  had_err = gt_rdb_prepare(rdb,
                           "CREATE TABLE IF NOT EXISTS empty_families "
                           "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "name TEXT)",
                           -1, &stmt, err);
  if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not save gui settings: %s",
                gt_error_get(err));
    gt_error_delete(err);
    gt_rdb_delete(rdb);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);

  had_err = gt_rdb_prepare(rdb,
                           "DELETE FROM empty_families",
                           -1, &stmt, err);
  if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not save gui settings: %s",
                gt_error_get(err));
    gt_error_delete(err);
    gt_rdb_delete(rdb);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);

  had_err = gt_rdb_prepare(rdb,
                           "CREATE TABLE IF NOT EXISTS invisible_columns "
                           "(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT)",
                           -1, &stmt, err);
  if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not save gui settings: %s",
                gt_error_get(err));
    gt_error_delete(err);
    gt_rdb_delete(rdb);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);

  had_err = gt_rdb_prepare(rdb,
                           "DELETE FROM invisible_columns",
                           -1, &stmt, err);
  if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not save gui settings: %s",
                gt_error_get(err));
    gt_error_delete(err);
    gt_rdb_delete(rdb);
    return -1;
  }

  had_err = gt_rdb_prepare(rdb,
                           "CREATE TABLE IF NOT EXISTS notebook_tabs "
                           "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "name TEXT, "
                            "position INTEGER NOT NULL)",
                           -1, &stmt, err);
  if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not save gui settings: %s",
                gt_error_get(err));
    gt_error_delete(err);
    gt_rdb_delete(rdb);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);

  had_err = gt_rdb_prepare(rdb,
                           "CREATE UNIQUE INDEX nbtabs ON notebook_tabs (name)",
                           -1, &stmt, err);
  if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not save gui settings: %s",
                gt_error_get(err));
    gt_error_delete(err);
    gt_rdb_delete(rdb);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);

  had_err = gt_rdb_prepare(rdb,
                           "DELETE FROM notebook_tabs",
                           -1, &stmt, err);
  if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not save gui settings: %s",
                gt_error_get(err));
    gt_error_delete(err);
    gt_rdb_delete(rdb);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);

  gtk_window_get_size(GTK_WINDOW(ltrgui->main_window), &width, &height);
  x = gtk_ltr_families_get_position(GTK_LTR_FAMILIES(ltrgui->ltr_families));
  y =
   gtk_ltr_families_get_hpaned_position(GTK_LTR_FAMILIES(ltrgui->ltr_families));
  z =
   gtk_ltr_families_get_vpaned_position(GTK_LTR_FAMILIES(ltrgui->ltr_families));

  g_snprintf(query, BUFSIZ,
             "INSERT INTO gui_settings "
             "(height, width, ltrfam, hpaned, vpaned) "
             "values (%d, %d, %d, %d, %d)",
             height,
             width,
             x, y, z);
  had_err = gt_rdb_prepare(rdb, query, -1, &stmt, err);
  if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not save gui settings: %s",
                gt_error_get(err));
    gt_error_delete(err);
    gt_rdb_delete(rdb);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);

  notebook = gtk_ltr_families_get_nb(GTK_LTR_FAMILIES(ltrgui->ltr_families));
  sw = gtk_notebook_get_nth_page(notebook, 0);
  children = gtk_container_get_children(GTK_CONTAINER(sw));
  list_view = GTK_TREE_VIEW(g_list_first(children)->data);
  columns = gtk_tree_view_get_columns(list_view);
  noc = g_list_length(columns);
  for (i = 0; i < noc; i++) {
    column = gtk_tree_view_get_column(list_view, i);
    if (!gtk_tree_view_column_get_visible(column)) {
      g_snprintf(query, BUFSIZ,
                 "INSERT INTO invisible_columns (name) values (\"%s\")",
                 gtk_tree_view_column_get_title(column));
      had_err = gt_rdb_prepare(rdb, query, -1, &stmt, err);
      if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
        g_set_error(&ltrgui->err,
                    G_FILE_ERROR,
                    0,
                    "Could not save gui settings: %s",
                    gt_error_get(err));
        gt_error_delete(err);
        g_list_free(children);
        g_list_free(columns);
        gt_rdb_delete(rdb);
        return -1;
      }
      gt_rdb_stmt_delete(stmt);
    }
  }
  g_list_free(children);
  g_list_free(columns);

  nop = gtk_notebook_get_n_pages(notebook);
  for (i = 0; i < nop; i++) {
    sw = gtk_notebook_get_nth_page(notebook, i);
    label_close = gtk_notebook_get_tab_label(notebook, sw);
    g_snprintf(query, BUFSIZ,
               "INSERT INTO notebook_tabs (name, position) values (\"%s\", %d)",
               gtk_label_close_get_text(GTKLABELCLOSE(label_close)),
               i);
    had_err = gt_rdb_prepare(rdb, query, -1, &stmt, err);
    if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
      g_set_error(&ltrgui->err,
                  G_FILE_ERROR,
                  0,
                  "Could not save gui settings: %s",
                  gt_error_get(err));
      gt_error_delete(err);
      gt_rdb_delete(rdb);
      return -1;
    }
    gt_rdb_stmt_delete(stmt);
  }

  list_view =
           gtk_ltr_families_get_lv_fams(GTK_LTR_FAMILIES(ltrgui->ltr_families));
  model = gtk_tree_view_get_model(list_view);

  valid = gtk_tree_model_get_iter_first(model, &iter);
  if (valid) {
    gtk_tree_model_get(model, &iter,
                       LTRFAMS_FAM_LV_NODE_ARRAY, &nodes,
                       LTRFAMS_FAM_LV_OLDNAME, &name,
                       -1);
    if (gt_array_size(nodes) == 0) {
      g_snprintf(query, BUFSIZ,
                 "INSERT INTO empty_families (name) values (\"%s\")",
                 name);
      had_err = gt_rdb_prepare(rdb, query, -1, &stmt, err);
      if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
        g_set_error(&ltrgui->err,
                    G_FILE_ERROR,
                    0,
                    "Could not save gui settings: %s",
                    gt_error_get(err));
        g_free(name);
        gt_error_delete(err);
        gt_rdb_delete(rdb);
        return -1;
      }
      gt_rdb_stmt_delete(stmt);
    }
    g_free(name);
    while (gtk_tree_model_iter_next(model, &iter)) {
      gtk_tree_model_get(model, &iter,
                         LTRFAMS_FAM_LV_NODE_ARRAY, &nodes,
                         LTRFAMS_FAM_LV_OLDNAME, &name,
                         -1);
      if (gt_array_size(nodes) == 0) {
        g_snprintf(query, BUFSIZ,
                   "INSERT INTO empty_families (name) values (\"%s\")",
                   name);
        had_err = gt_rdb_prepare(rdb,
                                 query,
                                 -1, &stmt, err);
        if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
          g_set_error(&ltrgui->err,
                      G_FILE_ERROR,
                      0,
                      "Could not save gui settings: %s",
                      gt_error_get(err));
          g_free(name);
          gt_error_delete(err);
          gt_rdb_delete(rdb);
          return -1;
        }
        gt_rdb_stmt_delete(stmt);
      }
      g_free(name);
    }
  }
  gt_rdb_delete(rdb);
  gt_error_delete(err);

  return had_err;
}

static gint apply_gui_settings(GUIData *ltrgui, const gchar *projectfile)
{
  GtRDB *rdb;
  GtRDBStmt *stmt = NULL;
  GtkWidget *sw,
            *label_close;
  GtkNotebook *nb;
  GtkTreeView *list_view;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreePath *path;
  GtArray *nodes;
  GtStr *result;
  GHashTable *hash;
  gboolean valid;
  gchar *name,
        buffer[BUFSIZ];
  gint had_err = 0, nop, i;
  GtError *err;
  err = gt_error_new();

  rdb = gt_rdb_sqlite_new(projectfile, err);
  if (!rdb) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not apply gui settings: %s",
                gt_error_get(err));
    gt_error_delete(err);
    return -1;
  }

  had_err = gt_rdb_prepare(rdb,
                           "SELECT height, width, ltrfam, hpaned, "
                           "vpaned FROM gui_settings",
                           -1,
                           &stmt,
                           err);

  if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not apply gui settings: %s",
                gt_error_get(err));
    gt_error_delete(err);
    gt_rdb_delete(rdb);
    return -1;
  }

  if (!had_err) {
    gint height, width, ltrfam, hpaned, vpaned;
    had_err = gt_rdb_stmt_get_int(stmt, 0, &height, err);
    had_err = gt_rdb_stmt_get_int(stmt, 1, &width, err);
    if (!had_err)
      gtk_window_resize(GTK_WINDOW(ltrgui->main_window), width, height);
      had_err = gt_rdb_stmt_get_int(stmt, 2, &ltrfam, err);
      had_err = gt_rdb_stmt_get_int(stmt, 3, &hpaned, err);
      had_err = gt_rdb_stmt_get_int(stmt, 4, &vpaned, err);
    if (!had_err) {

      gtk_ltr_families_set_position(GTK_LTR_FAMILIES(ltrgui->ltr_families),
                                    ltrfam);
      gtk_ltr_families_set_hpaned_position(GTK_LTR_FAMILIES(ltrgui->ltr_families),
                                           hpaned);
      gtk_ltr_families_set_vpaned_position(GTK_LTR_FAMILIES(ltrgui->ltr_families),
                                           vpaned);
    }
  }
  gt_rdb_stmt_delete(stmt);

  had_err = gt_rdb_prepare(rdb,
                           "SELECT name FROM empty_families",
                           -1, &stmt, NULL);
  if (!had_err) {
    list_view =
           gtk_ltr_families_get_lv_fams(GTK_LTR_FAMILIES(ltrgui->ltr_families));
    model = gtk_tree_view_get_model(list_view);
    while ((had_err = gt_rdb_stmt_exec(stmt, err)) == 0) {
      result = gt_str_new();
      nodes = gt_array_new(sizeof(GtGenomeNode*));
      had_err = gt_rdb_stmt_get_string(stmt, 0, result, err);
      if (!had_err) {
        g_snprintf(buffer, BUFSIZ, "%s (0)", gt_str_get(result));
        gtk_tree_store_append(GTK_TREE_STORE(model), &iter, NULL);
        gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
                           LTRFAMS_FAM_LV_NODE_ARRAY, nodes,
                           LTRFAMS_FAM_LV_TAB_CHILD, NULL,
                           LTRFAMS_FAM_LV_TAB_LABEL, NULL,
                           LTRFAMS_FAM_LV_OLDNAME, gt_str_get(result),
                           LTRFAMS_FAM_LV_CURNAME, buffer,
                           -1);
      }
      gt_str_delete(result);
    }
    gt_rdb_stmt_delete(stmt);
  }
  if (had_err == -1) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not apply gui settings: %s",
                gt_error_get(err));
    gt_error_delete(err);
    gt_rdb_delete(rdb);
    return had_err;
  }

  valid = gtk_tree_model_get_iter_first(model, &iter);
  if (valid) {
    GtArray *tmp_nodes;
    gtk_tree_model_get(model, &iter,
                       LTRFAMS_FAM_LV_OLDNAME, &name,
                       -1);
    g_snprintf(buffer, BUFSIZ,
               "SELECT name FROM notebook_tabs WHERE name = \"%s\"",
               name);
    g_free(name);
    had_err = gt_rdb_prepare(rdb, buffer, -1, &stmt, err);  
    if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
      g_set_error(&ltrgui->err,
                  G_FILE_ERROR,
                  0,
                  "Could not apply gui settings: %s",
                  gt_error_get(err));
      gt_error_delete(err);
      gt_rdb_delete(rdb);
      return had_err;
    }
    if (!had_err) {
      gtk_tree_model_get(model, &iter,
                         LTRFAMS_FAM_LV_NODE_ARRAY, &tmp_nodes,
                         -1);
      path = gtk_tree_model_get_path(model, &iter);
      gtk_ltr_families_nb_fam_add_tab(model, &iter, tmp_nodes, TRUE,
                                      GTK_LTR_FAMILIES(ltrgui->ltr_families));
      gtk_tree_path_free(path);
    }
    gt_rdb_stmt_delete(stmt);
    while (gtk_tree_model_iter_next(model, &iter)) {
      gtk_tree_model_get(model, &iter,
                         LTRFAMS_FAM_LV_OLDNAME, &name,
                         -1);
      g_snprintf(buffer, BUFSIZ,
                 "SELECT name FROM notebook_tabs WHERE name = \"%s\"",
                 name);
      g_free(name);
      had_err = gt_rdb_prepare(rdb, buffer, -1, &stmt, err);
      if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
        g_set_error(&ltrgui->err,
                    G_FILE_ERROR,
                    0,
                    "Could not apply gui settings: %s",
                    gt_error_get(err));
        gt_error_delete(err);
        gt_rdb_delete(rdb);
        return -1;
      }
      if (!had_err) {
        path = gtk_tree_model_get_path(model, &iter);
        gtk_tree_model_get(model, &iter,
                           LTRFAMS_FAM_LV_NODE_ARRAY, &tmp_nodes,
                           -1);
        gtk_ltr_families_nb_fam_add_tab(model, &iter, tmp_nodes, TRUE,
                                        GTK_LTR_FAMILIES(ltrgui->ltr_families));
        gtk_tree_path_free(path);
      }
      gt_rdb_stmt_delete(stmt);
    }
  }

  had_err = 0;
  nb = gtk_ltr_families_get_nb(GTK_LTR_FAMILIES(ltrgui->ltr_families));
  nop = gtk_notebook_get_n_pages(nb);
  if (nop > 1) {
    gint tab_pos;
    hash = g_hash_table_new(NULL, NULL);
    for (i = 0; i < nop; i++) {
      sw = gtk_notebook_get_nth_page(nb, i);
      label_close = gtk_notebook_get_tab_label(nb, sw);
      g_snprintf(buffer, BUFSIZ,
                 "SELECT position FROM notebook_tabs WHERE name = \"%s\"",
                 gtk_label_close_get_text(GTKLABELCLOSE(label_close)));
      had_err = gt_rdb_prepare(rdb, buffer, -1, &stmt, err);
      if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
        g_set_error(&ltrgui->err,
                    G_FILE_ERROR,
                    0,
                    "Could not apply gui settings: %s",
                    gt_error_get(err));
        gt_error_delete(err);
        gt_rdb_delete(rdb);
        return -1;
      }
      if (!had_err) {
        had_err = gt_rdb_stmt_get_int(stmt, 0, &tab_pos, err);
        if (!had_err)
          g_hash_table_insert(hash, (gpointer) sw, GINT_TO_POINTER(tab_pos));
      }
      gt_rdb_stmt_delete(stmt);
    }
    if (!had_err)
      g_hash_table_foreach(hash, (GHFunc) reorder_tabs, (gpointer) nb);
    g_hash_table_destroy(hash);
  }
  if (had_err) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not apply gui settings: %s",
                gt_error_get(err));
  }
  gt_error_delete(err);
  gt_rdb_delete(rdb);

  return had_err;
}

static gboolean mb_main_update_progress_dialog(gpointer data)
{
  MenuThreadData *threaddata = (MenuThreadData*) data;
  if (!threaddata->open)
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(threaddata->progressbar),
                                  (gdouble) threaddata->progress / gt_array_size(threaddata->nodes));
  else
    gtk_progress_bar_pulse(GTK_PROGRESS_BAR(threaddata->progressbar));
  return TRUE;
}

static void mb_main_progress_dialog_init(MenuThreadData *threaddata)
{
  GtkWidget *label,
            *vbox;
  guint sid;

  /* create the modal window which warns the user to wait */
  threaddata->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_modal(GTK_WINDOW(threaddata->window), TRUE);
  gtk_window_set_title(GTK_WINDOW(threaddata->window), "Progress");
  gtk_window_resize(GTK_WINDOW(threaddata->window), 200, 50);
  gtk_window_set_position(GTK_WINDOW(threaddata->window),
                          GTK_WIN_POS_CENTER_ALWAYS);
  gtk_container_set_border_width(GTK_CONTAINER(threaddata->window), 12);
  g_signal_connect(threaddata->window, "delete_event",
                   G_CALLBACK(gtk_true), NULL);
  vbox = gtk_vbox_new(FALSE, 12);
  /* create label */
  label = gtk_label_new("Please wait...");
  gtk_container_add(GTK_CONTAINER(vbox), label);
  /* create progress bar */
  threaddata->progressbar = gtk_progress_bar_new();
  gtk_container_add(GTK_CONTAINER(vbox), threaddata->progressbar);
  /* add vbox to dialog */
  gtk_container_add(GTK_CONTAINER(threaddata->window), vbox);
  gtk_widget_show_all(threaddata->window);
  /* refresh the progress dialog */
  sid = g_timeout_add(100, mb_main_update_progress_dialog,
                      (gpointer) threaddata);
  g_object_set_data(G_OBJECT(threaddata->window),
                    "source_id", GINT_TO_POINTER(sid));
}

static gboolean mb_save_project_data_finished(gpointer data)
{
  MenuThreadData *threaddata = (MenuThreadData*) data;
  GtkWidget *ltrfams = threaddata->ltrgui->ltr_families;
  g_source_remove(GPOINTER_TO_INT(
                               g_object_get_data(G_OBJECT(threaddata->window),
                                                 "source_id")));
  gtk_widget_destroy(GTK_WIDGET(threaddata->window)); 
  if (!threaddata->had_err) {
    gtk_ltr_families_set_modified(GTK_LTR_FAMILIES(ltrfams), FALSE);
    threaddata->had_err = save_gui_settings(threaddata->ltrgui,
                                            threaddata->filename);
    if (!threaddata->had_err)
      threaddata->had_err =
                  gtk_project_settings_save_data(GTK_PROJECT_SETTINGS(
                                                  threaddata->ltrgui->projset));
  } else
    g_rename(threaddata->tmp_filename, threaddata->filename);
  if (threaddata->had_err)
    error_handle(threaddata->ltrgui);

  gt_error_delete(threaddata->err);
  g_free(threaddata->tmp_filename);
  g_slice_free(MenuThreadData, threaddata);

  return FALSE;
}

gboolean mb_save_as_project_data_finished(gpointer data)
{
  MenuThreadData *threaddata = (MenuThreadData*) data;
  GtkWidget *ltrfams = threaddata->ltrgui->ltr_families,
            *projset = threaddata->ltrgui->projset;

  g_source_remove(GPOINTER_TO_INT(
                               g_object_get_data(G_OBJECT(threaddata->window),
                                                 "source_id")));
  gtk_widget_destroy(GTK_WIDGET(threaddata->window));

  if (threaddata->had_err) {
    g_set_error(&threaddata->ltrgui->err,
                G_FILE_ERROR,
                0,
                "Error while saving data: %s",
                gt_error_get(threaddata->err));
  }

  if (!threaddata->had_err) {
    threaddata->had_err = save_gui_settings(threaddata->ltrgui,
                                            threaddata->filename);
    gtk_project_settings_update_projectfile(GTK_PROJECT_SETTINGS(projset),
                                            threaddata->filename);
    threaddata->had_err =
                  gtk_project_settings_save_data(GTK_PROJECT_SETTINGS(projset));
  }
  if (!threaddata->had_err) {
    gtk_ltr_families_set_projectfile(GTK_LTR_FAMILIES(ltrfams),
                                     threaddata->filename);
    gtk_ltr_families_set_modified(GTK_LTR_FAMILIES(ltrfams), FALSE);
    create_recently_used_resource(threaddata->filename);
  } else {
    if (threaddata->bakfile)
      g_rename(threaddata->tmp_filename, threaddata->filename);
    error_handle(threaddata->ltrgui);
  }

  gt_error_delete(threaddata->err);
  g_free(threaddata->tmp_filename);
  g_slice_free(MenuThreadData, threaddata);

  return FALSE;
}

static gboolean mb_open_project_data_finished(gpointer data)
{
  MenuThreadData *threaddata = (MenuThreadData*) data;
  GtkWidget *ltrfams = threaddata->ltrgui->ltr_families;

  g_source_remove(GPOINTER_TO_INT(
                               g_object_get_data(G_OBJECT(threaddata->window),
                                                 "source_id")));
  gtk_widget_destroy(GTK_WIDGET(threaddata->window));

  if (!threaddata->had_err) {
    gtk_widget_destroy(ltrfams);
    threaddata->ltrgui->ltr_families = gtk_ltr_families_new();
    ltrfams = threaddata->ltrgui->ltr_families;
    gtk_box_pack_start(GTK_BOX(threaddata->ltrgui->vbox1_main),
                       ltrfams, TRUE, TRUE, 0);
    gtk_ltr_families_fill_with_data(GTK_LTR_FAMILIES(ltrfams),
                                    threaddata->nodes,
                                    threaddata->features,
                                    threaddata->n_features);
    gtk_ltr_families_set_projectfile(GTK_LTR_FAMILIES(ltrfams),
                                     threaddata->filename);
    gtk_project_settings_set_data_from_sqlite(GTK_PROJECT_SETTINGS(threaddata->ltrgui->projset),
                                              threaddata->filename);
    mb_main_view_columns_set_submenu(threaddata->ltrgui,
                                     threaddata->features,
                                     threaddata->err, TRUE);
    mb_main_activate_menuitems(threaddata->ltrgui);
    gtk_ltr_families_set_modified(GTK_LTR_FAMILIES(ltrfams), FALSE);
    create_recently_used_resource(threaddata->filename);
  } else {
    g_set_error(&threaddata->ltrgui->err,
                G_FILE_ERROR,
                0,
                "Error while opening data: %s",
                gt_error_get(threaddata->err));
  }
  if (!threaddata->had_err)
    threaddata->had_err = apply_gui_settings(threaddata->ltrgui,
                                             threaddata->filename);
  if (threaddata->had_err)
    error_handle(threaddata->ltrgui);

  gt_error_delete(threaddata->err);
  g_slice_free(MenuThreadData, threaddata);

  return FALSE;
}

static gpointer mb_main_save_project_data_start(gpointer data)
{
  MenuThreadData *threaddata = (MenuThreadData*) data;
  GtkWidget *ltrfams = threaddata->ltrgui->ltr_families;
  GtNodeStream *array_in_stream = NULL,
               *feature_stream = NULL;
  GtRDB *rdb = NULL;
  GtAnnoDB *adb = NULL;
  GtFeatureIndex *fi = NULL;
  unsigned long i;

  gtk_progress_bar_set_text(GTK_PROGRESS_BAR(threaddata->progressbar),
                            "Saving data");

  rdb = gt_rdb_sqlite_new(threaddata->filename, threaddata->err);
  if (!rdb)
    threaddata->had_err = -1;
  if (!threaddata->had_err) {
    adb = gt_anno_db_sqlite_new((GtRDBSqlite*) rdb, threaddata->err);
    if (!adb)
      threaddata->had_err = -1;
  }
  if (!threaddata->had_err) {
    fi = gt_feature_index_anno_db_new(adb, threaddata->err);
    if (!fi)
      threaddata->had_err = -1;
  }
  if (!threaddata->had_err) {
    threaddata->nodes = gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrfams));
    for (i = 0; i < gt_array_size(threaddata->nodes); i++) {
      GtGenomeNode *gn;
      gn = gt_genome_node_ref(*(GtGenomeNode**) gt_array_get(threaddata->nodes,
                                                             i));
      gn = gt_genome_node_ref(*(GtGenomeNode**) gt_array_get(threaddata->nodes,
                                                             i));
    }
    array_in_stream = gt_array_in_stream_new(threaddata->nodes,
                                             &threaddata->progress,
                                             threaddata->err);
    gt_assert(array_in_stream);

    feature_stream = gt_feature_stream_new(array_in_stream, fi);
    threaddata->had_err = gt_node_stream_pull(feature_stream, threaddata->err);
  }
  gt_node_stream_delete(feature_stream);
  gt_node_stream_delete(array_in_stream);
  gt_feature_index_delete(fi);
  gt_anno_db_delete(adb);
  gt_rdb_delete(rdb);
  if (threaddata->save_as)
    g_idle_add(mb_save_as_project_data_finished, data);
  else
    g_idle_add(mb_save_project_data_finished, data);
  return NULL;
}

static gpointer mb_main_open_project_data_start(gpointer data)
{
  MenuThreadData *threaddata = (MenuThreadData*) data;
  GtNodeStream *last_stream = NULL,
               *preprocess_stream = NULL,
               *array_in_stream = NULL;
  GtHashmap *features;
  GtArray *nodes,
          *tmp_nodes;
  GtFeatureIndex *fi = NULL;
  GtRDB *rdb;
  GtAnnoDB *adb;
  GtStrArray *seqids = NULL;
  unsigned long i, n_features;
  gtk_progress_bar_set_text(GTK_PROGRESS_BAR(threaddata->progressbar),
                            "Reading data from file");

  rdb = gt_rdb_sqlite_new(threaddata->filename, threaddata->err);
  if (!rdb)
    threaddata->had_err = -1;
  if (!threaddata->had_err) {
    adb = gt_anno_db_sqlite_new((GtRDBSqlite*) rdb, threaddata->err);
    if (!adb)
      threaddata->had_err = -1;
  }
  if (!threaddata->had_err) {
    fi = gt_feature_index_anno_db_new(adb, threaddata->err);
    threaddata->had_err = fi ? 0 : -1;
  }
  if (!threaddata->had_err) {
    nodes = gt_array_new(sizeof(GtFeatureNode*));
    seqids = gt_feature_index_get_seqids(fi, threaddata->err);
    if (seqids) {
      for (i = 0; i < gt_str_array_size(seqids); i++) {
        tmp_nodes =
            gt_feature_index_get_features_for_seqid(fi,
                                                    gt_str_array_get(seqids, i),
                                                    threaddata->err);
        gt_array_add_array(nodes, tmp_nodes);
        gt_array_delete(tmp_nodes);
        tmp_nodes = NULL;
      }
      gt_str_array_delete(seqids);
    }
  }
  gt_rdb_delete(rdb);
  gt_anno_db_delete(adb);
  if (!threaddata->had_err) {
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(threaddata->progressbar),
                              "Preprocessing candidates");
    features = gt_hashmap_new(GT_HASH_STRING, free_hash, NULL);
    n_features = LTRFAMS_LV_N_COLUMS;

    for (i = 0; i < gt_array_size(nodes); i++) {
      GtGenomeNode *gn;
      gn = gt_genome_node_ref(*(GtGenomeNode**) gt_array_get(nodes, i));
    }
    last_stream = array_in_stream = gt_array_in_stream_new(nodes, NULL,
                                                           threaddata->err);
    last_stream = preprocess_stream =
                                    gt_ltrgui_preprocess_stream_new(last_stream,
                                                                    features,
                                                                    &n_features,
                                                                    FALSE,
                                                                    threaddata->err);
    threaddata->had_err = gt_node_stream_pull(last_stream, threaddata->err);
  }
  gt_node_stream_delete(array_in_stream);
  gt_node_stream_delete(preprocess_stream);
  threaddata->nodes = nodes;
  threaddata->features = features;
  threaddata->n_features = n_features;
  //gt_feature_index_delete(fi);
  g_idle_add(mb_open_project_data_finished, data);
  return NULL;
}

void mb_main_activate_menuitems(GUIData *ltrgui)
{
  gtk_widget_set_sensitive(ltrgui->mb_main_view_columns, TRUE);
  gtk_widget_set_sensitive(ltrgui->mb_main_file_save, TRUE);
  gtk_widget_set_sensitive(ltrgui->mb_main_file_save_as, TRUE);
  gtk_widget_set_sensitive(ltrgui->mb_main_file_export_fasta, TRUE);
  gtk_widget_set_sensitive(ltrgui->mb_main_file_export_gff3, TRUE);
  gtk_widget_set_sensitive(ltrgui->mb_main_file_close, TRUE);
}

static int get_keys(void *key, GT_UNUSED void *values, void *cptns,
                         GT_UNUSED GtError *err)
{
  GtStrArray *captions = (GtStrArray*) cptns;
  const char *caption = (const char*) key;

  gt_str_array_add_cstr(captions, caption);

  return 0;
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
  gtk_box_pack_start_defaults(GTK_BOX (hbox), label);
  gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG (dialog)->vbox), hbox);
  gtk_widget_show_all(dialog);

  return dialog;
}

void mb_main_file_save_activate(GT_UNUSED GtkMenuItem *menuitem,
                                GUIData *ltrgui)
{
  MenuThreadData *threaddata;
  gchar *projectfile;

  projectfile =
       gtk_ltr_families_get_projectfile(GTK_LTR_FAMILIES(ltrgui->ltr_families));
  if (projectfile == NULL) {
    mb_main_file_save_as_activate(NULL, ltrgui);
    return;
  } else {
    threaddata = g_slice_new(MenuThreadData);
    threaddata->filename = projectfile;
    threaddata->tmp_filename =  g_strdup_printf("%s.bak", projectfile);
    threaddata->ltrgui = ltrgui;
    threaddata->nodes = NULL;
    threaddata->features = NULL;
    threaddata->err = gt_error_new();
    threaddata->save_as = FALSE;
    threaddata->open = FALSE;
    threaddata->bakfile = FALSE;
    threaddata->progress = 0;
    threaddata->n_features = GT_UNDEF_ULONG;
    threaddata->had_err = 0;
    g_rename(projectfile, threaddata->tmp_filename);

    mb_main_progress_dialog_init(threaddata);
    g_thread_create(mb_main_save_project_data_start,
                    (gpointer) threaddata, FALSE, NULL);
  }
}

void mb_main_file_save_as_activate(GT_UNUSED GtkMenuItem *menuitem,
                                      GUIData *ltrgui)
{
  MenuThreadData *threaddata;
  GtkWidget *filechooser,
            *dialog;
  gchar *filename;
  const gchar *projectfile;
  gboolean bakfile = FALSE;

  filechooser = gtk_file_chooser_dialog_new(GUI_DIALOG_SAVE_AS,
                                            GTK_WINDOW (ltrgui->main_window),
                                            GTK_FILE_CHOOSER_ACTION_SAVE,
                                            GTK_STOCK_CANCEL,
                                            GTK_RESPONSE_CANCEL,
                                            GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                            NULL);
  projectfile =
       gtk_ltr_families_get_projectfile(GTK_LTR_FAMILIES(ltrgui->ltr_families));
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

  if (gtk_dialog_run(GTK_DIALOG(filechooser)) == GTK_RESPONSE_ACCEPT)
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser));
  else {
    gtk_widget_destroy(filechooser);
    return;
  }
  gtk_widget_destroy(filechooser);

  threaddata = g_slice_new(MenuThreadData);
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
      g_slice_free(MenuThreadData, threaddata);
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
  threaddata->nodes = NULL;
  threaddata->features = NULL;
  threaddata->err = gt_error_new();
  threaddata->progress = 0;
  threaddata->n_features = GT_UNDEF_ULONG;
  threaddata->save_as = TRUE;
  threaddata->open = FALSE;
  threaddata->bakfile = bakfile;
  threaddata->had_err = 0;

  mb_main_progress_dialog_init(threaddata);
  g_thread_create(mb_main_save_project_data_start,
                  (gpointer) threaddata, FALSE, NULL);
}

void mb_main_file_export_gff3_activate(GT_UNUSED GtkMenuItem *menuitem,
                                       GUIData *ltrgui)
{
  GtkWidget *dialog;
  GtArray *nodes;
  GtNodeStream *last_stream = NULL,
               *array_in_stream = NULL,
               *gff3_out_stream = NULL;
  GtFile *outfp;
  GtError *err;
  gchar *filename,
        tmp_filename[BUFSIZ];
  const gchar *projectfile;
  int had_err;
  unsigned long i;
  gboolean bakfile = FALSE;


  nodes = gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrgui->ltr_families));

  if (gt_array_size(nodes) == 0) {
    dialog = gtk_message_dialog_new(GTK_WINDOW(ltrgui->main_window),
                                    GTK_DIALOG_MODAL,
                                    GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                    "%s",
                                    NO_DATA_DIALOG);
    gtk_window_set_title(GTK_WINDOW(dialog), "Information!");
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }

  dialog = gtk_file_chooser_dialog_new("Export as GFF3...",
                                       GTK_WINDOW(ltrgui->main_window),
                                       GTK_FILE_CHOOSER_ACTION_SAVE,
                                       GTK_STOCK_CANCEL,
                                       GTK_RESPONSE_CANCEL, "E_xport",
                                       GTK_RESPONSE_ACCEPT, NULL);
  projectfile =
      gtk_ltr_families_get_projectfile(GTK_LTR_FAMILIES(ltrgui->ltr_families));
  if (projectfile != NULL)
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),
                                        g_path_get_dirname(projectfile));
  else
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),
                                        g_get_home_dir());

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    gtk_widget_destroy(dialog);

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

    for (i = 0; i < gt_array_size(nodes); i++) {
      GtGenomeNode *gn;
      gn = gt_genome_node_ref(*(GtGenomeNode**) gt_array_get(nodes, i));
    }

    last_stream = array_in_stream = gt_array_in_stream_new(nodes, NULL ,err);
    last_stream = gff3_out_stream = gt_gff3_out_stream_new(last_stream, outfp);

    had_err = gt_node_stream_pull(last_stream, err);
    gt_file_delete(outfp);

    if (had_err) {
      if (bakfile)
        g_rename(tmp_filename, filename);
      g_set_error(&ltrgui->err,
                  G_FILE_ERROR,
                  0,
                  "Error while exporting data: %s",
                  gt_error_get(err));
      error_handle(ltrgui);
    }
    gt_node_stream_delete(array_in_stream);
    gt_node_stream_delete(gff3_out_stream);
    gt_error_delete(err);
  } else
    gtk_widget_destroy(dialog);
}

static void mb_main_view_columns_toggled(GtkCheckMenuItem *menuitem,
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
  notebook = gtk_ltr_families_get_nb(GTK_LTR_FAMILIES(user_data));
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
    GtRDB *rdb = NULL;
    GtRDBStmt *stmt = NULL;
    GtError *err = gt_error_new();
    gint had_err = 0;
    gchar query[BUFSIZ];

    rdb = gt_rdb_sqlite_new(file, err);
    if (!rdb)
      return;
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
    gt_rdb_stmt_delete(stmt);
    gt_rdb_delete(rdb);
  }
}

void mb_main_view_columns_set_submenu(GUIData *ltrgui, GtHashmap *features,
                                      GtError *err, gboolean sqlt)
{
  GtkWidget *menu,
            *menuitem;
  GtStrArray *captions;
  GtRDB *rdb = NULL;
  GtRDBStmt *stmt;
  gchar buffer[BUFSIZ];
  const gchar *projectfile;
  gint had_err = 0;
  unsigned long i;

  captions = gt_str_array_new();
  menu = gtk_menu_new();

  gt_hashmap_foreach(features, get_keys, (void*) captions, err);

  if (sqlt) {
    projectfile =
       gtk_ltr_families_get_projectfile(GTK_LTR_FAMILIES(ltrgui->ltr_families));
    if (projectfile)
      rdb = gt_rdb_sqlite_new(projectfile, err);
   }

  for (i = 0; i < gt_str_array_size(captions); i++) {
    menuitem = gtk_check_menu_item_new_with_label(gt_str_array_get(captions,
                                                                   i));
    if (sqlt && rdb) {
      g_signal_connect(G_OBJECT(menuitem), "toggled",
                       G_CALLBACK(mb_main_view_columns_toggled),
                       ltrgui->ltr_families);
      g_snprintf(buffer, BUFSIZ,
                 "SELECT name FROM invisible_columns WHERE name = \"%s\"",
                 double_underscores(gt_str_array_get(captions, i)));
      had_err = gt_rdb_prepare(rdb, buffer, -1, &stmt, err);
      if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
        gt_rdb_delete(rdb);
        rdb = NULL;
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), TRUE);
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
                       G_CALLBACK(mb_main_view_columns_toggled),
                       ltrgui->ltr_families);
    }
    if (g_strcmp0(gt_str_array_get(captions, i), FNT_LLTR) == 0)
      gtk_menu_shell_insert(GTK_MENU_SHELL(menu), menuitem, 0);
    else if (g_strcmp0(gt_str_array_get(captions, i), FNT_RLTR) == 0)
      gtk_menu_shell_insert(GTK_MENU_SHELL(menu), menuitem, 1);
    else
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
  }
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(ltrgui->mb_main_view_columns), menu);
  gtk_widget_show_all(menu);
  gtk_widget_set_sensitive(ltrgui->mb_main_view_columns, TRUE);
  if (rdb)
    gt_rdb_delete(rdb);
  gt_str_array_delete(captions);
}

void mb_main_file_import_activate(GT_UNUSED GtkMenuItem *menuitem,
                                  GUIData *ltrgui)
{
  GtkWidget *filechooser;
  GtkFileFilter *gff3_file_filter;
  GtArray *nodes;
  GtHashmap *features;
  GtNodeStream *last_stream = NULL,
               *gff3_in_stream = NULL,
               *preprocess_stream = NULL,
               *array_out_stream = NULL;
  GtError *err = gt_error_new();
  int had_err = 0;
  unsigned long n_features;
  gchar *filename;

  /* TODO: check for modified project (check for save) */

  filechooser = gtk_file_chooser_dialog_new(GUI_DIALOG_IMPORT,
                                            GTK_WINDOW (ltrgui->main_window),
                                            GTK_FILE_CHOOSER_ACTION_OPEN,
                                            GTK_STOCK_CANCEL,
                                            GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN,
                                            GTK_RESPONSE_ACCEPT, NULL);
  gff3_file_filter = gtk_file_filter_new();
  gtk_file_filter_set_name(gff3_file_filter, GFF3_PATTERN);
  gtk_file_filter_add_pattern(gff3_file_filter, GFF3_PATTERN);
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filechooser), gff3_file_filter);

  if (gtk_dialog_run(GTK_DIALOG(filechooser)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser));
    gtk_widget_destroy(filechooser);
  } else {
    gtk_widget_destroy(filechooser);
    return;
  }

  nodes = gt_array_new(sizeof(GtGenomeNode*));
  features = gt_hashmap_new(GT_HASH_STRING, free_hash, NULL);
  n_features = LTRFAMS_LV_N_COLUMS;
  last_stream = gff3_in_stream = gt_gff3_in_stream_new_sorted(filename);
  last_stream = preprocess_stream = gt_ltrgui_preprocess_stream_new(last_stream,
                                                                    features,
                                                                    &n_features,
                                                                    FALSE,
                                                                    err);
  last_stream = array_out_stream = gt_array_out_stream_new(last_stream, nodes,
                                                           err);
  had_err = gt_node_stream_pull(last_stream, err);
  if (!had_err) {
    gtk_ltr_families_fill_with_data(GTK_LTR_FAMILIES(ltrgui->ltr_families),
                                    nodes,
                                    features,
                                    n_features);
    mb_main_view_columns_set_submenu(ltrgui, features, err, FALSE);
    mb_main_activate_menuitems(ltrgui);
  } else {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not import data: %s",
                gt_error_get(err));
    error_handle(ltrgui);
  }
  gt_node_stream_delete(gff3_in_stream);
  gt_node_stream_delete(preprocess_stream);
  gt_node_stream_delete(array_out_stream);
  gt_error_delete(err);
}

void mb_main_file_open_activate(GT_UNUSED GtkMenuItem *menuitem,
                                GUIData *ltrgui)
{
  MenuThreadData *threaddata;
  GtkWidget *filechooser,
            *dialog;
  gchar *filename = NULL;
  gint response = GTK_RESPONSE_REJECT;

  if (gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrgui->ltr_families))) {
    if (!gtk_ltr_families_get_projectfile(
                                        GTK_LTR_FAMILIES(ltrgui->ltr_families))) {
      dialog = unsaved_changes_dialog(ltrgui, NO_PROJECT_DIALOG);
      response = gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
    } else if (gtk_ltr_families_get_modified(GTK_LTR_FAMILIES(ltrgui->ltr_families))) {
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
      mb_main_file_save_activate(NULL, ltrgui);
      return;
      break;
    case GTK_RESPONSE_REJECT:
      break;
    default:
      return;
      break;
  }

  filechooser = gtk_file_chooser_dialog_new(GUI_DIALOG_OPEN,
                                            GTK_WINDOW (ltrgui->main_window),
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

  threaddata = g_slice_new(MenuThreadData);
  threaddata->ltrgui = ltrgui;
  threaddata->filename = filename;
  threaddata->nodes = NULL;
  threaddata->err = gt_error_new();
  threaddata->progress = GT_UNDEF_ULONG;
  threaddata->save_as = FALSE;
  threaddata->bakfile = FALSE;
  threaddata->open = TRUE;
  threaddata->had_err = 0;
  mb_main_progress_dialog_init(threaddata);

  g_thread_create(mb_main_open_project_data_start,
                  (gpointer) threaddata, FALSE, NULL);
}

void mb_main_file_new_activate(GT_UNUSED GtkMenuItem *menuitem, GUIData *ltrgui)
{
  GtkWidget *dialog;
  gint response = 0;

  if (gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrgui->ltr_families))) {
    if (!gtk_ltr_families_get_projectfile(
                                        GTK_LTR_FAMILIES(ltrgui->ltr_families))) {
      dialog = unsaved_changes_dialog(ltrgui, NO_PROJECT_DIALOG);
      response = gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
    } else if (gtk_ltr_families_get_modified(GTK_LTR_FAMILIES(ltrgui->ltr_families))) {
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
      mb_main_file_save_activate(NULL, ltrgui);
      break;
    case GTK_RESPONSE_REJECT:
      break;
    default:
      break;
  }
  gtk_widget_show(ltrgui->assistant);
}

void mb_main_file_close_activate(GT_UNUSED GtkMenuItem *menuitem,
                                 GUIData *ltrgui)
{
  GtkWidget *dialog;
  gint response = GTK_RESPONSE_REJECT;

  if (!gtk_ltr_families_get_projectfile(
                                      GTK_LTR_FAMILIES(ltrgui->ltr_families))) {
    dialog = unsaved_changes_dialog(ltrgui, NO_PROJECT_DIALOG);
    response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  } else if (gtk_ltr_families_get_modified(GTK_LTR_FAMILIES(ltrgui->ltr_families))) {
    dialog = unsaved_changes_dialog(ltrgui, UNSAVED_CHANGES_DIALOG);
    response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  }

  switch (response) {
    case GTK_RESPONSE_CANCEL:
      return;
      break;
    case GTK_RESPONSE_ACCEPT:
      mb_main_file_save_activate(NULL, ltrgui);
      break;
    case GTK_RESPONSE_REJECT:
      gtk_widget_set_sensitive(ltrgui->mb_main_view_columns, FALSE);
      gtk_widget_set_sensitive(ltrgui->mb_main_file_save, FALSE);
      gtk_widget_set_sensitive(ltrgui->mb_main_file_save_as, FALSE);
      gtk_widget_set_sensitive(ltrgui->mb_main_file_export_fasta, FALSE);
      gtk_widget_set_sensitive(ltrgui->mb_main_file_export_gff3, FALSE);
      gtk_widget_set_sensitive(ltrgui->mb_main_file_close, FALSE);
      gtk_window_set_title(GTK_WINDOW(ltrgui->main_window), GUI_NAME);
      gtk_widget_destroy(ltrgui->ltr_families);
      ltrgui->ltr_families = gtk_ltr_families_new();
      gtk_box_pack_start(GTK_BOX(ltrgui->vbox1_main), ltrgui->ltr_families,
                         TRUE, TRUE, 0);
      break;
    default:
      break;
  }
}

void mb_main_help_about_activate(GT_UNUSED GtkMenuItem *menutiem,
                                 GT_UNUSED GUIData *ltrgui)
{
  GtkWidget *dialog;
  GdkPixbuf *logo;
  GError *err = NULL;

  dialog = gtk_about_dialog_new();

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
  gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(dialog), GUI_LICENSE);
  gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), GUI_WEBSITE);

  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

static void mb_main_file_open_recent_activated(GtkRecentChooser *rc,
                                               GUIData *ltrgui)
{
  MenuThreadData *threaddata;
  GtkWidget *dialog;
  gchar *filename;
  gint response = GTK_RESPONSE_REJECT;

  if (gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrgui->ltr_families))) {
    if (!gtk_ltr_families_get_projectfile(
                                        GTK_LTR_FAMILIES(ltrgui->ltr_families))) {
      dialog = unsaved_changes_dialog(ltrgui, NO_PROJECT_DIALOG);
      response = gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
    } else if (gtk_ltr_families_get_modified(GTK_LTR_FAMILIES(ltrgui->ltr_families))) {
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
      mb_main_file_save_activate(NULL, ltrgui);
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

  threaddata = g_slice_new(MenuThreadData);
  threaddata->ltrgui = ltrgui;
  threaddata->filename = filename;
  threaddata->err = gt_error_new();
  threaddata->progress = GT_UNDEF_ULONG;
  threaddata->save_as = FALSE;
  threaddata->bakfile = FALSE;
  threaddata->open = TRUE;
  threaddata->had_err = 0;
  mb_main_progress_dialog_init(threaddata);

  g_thread_create(mb_main_open_project_data_start,
                  (gpointer) threaddata, FALSE, NULL);
}

void mb_main_file_quit_activate(GT_UNUSED GtkMenuItem *menuitem,
                                GUIData *ltrgui)
{
  GtkWidget *dialog;
  gint response = GTK_RESPONSE_REJECT;

  if (gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrgui->ltr_families))) {
    if (!gtk_ltr_families_get_projectfile(
                                        GTK_LTR_FAMILIES(ltrgui->ltr_families))) {
      dialog = unsaved_changes_dialog(ltrgui, NO_PROJECT_DIALOG);
      response = gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
    } else if (gtk_ltr_families_get_modified(GTK_LTR_FAMILIES(ltrgui->ltr_families))) {
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
      mb_main_file_save_activate(NULL, ltrgui);
      return;
      break;
    case GTK_RESPONSE_REJECT:
      break;
    default:
      break;
  }
  gtk_main_quit();
}

void mb_main_file_export_fasta_activate(GT_UNUSED GtkMenuItem *menuitem,
                                        GUIData *ltrgui)
{
  GtkWidget *dialog,
            *projset = ltrgui->projset;
  GtArray *nodes;
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
  const gchar *projectfile,
              *indexname;
  char *buffer, header[BUFSIZ];
  const char *attr;
  gint had_err;
  unsigned long i,
                seqnum,
                startpos;
  gboolean bakfile = FALSE;

  nodes = gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrgui->ltr_families));

  if (gt_array_size(nodes) == 0) {
    dialog = gtk_message_dialog_new(GTK_WINDOW(ltrgui->main_window),
                                    GTK_DIALOG_MODAL,
                                    GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                    "%s",
                                    NO_DATA_DIALOG);
    gtk_window_set_title(GTK_WINDOW(dialog), "Information!");
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }

  indexname = gtk_project_settings_get_indexname(GTK_PROJECT_SETTINGS(projset));
  projectfile =
       gtk_ltr_families_get_projectfile(GTK_LTR_FAMILIES(ltrgui->ltr_families));
  if (g_strcmp0(indexname, "") == 0) {
    dialog = gtk_message_dialog_new(GTK_WINDOW(ltrgui->main_window),
                                    GTK_DIALOG_MODAL ||
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
                                    "%s",
                                    NO_INDEX_DIALOG);
    gtk_window_set_title(GTK_WINDOW(dialog), "Information!");
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_NO) {
      gtk_widget_destroy(dialog);
      return;
    } else {
      gtk_widget_destroy(dialog);
      dialog = gtk_file_chooser_dialog_new("Select indexname...",
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
      if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        gchar *tmpname;
        tmpname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        gtk_widget_destroy(dialog);
        gtk_project_settings_update_indexname(GTK_PROJECT_SETTINGS(projset),
                                              tmpname);
        g_free(tmpname);
        indexname =
              gtk_project_settings_get_indexname(GTK_PROJECT_SETTINGS(projset));
      } else {
        gtk_widget_destroy(dialog);
        return;
      }
    }
  }

  dialog = gtk_file_chooser_dialog_new("Export FASTA...",
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
        attr = gt_feature_node_get_attribute(curnode, "ltrfam");
        if (attr)
          g_snprintf(header, BUFSIZ, "%s_%s_%lu_%lu", gt_str_get(seqid), attr,
                     range.start, range.end);
        else
          g_snprintf(header, BUFSIZ, "%s_%lu_%lu", gt_str_get(seqid),
                     range.start, range.end);
        sscanf(gt_str_get(seqid), "seq%lu", &seqnum);
        buffer = gt_calloc((size_t) gt_range_length(&range) + 1, sizeof(char));
        startpos = gt_encseq_seqstartpos(encseq, seqnum);
        gt_encseq_extract_decoded(encseq, buffer, startpos + range.start - 1,
                                  startpos + range.end - 1);
        gt_fasta_show_entry(header, buffer, gt_range_length(&range),
                            50, outfp);
        gt_free(buffer);
        gt_feature_node_iterator_delete(fni);
      }
      gt_file_delete(outfp);
    }

    if (had_err) {
      if (bakfile)
        g_rename(tmp_filename, filename);
      g_set_error(&ltrgui->err,
                  G_FILE_ERROR,
                  0,
                  "Error while exporting data: %s",
                  gt_error_get(err));
      error_handle(ltrgui);
    }
    gt_error_delete(err);
    g_free(filename);
  } else
    gtk_widget_destroy(dialog);
}

void mb_main_project_settings_activate(GT_UNUSED GtkMenuItem *menuitem,
                                       GUIData *ltrgui)
{
  gtk_widget_show(ltrgui->projset);
}

void mb_main_init(GUIData *ltrgui)
{
  GtkWidget *rc;
  GtkRecentFilter *rf;

  g_object_set_data(G_OBJECT(ltrgui->mb_main_file_new),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_NEW);
  g_object_set_data(G_OBJECT(ltrgui->mb_main_file_open),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_OPEN);
  g_object_set_data(G_OBJECT(ltrgui->mb_main_file_save),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_SAVE);
  g_object_set_data(G_OBJECT(ltrgui->mb_main_file_save_as),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_SAVE_AS);
  g_object_set_data(G_OBJECT(ltrgui->mb_main_file_import),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_IMPORT);
  g_object_set_data(G_OBJECT(ltrgui->mb_main_file_export_fasta),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_FASTA);
  g_object_set_data(G_OBJECT(ltrgui->mb_main_file_export_gff3),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_EGFF3);
  g_object_set_data(G_OBJECT(ltrgui->mb_main_file_close),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_CLOSE);
  g_object_set_data(G_OBJECT(ltrgui->mb_main_file_quit),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_QUIT);
  g_object_set_data(G_OBJECT(ltrgui->mb_main_project_settings),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_SETTINGS);
  g_object_set_data(G_OBJECT(ltrgui->mb_main_view_columns),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_COLUMNS);

  rc = gtk_recent_chooser_menu_new();
  g_signal_connect(G_OBJECT(rc), "item-activated",
                   (GCallback) mb_main_file_open_recent_activated,
                   ltrgui);
  rf = gtk_recent_filter_new();
  gtk_recent_filter_add_group(rf, GUI_RECENT_GROUP);
  gtk_recent_chooser_add_filter(GTK_RECENT_CHOOSER(rc), rf);
  gtk_recent_chooser_set_show_not_found(GTK_RECENT_CHOOSER(rc), FALSE);
  gtk_recent_chooser_set_local_only(GTK_RECENT_CHOOSER(rc), TRUE);
  gtk_recent_chooser_set_limit(GTK_RECENT_CHOOSER(rc), 5);
  gtk_recent_chooser_set_sort_type(GTK_RECENT_CHOOSER(rc), GTK_RECENT_SORT_MRU);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(ltrgui->mb_main_file_open_recent),
                            rc);
}
