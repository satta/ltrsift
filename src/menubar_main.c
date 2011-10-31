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
#include "statusbar_main.h"
#include "gtk_ltr_families.h"

void mb_main_activate_menuitems(GUIData *ltrgui)
{
  gtk_widget_set_sensitive(ltrgui->mb_main_view_columns, TRUE);
  gtk_widget_set_sensitive(ltrgui->mb_main_file_save, TRUE);
  gtk_widget_set_sensitive(ltrgui->mb_main_file_save_as, TRUE);
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

static void reorder_tabs(gpointer key, gpointer value, gpointer user_data)
{

  gtk_notebook_reorder_child((GtkNotebook*) user_data,
                             (GtkWidget*) key,
                             (gint) value);
}

static gint save_gui_settings(GUIData *ltrgui, const gchar *projectfile)
{
  sqlite3 *db;
  sqlite3_stmt *stmt;
  GtkWidget *sw,
            *label_close;
  GtkNotebook *notebook;
  GtkTreeView *list_view;
  GtkTreeViewColumn *column;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtArray *nodes;
  GList *children,
        *columns;
  gboolean valid;
  gchar *errmsg,
        *name,
        query[BUFSIZ];
  gint width,
       height,
       x, y, z, i, nop, ret;
  guint noc;

  ret = sqlite3_open(projectfile, &db);
  if (ret) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not save gui settings: %s",
                sqlite3_errmsg(db));
    sqlite3_close(db);
    return -1;
  }

  sqlite3_prepare_v2(db,
                     "CREATE TABLE IF NOT EXISTS gui_settings "
                     "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "height INTEGER NOT NULL, "
                      "width INTEGER NOT NULL, "
                      "ltrfam INTEGER NOT NULL, "
                      "hpaned INTEGER NOT NULL, "
                      "vpaned INTEGER NOT NULL)",
                     -1, &stmt, NULL);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_ERROR) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not save gui settings: %s",
                sqlite3_errmsg(db));
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return -1;
  }
  sqlite3_finalize(stmt);

  sqlite3_prepare_v2(db,
                     "CREATE TABLE IF NOT EXISTS project_params "
                     "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "psmall INTEGER NOT NULL, "
                      "plarge INTEGER NOT NULL, "
                      "wordsize INTEGER NOT NULL, "
                      "xdrop REAL NOT NULL, "
                      "seq_identity REAL NOT NULL)",
                     -1, &stmt, NULL);

  ret = sqlite3_step(stmt);
  if (ret == SQLITE_ERROR) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not save gui settings: %s",
                sqlite3_errmsg(db));
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return -1;
  }
  sqlite3_finalize(stmt);

  sqlite3_prepare_v2(db,
                     "CREATE TABLE IF NOT EXISTS empty_families "
                     "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                       "name TEXT)",
                     -1, &stmt, NULL);

  ret = sqlite3_step(stmt);
  if (ret == SQLITE_ERROR) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not save gui settings: %s",
                sqlite3_errmsg(db));
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return -1;
  }
  sqlite3_finalize(stmt);

  sqlite3_prepare_v2(db,
                     "CREATE TABLE IF NOT EXISTS invisible_columns "
                     "(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT)",
                     -1, &stmt, NULL);

  ret = sqlite3_step(stmt);
  if (ret == SQLITE_ERROR) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not save gui settings: %s",
                sqlite3_errmsg(db));
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return -1;
  }
  sqlite3_finalize(stmt);

  sqlite3_prepare_v2(db,
                     "CREATE TABLE IF NOT EXISTS notebook_tabs "
                     "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "name TEXT, "
                      "position INTEGER NOT NULL)",
                     -1, &stmt, NULL);

  ret = sqlite3_step(stmt);
  if (ret == SQLITE_ERROR) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not save gui settings: %s",
                sqlite3_errmsg(db));
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return -1;
  }
  sqlite3_finalize(stmt);

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
  ret = sqlite3_exec(db, query, NULL, NULL, &errmsg);
  if (ret) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not save gui settings: %s",
                errmsg);
    sqlite3_free(errmsg);
    sqlite3_close(db);
    return -1;
  }

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
      ret = sqlite3_exec(db, query, NULL, NULL, &errmsg);
      if (ret) {
        g_set_error(&ltrgui->err,
                    G_FILE_ERROR,
                    0,
                    "Could not save gui settings: %s",
                    errmsg);
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return -1;
      }
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
    ret = sqlite3_exec(db, query, NULL, NULL, &errmsg);
    if (ret) {
      g_set_error(&ltrgui->err,
                  G_FILE_ERROR,
                  0,
                  "Could not save gui settings: %s",
                  errmsg);
      sqlite3_free(errmsg);
      sqlite3_close(db);
      return -1;
    }
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
      ret = sqlite3_exec(db, query, NULL, NULL, &errmsg);
      if (ret) {
        g_set_error(&ltrgui->err,
                    G_FILE_ERROR,
                    0,
                    "Could not save gui settings: %s",
                    errmsg);
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return -1;
      }
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
        ret = sqlite3_exec(db, query, NULL, NULL, &errmsg);
        if (ret) {
          g_set_error(&ltrgui->err,
                      G_FILE_ERROR,
                      0,
                      "Could not save gui settings: %s",
                      errmsg);
          sqlite3_free(errmsg);
          sqlite3_close(db);
          return -1;
        }
      }
      g_free(name);
    }
  }
  sqlite3_close(db);

  return 0;
}

