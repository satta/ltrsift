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
#include "ltrsift.h"
#include "menubar.h"
#include "message_strings.h"
#include "statusbar.h"
#include "support.h"

static void free_gui(GUIData *ltrgui)
{
  gt_error_delete(ltrgui->err);
  g_slice_free(GUIData, ltrgui);
}

static void reorder_tabs(gpointer key, gpointer value, gpointer user_data)
{

  gtk_notebook_reorder_child((GtkNotebook*) user_data,
                             (GtkWidget*) key,
                             GPOINTER_TO_INT(value));
}

gint save_gui_settings(GUIData *ltrgui)
{
  GtRDB *rdb;
  GtRDBStmt *stmt;
  GtError *err;
  gint had_err = 0;

  err = gt_error_new();
  rdb = gtk_ltr_families_get_rdb(GTK_LTR_FAMILIES(ltrgui->ltrfams));
  if (!rdb) {
    gt_error_set(ltrgui->err,
                "Could not save gui settings: %s",
                gt_error_get(err));
    return -1;
  }
  stmt = gt_rdb_prepare(rdb,
                           "CREATE TABLE IF NOT EXISTS invisible_columns "
                           "(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT)",
                           0, err);
  if (!stmt || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    gt_error_set(ltrgui->err,
                "Could not save gui settings: %s",
                gt_error_get(err));
    gt_error_delete(err);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);

  stmt = gt_rdb_prepare(rdb,
                           "CREATE TABLE IF NOT EXISTS filter_files "
                           "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "filename TEXT)",
                           0, err);
  if (!stmt || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    return -1;
  }
  gt_rdb_stmt_delete(stmt);

  stmt = gt_rdb_prepare(rdb,
                           "CREATE TABLE IF NOT EXISTS notebook_tabs "
                           "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "name TEXT, "
                            "position INTEGER NOT NULL)",
                           0, err);
  if (!stmt || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    gt_error_set(ltrgui->err,
                "Could not save gui settings: %s",
                gt_error_get(err));
    gt_error_delete(err);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);

   stmt = gt_rdb_prepare(rdb,
                         "CREATE UNIQUE INDEX IF NOT EXISTS "
                         "nbtabs ON notebook_tabs (name)",
                           0, err);
  if (!stmt || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    gt_error_set(ltrgui->err,
                "Could not save gui settings: %s",
                gt_error_get(err));
    gt_error_delete(err);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);
  gt_error_delete(err);

  return 0;
}

gint apply_gui_settings(GUIData *ltrgui)
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
  GHashTable *hash;
  gboolean valid;
  gchar *name,
        buffer[BUFSIZ];
  gint had_err = 0, nop, i;
  GtError *err;
  err = gt_error_new();
  rdb = gtk_ltr_families_get_rdb(GTK_LTR_FAMILIES(ltrgui->ltrfams));
  list_view = gtk_ltr_families_get_list_view_families(
                                           GTK_LTR_FAMILIES(ltrgui->ltrfams));
  model = gtk_tree_view_get_model(list_view);
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
    stmt = gt_rdb_prepare(rdb, buffer, -1, err);
    if (!stmt || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
      gt_error_set(ltrgui->err,
                  "Could not apply gui settings: %s",
                  gt_error_get(err));
      gt_error_delete(err);
      return -1;
    }
    if (!had_err) {
      gtk_tree_model_get(model, &iter,
                         LTRFAMS_FAM_LV_NODE_ARRAY, &tmp_nodes,
                         -1);
      path = gtk_tree_model_get_path(model, &iter);
      gtk_ltr_families_notebook_add_tab(model, &iter, tmp_nodes, TRUE,
                                        GTK_LTR_FAMILIES(ltrgui->ltrfams));
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
      stmt = gt_rdb_prepare(rdb, buffer, -1, err);
      if (!stmt || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
        gt_error_set(ltrgui->err,
                    "Could not apply gui settings: %s",
                    gt_error_get(err));
        gt_error_delete(err);
        return -1;
      }
      if (!had_err) {
        path = gtk_tree_model_get_path(model, &iter);
        gtk_tree_model_get(model, &iter,
                           LTRFAMS_FAM_LV_NODE_ARRAY, &tmp_nodes,
                           -1);
        gtk_ltr_families_notebook_add_tab(model, &iter, tmp_nodes, TRUE,
                                          GTK_LTR_FAMILIES(ltrgui->ltrfams));
        gtk_tree_path_free(path);
      }
      gt_rdb_stmt_delete(stmt);
    }
  }

  had_err = 0;
  nb = gtk_ltr_families_get_notebook(GTK_LTR_FAMILIES(ltrgui->ltrfams));
  nop = gtk_notebook_get_n_pages(nb);
  if (nop > 1) {
    gint tab_pos;
    hash = g_hash_table_new(NULL, NULL);
    for (i = 0; i < nop; i++) {
      sw = gtk_notebook_get_nth_page(nb, i);
      label_close = gtk_notebook_get_tab_label(nb, sw);
      g_snprintf(buffer, BUFSIZ,
                 "SELECT position FROM notebook_tabs WHERE name = \"%s\"",
                 gtk_label_close_get_text(GTK_LABEL_CLOSE(label_close)));
      stmt = gt_rdb_prepare(rdb, buffer, -1, err);
      if (!stmt || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
        gt_error_set(ltrgui->err,
                    "Could not apply gui settings: %s",
                    gt_error_get(err));
        gt_error_delete(err);
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
    gt_error_set(ltrgui->err,
                "Could not apply gui settings: %s",
                gt_error_get(err));
    gt_error_delete(err);
    return -1;
  }
  gt_error_delete(err);
  return 0;
}

static gboolean main_window_delete_event(GT_UNUSED GtkWidget *widget,
                                         GT_UNUSED GdkEvent *event,
                                         GUIData *ltrgui)
{
  GtkWidget *dialog;
  gint response = GTK_RESPONSE_REJECT;

  if (gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrgui->ltrfams))) {
    if (!gtk_ltr_families_get_projectfile(
                                      GTK_LTR_FAMILIES(ltrgui->ltrfams))) {
      dialog = unsaved_changes_dialog(ltrgui, NO_PROJECT_DIALOG);
      response = gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
    } else if (gtk_ltr_families_get_modified(GTK_LTR_FAMILIES(
                                                       ltrgui->ltrfams))) {
      dialog = unsaved_changes_dialog(ltrgui, UNSAVED_CHANGES_DIALOG);
      response = gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
    }
  }

  switch (response) {
    case GTK_RESPONSE_CANCEL:
      return TRUE;
      break;
    case GTK_RESPONSE_ACCEPT:
      menubar_save_activate(NULL, ltrgui);
      return TRUE;
      break;
    case GTK_RESPONSE_REJECT:
      break;
    default:
      break;
  }
  return FALSE;
}

static void init_gui(GUIData *ltrgui)
{

  ltrgui->main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(ltrgui->main_window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(ltrgui->main_window), GUI_NAME);
  gtk_window_set_default_size(GTK_WINDOW(ltrgui->main_window), 1024, 768);
  g_signal_connect(G_OBJECT(ltrgui->main_window), "delete-event",
                   G_CALLBACK(main_window_delete_event), ltrgui);
  g_signal_connect(G_OBJECT(ltrgui->main_window), "destroy",
                   G_CALLBACK(gtk_main_quit), NULL);
  ltrgui->vbox = gtk_vbox_new(FALSE, 1);
  gtk_container_add(GTK_CONTAINER(ltrgui->main_window), ltrgui->vbox);

  menubar_init(ltrgui);
  statusbar_init(ltrgui);
  gtk_box_pack_start(GTK_BOX(ltrgui->vbox), ltrgui->menubar, FALSE, FALSE, 0);
  ltrgui->err = gt_error_new();
  ltrgui->projset = gtk_project_settings_new(NULL);
  ltrgui->ltrfams = gtk_ltr_families_new(ltrgui->statusbar,
                                         ltrgui->progressbar,
                                         ltrgui->projset,
                                         ltrgui->style_file,
                                         ltrgui->err);
  gtk_box_pack_start(GTK_BOX(ltrgui->vbox), ltrgui->ltrfams, TRUE, TRUE, 0);
  ltrgui->ltrfilt = gtk_ltr_filter_new(ltrgui->ltrfams);
  gtk_ltr_families_set_filter_widget(GTK_LTR_FAMILIES(ltrgui->ltrfams),
                                     ltrgui->ltrfilt);
  ltrgui->assistant = NULL;
  ltrgui->refseq_paramsets = NULL;
  gtk_widget_show_all(ltrgui->main_window);
  gtk_widget_hide(ltrgui->progressbar);
}

gint main(gint argc, gchar *argv[])
{
  GUIData *ltrgui;

  /* allocate the memory needed by GUIData */
  ltrgui = g_slice_new(GUIData);
  /* initialize libraries */
  if (!g_thread_supported())
    g_thread_init(NULL);
  gdk_threads_init();
  gtk_init(&argc, &argv);
  gt_lib_init();

  if (argc == 2) {
    ltrgui->style_file = argv[1];
  } else {
    ltrgui->style_file = getenv(LTRSIFT_STYLE_ENV);
    if (!ltrgui->style_file)
      ltrgui->style_file = DEFAULT_STYLE;
  }
  init_gui(ltrgui);

  /* show the window */
  gtk_widget_show(ltrgui->main_window);

  /* enter GTK+ main loop */
  gdk_threads_enter();
  gtk_main();
  gdk_threads_leave();
  /* free memory allocated for GUIData struct */
  free_gui(ltrgui);
  gt_lib_clean();
  return EXIT_SUCCESS;
}