static gint apply_gui_settings(GUIData *ltrgui, const gchar *projectfile)
{
  sqlite3 *db;
  sqlite3_stmt *stmt;
  GtkWidget *sw,
            *label_close;
  GtkNotebook *nb;
  GtkTreeView *list_view;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreePath *path;
  GtArray *nodes;
  GHashTable *hash;
  gboolean valid;
  gchar *name,
        buffer[BUFSIZ];
  gint ret, nop, i;

  ret = sqlite3_open_v2(projectfile, &db, SQLITE_OPEN_READONLY, NULL);
  if (ret) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not apply gui settings: %s",
                sqlite3_errmsg(db));
    sqlite3_close(db);
    return -1;
  }

  sqlite3_prepare_v2(db,
                     "SELECT height, width, ltrfam, hpaned, "
                     "vpaned FROM gui_settings",
                     -1,
                     &stmt,
                     NULL);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_ERROR) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not apply gui settings: %s",
                sqlite3_errmsg(db));
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return -1;
  }

  if (ret == SQLITE_ROW) {
    gtk_window_resize(GTK_WINDOW(ltrgui->main_window),
                      sqlite3_column_int(stmt, 1),
                      sqlite3_column_int(stmt, 0));
    gtk_ltr_families_set_position(GTK_LTR_FAMILIES(ltrgui->ltr_families),
                                  sqlite3_column_int(stmt, 2));
    gtk_ltr_families_set_hpaned_position(GTK_LTR_FAMILIES(ltrgui->ltr_families),
                                         sqlite3_column_int(stmt, 3));
    gtk_ltr_families_set_vpaned_position(GTK_LTR_FAMILIES(ltrgui->ltr_families),
                                         sqlite3_column_int(stmt, 4));
  }
  sqlite3_finalize(stmt);

  sqlite3_prepare_v2(db, "SELECT name FROM empty_families", -1, &stmt, NULL);

  list_view =
           gtk_ltr_families_get_lv_fams(GTK_LTR_FAMILIES(ltrgui->ltr_families));
  model = gtk_tree_view_get_model(list_view);
  while ((ret = sqlite3_step(stmt)) == SQLITE_ROW) {
    nodes = gt_array_new(sizeof(GtGenomeNode*));
    g_snprintf(buffer, BUFSIZ, "%s (0)", sqlite3_column_text(stmt, 0));
    gtk_tree_store_append(GTK_TREE_STORE(model), &iter, NULL);
    gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
                       LTRFAMS_FAM_LV_NODE_ARRAY, nodes,
                       LTRFAMS_FAM_LV_TAB_CHILD, NULL,
                       LTRFAMS_FAM_LV_TAB_LABEL, NULL,
                       LTRFAMS_FAM_LV_OLDNAME, sqlite3_column_text(stmt, 0),
                       LTRFAMS_FAM_LV_CURNAME, buffer,
                       -1);
  }
  sqlite3_finalize(stmt);
  if (ret == SQLITE_ERROR) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not apply gui settings: %s",
                sqlite3_errmsg(db));
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return -1;
  }

  valid = gtk_tree_model_get_iter_first(model, &iter);
  if (valid) {
    gtk_tree_model_get(model, &iter,
                       LTRFAMS_FAM_LV_OLDNAME, &name,
                       -1);
    g_snprintf(buffer, BUFSIZ,
               "SELECT name FROM notebook_tabs WHERE name = \"%s\"",
               name);
    sqlite3_prepare_v2(db, buffer, -1, &stmt, NULL);
    ret = sqlite3_step(stmt);
    if (ret == SQLITE_ERROR) {
      g_set_error(&ltrgui->err,
                  G_FILE_ERROR,
                  0,
                  "Could not apply gui settings: %s",
                  sqlite3_errmsg(db));
      sqlite3_finalize(stmt);
      sqlite3_close(db);
      return -1;
    }
    if (sqlite3_column_text(stmt, 0)) {
      path = gtk_tree_model_get_path(model, &iter);
      gtk_ltr_families_lv_fams_row_activated(list_view, path, NULL,
                                        GTK_LTR_FAMILIES(ltrgui->ltr_families));
      gtk_tree_path_free(path);
    }
    sqlite3_finalize(stmt);
    while (gtk_tree_model_iter_next(model, &iter)) {
      gtk_tree_model_get(model, &iter,
                         LTRFAMS_FAM_LV_OLDNAME, &name,
                         -1);
      g_snprintf(buffer, BUFSIZ,
                 "SELECT name FROM notebook_tabs WHERE name = \"%s\"",
                 name);
      sqlite3_prepare_v2(db, buffer, -1, &stmt, NULL);
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_ERROR) {
        g_set_error(&ltrgui->err,
                    G_FILE_ERROR,
                    0,
                    "Could not apply gui settings: %s",
                    sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
      }
      if (ret == SQLITE_ROW) {
        path = gtk_tree_model_get_path(model, &iter);
        gtk_ltr_families_lv_fams_row_activated(list_view, path, NULL,
                                        GTK_LTR_FAMILIES(ltrgui->ltr_families));
        gtk_tree_path_free(path);
      }
      sqlite3_finalize(stmt);
    }
  }

  nb = gtk_ltr_families_get_nb(GTK_LTR_FAMILIES(ltrgui->ltr_families));
  nop = gtk_notebook_get_n_pages(nb);
  if (nop > 1) {
    hash = g_hash_table_new(NULL, NULL);
    for (i = 0; i < nop; i++) {
      sw = gtk_notebook_get_nth_page(nb, i);
      label_close = gtk_notebook_get_tab_label(nb, sw);
      g_snprintf(buffer, BUFSIZ,
                 "SELECT position FROM notebook_tabs WHERE name = \"%s\"",
                 gtk_label_close_get_text(GTKLABELCLOSE(label_close)));
      sqlite3_prepare_v2(db, buffer, -1, &stmt, NULL);
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_ERROR) {
        g_set_error(&ltrgui->err,
                    G_FILE_ERROR,
                    0,
                    "Could not apply gui settings: %s",
                    sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
      }
      if (ret == SQLITE_ROW) {
        g_hash_table_insert(hash, (gpointer) sw,
                            (gpointer) sqlite3_column_int(stmt, 0));
      }
      sqlite3_finalize(stmt);
    }
    g_hash_table_foreach(hash, (GHFunc) reorder_tabs, (gpointer) nb);
    g_hash_table_destroy(hash);
  }
  sqlite3_close(db);
  return 0;
}

static gint open_project_data(GUIData *ltrgui, gchar *filename)
{
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
  GtError *err;
  int had_err = 0;
  unsigned long i, n_features;

  err = gt_error_new();
  rdb = gt_rdb_sqlite_new(filename, err);
  if (!rdb)
    had_err = -1;
  if (!had_err) {
    adb = gt_anno_db_sqlite_new((GtRDBSqlite*) rdb, err);
    if (!adb)
      had_err = -1;
  }
  if (!had_err) {
    fi = gt_feature_index_anno_db_new(adb, err);
    had_err = fi ? 0 : -1;
  }
  if (!had_err) {
    nodes = gt_array_new(sizeof(GtFeatureNode*));
    seqids = gt_feature_index_get_seqids(fi, err);
    if (seqids) {
      for (i = 0; i < gt_str_array_size(seqids); i++) {
        tmp_nodes =
            gt_feature_index_get_features_for_seqid(fi,
                                                    gt_str_array_get(seqids, i),
                                                    err);
        gt_array_add_array(nodes, tmp_nodes);
        gt_array_delete(tmp_nodes);
        tmp_nodes = NULL;
      }
      gt_str_array_delete(seqids);
    }
  }
  gt_rdb_delete(rdb);
  gt_anno_db_delete(adb);

  if (!had_err) {
    features = gt_hashmap_new(GT_HASH_STRING, free_hash, NULL);
    n_features = LTRFAMS_LV_N_COLUMS;

    for (i = 0; i < gt_array_size(nodes); i++) {
      GtGenomeNode *gn;
      gn = gt_genome_node_ref(*(GtGenomeNode**) gt_array_get(nodes, i));
    }

    last_stream = array_in_stream = gt_array_in_stream_new(nodes, err);
    last_stream = preprocess_stream =
                                    gt_ltrgui_preprocess_stream_new(last_stream,
                                                                    features,
                                                                    &n_features,
                                                                    err);
    had_err = gt_node_stream_pull(last_stream, err);
    if (!had_err) {
      gtk_widget_destroy(ltrgui->ltr_families);
      ltrgui->ltr_families = gtk_ltr_families_new();
      gtk_box_pack_start(GTK_BOX(ltrgui->vbox1_main), ltrgui->ltr_families,
                         TRUE, TRUE, 0);
      gtk_ltr_families_fill_with_data(GTK_LTR_FAMILIES(ltrgui->ltr_families),
                                      nodes,
                                      features,
                                      n_features);
      gtk_ltr_families_set_projectfile(GTK_LTR_FAMILIES(ltrgui->ltr_families),
                                       filename);
      mb_main_view_columns_set_submenu(ltrgui, features, err, TRUE);
      mb_main_activate_menuitems(ltrgui);
      gtk_ltr_families_set_modified(GTK_LTR_FAMILIES(ltrgui->ltr_families),
                                    FALSE);
      create_recently_used_resource(filename);
    }
    gt_node_stream_delete(array_in_stream);
    gt_node_stream_delete(preprocess_stream);
    //gt_feature_index_delete(fi);
  }
  if (had_err) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Error while opening data: %s",
                gt_error_get(err));
  }
  gt_error_delete(err);

  return had_err;
}

static gint save_project_data(GUIData *ltrgui, const gchar *filename)
{
  GtArray *nodes;
  GtNodeStream *array_in_stream = NULL,
               *feature_stream = NULL;
  GtRDB *rdb = NULL;
  GtAnnoDB *adb = NULL;
  GtFeatureIndex *fi = NULL;
  GtError *err;
  unsigned long i;
  gint had_err = 0;

  err = gt_error_new();

  rdb = gt_rdb_sqlite_new(filename, err);
  if (!rdb)
    had_err = -1;
  if (!had_err) {
    adb = gt_anno_db_sqlite_new((GtRDBSqlite*) rdb, err);
    if (!adb)
      had_err = -1;
  }
  if (!had_err) {
    fi = gt_feature_index_anno_db_new(adb, err);
    if (!fi)
      had_err = -1;
  }
  if (!had_err) {
    nodes = gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrgui->ltr_families));
    for (i = 0; i < gt_array_size(nodes); i++) {
      GtGenomeNode *gn;
      gn = gt_genome_node_ref(*(GtGenomeNode**) gt_array_get(nodes, i));
      gn = gt_genome_node_ref(*(GtGenomeNode**) gt_array_get(nodes, i));
    }
    array_in_stream = gt_array_in_stream_new(nodes, err);

    gt_assert(array_in_stream);

    feature_stream = gt_feature_stream_new(array_in_stream, fi);
    had_err = gt_node_stream_pull(feature_stream, err);
  }
  if (had_err) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Error while saving data: %s",
                gt_error_get(err));
  }
  gt_node_stream_delete(feature_stream);
  gt_node_stream_delete(array_in_stream);
  gt_feature_index_delete(fi);
  gt_anno_db_delete(adb);
  gt_rdb_delete(rdb);

  return had_err;
}

void mb_main_file_save_activate(GT_UNUSED GtkMenuItem *menuitem,
                                GUIData *ltrgui)
{
  gchar tmp_filename[BUFSIZ];
  const gchar *projectfile;
  gint had_err = 0;

  projectfile =
       gtk_ltr_families_get_projectfile(GTK_LTR_FAMILIES(ltrgui->ltr_families));
  if (projectfile == NULL) {
    mb_main_file_save_as_activate(NULL, ltrgui);
    return;
  } else {
    g_snprintf(tmp_filename, BUFSIZ, "%s.bak", projectfile);
    g_rename(projectfile, tmp_filename);

    had_err = save_project_data(ltrgui, projectfile);
    if (!had_err) {
      gtk_ltr_families_set_modified(GTK_LTR_FAMILIES(ltrgui->ltr_families),
                                    FALSE);
      had_err = save_gui_settings(ltrgui, projectfile);
    } else
      g_rename(tmp_filename, projectfile);
    if (had_err)
      error_handle(ltrgui->err);
  }
}

void mb_main_file_save_as_activate(GT_UNUSED GtkMenuItem *menuitem,
                                      GUIData *ltrgui)
{
  GtkWidget *filechooser,
            *dialog;
  gchar *filename,
        tmp_filename[BUFSIZ];
  const gchar *projectfile;
  gint had_err = 0;
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

  had_err = save_project_data(ltrgui, filename);
  if (!had_err)
    had_err = save_gui_settings(ltrgui, filename);

  if (!had_err) {
    gtk_ltr_families_set_projectfile(GTK_LTR_FAMILIES(ltrgui->ltr_families),
                                     filename);
    gtk_ltr_families_set_modified(GTK_LTR_FAMILIES(ltrgui->ltr_families),
                                  FALSE);
    create_recently_used_resource(filename);
  } else {
    if (bakfile)
      g_rename(tmp_filename, filename);
    error_handle(ltrgui->err);
  }
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
      g_snprintf(buffer, BUFSIZ, FILE_EXISTS_DIALOG, filename);
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

    last_stream = array_in_stream = gt_array_in_stream_new(nodes, err);
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
      error_handle(ltrgui->err);
    }
    gt_node_stream_delete(array_in_stream);
    gt_node_stream_delete(gff3_out_stream);
    gt_error_delete(err);
  }
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
}

void mb_main_view_columns_set_submenu(GUIData *ltrgui, GtHashmap *features,
                                      GtError *err, gboolean sqlt)
{
  GtkWidget *menu,
            *menuitem;
  GtStrArray *captions;
  sqlite3 *db = NULL;
  sqlite3_stmt *stmt;
  gchar buffer[BUFSIZ];
  const gchar *projectfile;
  gint ret = 0;
  unsigned long i;

  captions = gt_str_array_new();
  menu = gtk_menu_new();

  gt_hashmap_foreach(features, get_keys, (void*) captions, err);

  if (sqlt) {
    projectfile =
       gtk_ltr_families_get_projectfile(GTK_LTR_FAMILIES(ltrgui->ltr_families));
    ret = sqlite3_open_v2(projectfile, &db, SQLITE_OPEN_READONLY, NULL);
  }
  if (ret) {
    g_set_error(&ltrgui->err,
                G_FILE_ERROR,
                0,
                "Could not apply gui settings: %s",
                sqlite3_errmsg(db));
    sqlite3_close(db);
    error_handle(ltrgui->err);
  }

  for (i = 0; i < gt_str_array_size(captions); i++) {
    menuitem = gtk_check_menu_item_new_with_label(gt_str_array_get(captions,
                                                                   i));
    if (sqlt && !ret) {
      g_signal_connect(G_OBJECT(menuitem), "toggled",
                       G_CALLBACK(mb_main_view_columns_toggled),
                       ltrgui->ltr_families);
      g_snprintf(buffer, BUFSIZ,
                 "SELECT name FROM invisible_columns WHERE name = \"%s\"",
                 double_underscores(gt_str_array_get(captions, i)));
      sqlite3_prepare_v2(db, buffer, -1, &stmt, NULL);
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_ERROR) {
        g_set_error(&ltrgui->err,
                    G_FILE_ERROR,
                    0,
                    "Could not apply gui settings: %s",
                    sqlite3_errmsg(db));
        error_handle(ltrgui->err);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), TRUE);
        ret = 1;
      } else if (ret == SQLITE_ROW) {
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), TRUE);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), FALSE);
        ret = 0;
      } else {
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), TRUE);
        ret = 0;
      }
      sqlite3_finalize(stmt);
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

   /* if (!sqlt) {
      gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), TRUE);
      g_signal_connect(G_OBJECT(menuitem), "toggled",
                       G_CALLBACK(mb_main_view_columns_toggled),
                       ltrgui->ltr_families);
    } else {
      g_signal_connect(G_OBJECT(menuitem), "toggled",
                       G_CALLBACK(mb_main_view_columns_toggled),
                       ltrgui->ltr_families);
      projectfile =
       gtk_ltr_families_get_projectfile(GTK_LTR_FAMILIES(ltrgui->ltr_families));
      if (ret) {
        g_set_error(&ltrgui->err,
                    G_FILE_ERROR,
                    0,
                    "Could not apply gui settings: %s",
                    sqlite3_errmsg(db));
        sqlite3_close(db);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), TRUE);
        error_handle(ltrgui->err);
      } else {
        g_snprintf(buffer, BUFSIZ,
                   "SELECT name FROM invisible_columns WHERE name = \"%s\"",
                   double_underscores(gt_str_array_get(captions, i)));
        sqlite3_prepare_v2(db, buffer, -1, &stmt, NULL);
        ret = sqlite3_step(stmt);
        if (ret == SQLITE_ERROR) {
          g_set_error(&ltrgui->err,
                      G_FILE_ERROR,
                      0,
                      "Could not apply gui settings: %s",
                      sqlite3_errmsg(db));
          error_handle(ltrgui->err);
        } else if (ret == SQLITE_ROW) {
          gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), TRUE);
          gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), FALSE);
        } else
          gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), TRUE);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
      }
    }
    if (g_strcmp0(gt_str_array_get(captions, i), FNT_LLTR) == 0)
      gtk_menu_shell_insert(GTK_MENU_SHELL(menu), menuitem, 0);
    else if (g_strcmp0(gt_str_array_get(captions, i), FNT_RLTR) == 0)
      gtk_menu_shell_insert(GTK_MENU_SHELL(menu), menuitem, 1);
    else
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);*/
  }
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(ltrgui->mb_main_view_columns), menu);
  gtk_widget_show_all(menu);
  gtk_widget_set_sensitive(ltrgui->mb_main_view_columns, TRUE);
  if (db)
    sqlite3_close(db);
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
    error_handle(ltrgui->err);
  }
  gt_node_stream_delete(gff3_in_stream);
  gt_node_stream_delete(preprocess_stream);
  gt_node_stream_delete(array_out_stream);
  gt_error_delete(err);
}

void mb_main_file_open_activate(GT_UNUSED GtkMenuItem *menuitem,
                                GUIData *ltrgui)
{
  GtkWidget *filechooser,
            *dialog;
  gchar *filename = NULL;
  gint response = GTK_RESPONSE_REJECT,
       had_err = 0;

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
  had_err = open_project_data(ltrgui, filename);
  if (!had_err)
    had_err = apply_gui_settings(ltrgui, filename);
  if (had_err)
    error_handle(ltrgui->err);
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
  gtk_widget_show(ltrgui->project_wizard);
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
  gchar *filename;

  GtkWidget *dialog;
  gint response = GTK_RESPONSE_REJECT,
       had_err = 0;

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
  had_err = open_project_data(ltrgui, filename);
  if (!had_err)
    had_err = apply_gui_settings(ltrgui, filename);
  if (had_err)
    error_handle(ltrgui->err);
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
  g_object_set_data(G_OBJECT(ltrgui->mb_main_file_export_gff3),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_EGFF3);
  g_object_set_data(G_OBJECT(ltrgui->mb_main_file_close),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_CLOSE);
  g_object_set_data(G_OBJECT(ltrgui->mb_main_file_quit),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_QUIT);

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
