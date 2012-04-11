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
#include "gtk_ltr_filter.h"
#include "message_strings.h"
#include "support.h"

void gtk_ltr_filter_set_range(GtkLTRFilter *ltrfilt, gint range)
{
  ltrfilt->range = range;
}

void gtk_ltr_filter_set_apply_text(GtkLTRFilter *ltrfilt, const gchar *text)
{
  gtk_button_set_label(GTK_BUTTON(ltrfilt->apply), text);
}

static void remove_file_from_sqlite(GtkTreeRowReference *row_ref,
                                    GtkLTRFilter *ltrfilt)
{
  GtkTreeModel *model;
  GtkTreePath *path;
  GtkTreeIter iter;
  GtRDB *rdb;
  GtRDBStmt *stmt;
  GtError *err = gt_error_new();
  gchar *file, *pfile, query[BUFSIZ];
  gint had_err = 0;

  pfile = gtk_ltr_families_get_projectfile(GTK_LTR_FAMILIES(ltrfilt->ltrfams));
  if (!pfile)
    return;

  model = gtk_tree_row_reference_get_model(row_ref);
  path = gtk_tree_row_reference_get_path(row_ref);
  gtk_tree_model_get_iter(model, &iter, path);
  gtk_tree_model_get(model, &iter, LTR_FILTER_LV_FILE, &file, -1);

  rdb = gtk_ltr_families_get_rdb(GTK_LTR_FAMILIES(ltrfilt->ltrfams));
  if (!rdb) {
    error_handle(GTK_WIDGET(ltrfilt), err);
    gt_error_delete(err);
    gtk_tree_path_free(path);
    g_free(file);
    return;
  }
  g_snprintf(query, BUFSIZ,
             "DELETE FROM filter_files WHERE filename = \"%s\"", file);
  had_err = gt_rdb_prepare(rdb, query, -1, &stmt, err);
  if (had_err) {
    error_handle(GTK_WIDGET(ltrfilt), err);
    gt_error_delete(err);
    gtk_tree_path_free(path);
    g_free(file);
    return;
  }
  if ((had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    error_handle(GTK_WIDGET(ltrfilt), err);
    gt_rdb_stmt_delete(stmt);
    gt_error_delete(err);
    gtk_tree_path_free(path);
    g_free(file);
    return;
  }
  gt_rdb_stmt_delete(stmt);
  gt_error_delete(err);
  gtk_tree_path_free(path);
  g_free(file);
}

/*gint gtk_ltr_filter_save_data(GtkLTRFilter *ltrfilt, GtError *err)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtRDB *rdb;
  GtRDBStmt *stmt;
  gboolean valid;
  gchar *pfile, query[BUFSIZ], *file;
  gint had_err = 0;

  pfile = gtk_ltr_families_get_projectfile(GTK_LTR_FAMILIES(ltrfilt->ltrfams));
  if (!pfile)
    return had_err;
  rdb = gt_rdb_sqlite_new(pfile, err);
  if (!rdb)
    return -1;
  had_err = gt_rdb_prepare(rdb,
                           "CREATE TABLE IF NOT EXISTS filter_files "
                           "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "filename TEXT)",
                           -1, &stmt, err);
  if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    gt_rdb_delete(rdb);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);

  had_err = gt_rdb_prepare(rdb,
                           "DELETE FROM filter_files",
                           -1, &stmt, err);

  if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    gt_rdb_delete(rdb);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfilt->list_view_all));
  valid = gtk_tree_model_get_iter_first(model, &iter);
  if (!valid)
    return 0;

  gtk_tree_model_get(model, &iter, LTR_FILTER_LV_FILE, &file, -1);
  g_snprintf(query, BUFSIZ,
             "INSERT INTO filter_files (filename) values (\"%s\")", file);
  had_err = gt_rdb_prepare(rdb, query, -1, &stmt, err);
  if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    g_free(file);
    gt_rdb_delete(rdb);
    return -1;
  }
  g_free(file);
  gt_rdb_stmt_delete(stmt);

  while (gtk_tree_model_iter_next(model, &iter)) {
    gtk_tree_model_get(model, &iter, LTR_FILTER_LV_FILE, &file, -1);
    g_snprintf(query, BUFSIZ,
               "INSERT INTO filter_files (filename) values (\"%s\")", file);
    had_err = gt_rdb_prepare(rdb, query, -1, &stmt, err);
    if (had_err || (had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
      g_free(file);
      gt_rdb_delete(rdb);
      return -1;
    }
    g_free(file);
    gt_rdb_stmt_delete(stmt);
  }
  gt_rdb_delete(rdb);

  return 0;
}*/

static void add_file_to_sqlite(GtkLTRFilter *ltrfilt, gchar *file)
{
  GtRDB *rdb;
  GtRDBStmt *stmt;
  GtError *err = gt_error_new();
  gchar *pfile, query[BUFSIZ];
  gint had_err = 0;

  pfile = gtk_ltr_families_get_projectfile(GTK_LTR_FAMILIES(ltrfilt->ltrfams));
  if (!pfile)
    return;
  rdb = gtk_ltr_families_get_rdb(GTK_LTR_FAMILIES(ltrfilt->ltrfams));
  if (!rdb) {
    error_handle(GTK_WIDGET(ltrfilt), err);
    gt_error_delete(err);
    return;
  }
  had_err = gt_rdb_prepare(rdb,
                           "CREATE TABLE IF NOT EXISTS filter_files "
                           "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "filename TEXT)",
                           -1, &stmt, err);
  if (had_err) {
    error_handle(GTK_WIDGET(ltrfilt), err);
    gt_error_delete(err);
    return;
  }
  if ((had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    error_handle(GTK_WIDGET(ltrfilt), err);
    gt_rdb_stmt_delete(stmt);
    gt_error_delete(err);
    return;
  }
  gt_rdb_stmt_delete(stmt);
  g_snprintf(query, BUFSIZ,
             "INSERT INTO filter_files (filename) values (\"%s\")", file);
  had_err = gt_rdb_prepare(rdb, query, -1, &stmt, err);
  if (had_err) {
    error_handle(GTK_WIDGET(ltrfilt), err);
    gt_error_delete(err);
    return;
  }
  if ((had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    error_handle(GTK_WIDGET(ltrfilt), err);
    gt_rdb_stmt_delete(stmt);
    gt_error_delete(err);
    return;
  }
  gt_rdb_stmt_delete(stmt);
  gt_error_delete(err);
}

gint gtk_ltr_filter_get_filter_files_from_sql(GtkLTRFilter *ltrfilt,
                                              GtError *err)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtScriptFilter *script_filter;
  GtRDB *rdb;
  GtRDBStmt *stmt;
  GtStr *result;
  gboolean valid;
  gint had_err = 0;
  gchar *pfile;

  pfile = gtk_ltr_families_get_projectfile(GTK_LTR_FAMILIES(ltrfilt->ltrfams));
  if (pfile == NULL)
    return 0;
  rdb = gtk_ltr_families_get_rdb(GTK_LTR_FAMILIES(ltrfilt->ltrfams));
  had_err = gt_rdb_prepare(rdb,
                           "SELECT filename FROM filter_files",
                           -1, &stmt, err);
  if (had_err)
    return -1;
  while ((had_err = gt_rdb_stmt_exec(stmt, err)) == 0) {
    result = gt_str_new();
    had_err = gt_rdb_stmt_get_string(stmt, 0, result, err);
    if (!had_err) {
      if (g_file_test(gt_str_get(result), G_FILE_TEST_EXISTS)) {
        model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfilt->list_view_all));
        script_filter = gt_script_filter_new(gt_str_get(result), err);
        if (script_filter != NULL) {
          valid = gt_script_filter_validate(script_filter, err);
          if (valid) {
            gtk_list_store_append(GTK_LIST_STORE(model), &iter);
            gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                               LTR_FILTER_LV_FILE, gt_str_get(result), -1);
          }
        }
        gt_script_filter_delete(script_filter);
        gt_error_unset(err);
      }
    } else {
      gt_rdb_stmt_delete(stmt);
      gt_str_delete(result);
      return -1;
    }
    gt_str_delete(result);
  }
  gt_rdb_stmt_delete(stmt);

  return 0;
}

static void delete_from_family(CandidateData *cdata, GtGenomeNode *gn,
                               GtkNotebook *notebook)
{
  GtkTreePath *path;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkWidget *tab_label;
  GtArray *tmp_nodes;
  gboolean valid;
  gchar *old_name, cur_name[BUFSIZ];

  path = gtk_tree_row_reference_get_path(cdata->fam_ref);
  model = gtk_tree_row_reference_get_model(cdata->fam_ref);
  valid = gtk_tree_model_get_iter(model, &iter, path);
  if (!valid)
    g_warning("%s", "Programming error!");
  else {
    gtk_tree_model_get(model, &iter,
                       LTRFAMS_FAM_LV_NODE_ARRAY, &tmp_nodes,
                       LTRFAMS_FAM_LV_OLDNAME, &old_name,
                       LTRFAMS_FAM_LV_TAB_LABEL, &tab_label,
                       -1);
    remove_node_from_array(tmp_nodes, gn);
    if (gt_array_size(tmp_nodes) == 0) {
      if (tab_label) {
        gtk_notebook_remove_page(notebook,
                                GPOINTER_TO_INT(gtk_label_close_get_button_data(
                                                  GTK_LABEL_CLOSE(tab_label),
                                                  "nbpage")));
      }
      remove_row(cdata->fam_ref);
      gt_array_delete(tmp_nodes);
    } else {
      g_snprintf(cur_name, BUFSIZ, "%s (%lu)", old_name,
                 gt_array_size(tmp_nodes));
      gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                         LTRFAMS_FAM_LV_CURNAME, cur_name,
                         -1);
    }
    g_free(old_name);
  }
  gtk_tree_path_free(path);

  if (cdata->cand_ref) {
    remove_row(cdata->cand_ref);
    cdata->cand_ref = NULL;
  }
}

void gtk_ltr_filter_set_ltrfams(GtkLTRFilter *ltrfilt, GtkWidget *ltrfams)
{
  ltrfilt->ltrfams = ltrfams;
}

static void edit_dialog_delete_event(GtkWidget *widget,
                                     GT_UNUSED GdkEvent *event,
                                     GtkLTRFilter *ltrfilt)
{
  gtk_widget_destroy(widget);
  ltrfilt->edit_dialog = NULL;
}

static void edit_dialog_close_clicked(GT_UNUSED GtkWidget *button,
                                      GtkLTRFilter *ltrfilt)
{
  GtkWidget *dialog;

  if (ltrfilt->cur_filename)
    g_free(ltrfilt->cur_filename);
  ltrfilt->cur_filename = NULL;
  if (gtk_text_buffer_get_modified(ltrfilt->text_buffer)) {
    dialog = gtk_message_dialog_new(GTK_WINDOW(ltrfilt),
                                    GTK_DIALOG_MODAL |
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_INFO, GTK_BUTTONS_YES_NO,
                                    "%s",
                                    LTR_FILTER_UNSAVED_CHANGES);
    gtk_window_set_title(GTK_WINDOW(dialog), "Attention!");

    if (gtk_dialog_run(GTK_DIALOG(dialog)) != GTK_RESPONSE_YES) {
      gtk_widget_destroy(dialog);
      return;
    } else
      gtk_widget_destroy(dialog);
  }
  gtk_widget_destroy(ltrfilt->edit_dialog);
  ltrfilt->edit_dialog = NULL;
}

static gboolean save_filter_file(GtkLTRFilter *ltrfilt)
{
  GtkWidget *dialog;
  GtkTextIter start, end;
  GtScriptFilter *script_filter = NULL;
  GtError *err = gt_error_new();
  gboolean valid, result = FALSE;
  gchar *text;

  gtk_text_buffer_get_bounds(ltrfilt->text_buffer, &start, &end);
  text = gtk_text_buffer_get_text(ltrfilt->text_buffer, &start, &end, FALSE);
  script_filter = gt_script_filter_new_from_string(text, err);
  if (script_filter != NULL) {
    valid = gt_script_filter_validate(script_filter, err);
    if (valid) {
      result = g_file_set_contents(ltrfilt->cur_filename, text, -1, NULL);
      if (!result) {
        gt_error_set(err, "Could not save content to %s",
                     ltrfilt->cur_filename);
        error_handle(GTK_WIDGET(ltrfilt), err);
      } else
        gtk_text_buffer_set_modified(ltrfilt->text_buffer, FALSE);
    } else {
      dialog = gtk_message_dialog_new(GTK_WINDOW(ltrfilt),
                                      GTK_DIALOG_MODAL |
                                      GTK_DIALOG_DESTROY_WITH_PARENT,
                                      GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                      "%s",
                                      LTR_FILTER_NOT_SAVED_FILE);
      gtk_window_set_title(GTK_WINDOW(dialog), "Attention!");
      gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
    }
  } else {
    dialog = gtk_message_dialog_new(GTK_WINDOW(ltrfilt),
                                    GTK_DIALOG_MODAL |
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                    "%s",
                                    LTR_FILTER_NOT_SAVED_FILE);
    gtk_window_set_title(GTK_WINDOW(dialog), "Attention!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  }
  gt_script_filter_delete(script_filter);

  return result;
}

static void edit_dialog_save_as_clicked(GT_UNUSED GtkWidget *button,
                                        GtkLTRFilter *ltrfilt)
{
  GtkWidget *fc;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkFileFilter *lua_file_filter;
  gboolean saved;
  gchar *filename;

  fc = gtk_file_chooser_dialog_new(GUI_DIALOG_SAVE_AS,
                                   GTK_WINDOW(ltrfilt->edit_dialog),
                                   GTK_FILE_CHOOSER_ACTION_SAVE,
                                   GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                   GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(fc), TRUE);
  lua_file_filter = gtk_file_filter_new();
  gtk_file_filter_set_name(lua_file_filter, LUA_FILTER_PATTERN);
  gtk_file_filter_add_pattern(lua_file_filter, LUA_FILTER_PATTERN);
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(fc), lua_file_filter);
  if (ltrfilt->cur_filename) {
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(fc), ltrfilt->cur_filename);
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(fc),
                                    g_path_get_basename(ltrfilt->cur_filename));
  } else if (ltrfilt->last_dir)
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(fc),
                                        ltrfilt->last_dir);
  else
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(fc), g_get_home_dir());

  if (gtk_dialog_run(GTK_DIALOG(fc)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fc));
    if (!g_str_has_suffix(filename, LUA_PATTERN)) {
      ltrfilt->cur_filename = g_strconcat(filename, LUA_PATTERN, NULL);
      g_free(filename);
    } else
      ltrfilt->cur_filename = filename;
  } else {
    gtk_widget_destroy(fc);
    return;
  }
  gtk_widget_destroy(fc);
  saved = save_filter_file(ltrfilt);
  if (saved) {
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfilt->list_view_all));
    if (!entry_in_list_view(model, ltrfilt->cur_filename, LTR_FILTER_LV_FILE)) {
      gtk_list_store_append(GTK_LIST_STORE(model), &iter);
      gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                         LTR_FILTER_LV_FILE, ltrfilt->cur_filename, -1);
    }
  }
}

static void edit_dialog_save_clicked(GT_UNUSED GtkWidget *button,
                                     GtkLTRFilter *ltrfilt)
{
  if (ltrfilt->cur_filename == NULL)
    edit_dialog_save_as_clicked(NULL, ltrfilt);
  else {
    GT_UNUSED gboolean saved;
    saved = save_filter_file(ltrfilt);
  }
}

static void create_edit_dialog(GtkLTRFilter *ltrfilt)
{
  GtkWidget *sw, *tv, *vbox, *hbox, *button;
  PangoFontDescription *font_desc;

  font_desc = pango_font_description_from_string("Courier");

  ltrfilt->edit_dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(ltrfilt->edit_dialog),
                       "Edit/New filter script");
  gtk_container_set_border_width(GTK_CONTAINER(ltrfilt->edit_dialog), 5);
  tv = gtk_text_view_new();
  gtk_widget_modify_font(tv, font_desc);
  ltrfilt->text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv));

  if (ltrfilt->cur_filename == NULL) {
    gtk_text_buffer_set_text(ltrfilt->text_buffer, LTR_FILTER_TEMPLATE, -1);
    gtk_text_buffer_set_modified(ltrfilt->text_buffer, TRUE);
  } else {
    gboolean result;
    gchar *text;
    result = g_file_get_contents(ltrfilt->cur_filename, &text, NULL, NULL);
    if (result) {
      gtk_text_buffer_set_text(ltrfilt->text_buffer, text, -1);
      gtk_text_buffer_set_modified(ltrfilt->text_buffer, FALSE);
      g_free(text);
    }
  }
  sw = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(sw), tv);
  vbox = gtk_vbox_new(FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), sw, TRUE, TRUE, 1);
  hbox = gtk_hbox_new(FALSE, 1);
  button = gtk_button_new_from_stock(GTK_STOCK_SAVE);
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(edit_dialog_save_clicked),
                   ltrfilt);
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 1);
  button = gtk_button_new_from_stock(GTK_STOCK_SAVE_AS);
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(edit_dialog_save_as_clicked),
                   ltrfilt);
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 1);
  button = gtk_button_new_from_stock(GTK_STOCK_CLOSE);
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(edit_dialog_close_clicked),
                   ltrfilt);
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 1);

  gtk_container_add(GTK_CONTAINER(ltrfilt->edit_dialog), vbox);
  gtk_window_set_transient_for(GTK_WINDOW(ltrfilt->edit_dialog),
                               GTK_WINDOW(ltrfilt));
  gtk_window_set_position(GTK_WINDOW(ltrfilt->edit_dialog),
                          GTK_WIN_POS_CENTER_ON_PARENT);
  gtk_window_set_modal(GTK_WINDOW(ltrfilt->edit_dialog), TRUE);
  gtk_window_resize(GTK_WINDOW(ltrfilt->edit_dialog), 512, 384);
  g_signal_connect(G_OBJECT(ltrfilt->edit_dialog), "delete_event",
                   G_CALLBACK(edit_dialog_delete_event), ltrfilt);
  gtk_widget_show_all(ltrfilt->edit_dialog);
  pango_font_description_free(font_desc);
}

static void apply_clicked(GT_UNUSED GtkButton *button, GtkLTRFilter *ltrfilt)
{
  CandidateData *cdata;
  GtkNotebook *notebook;
  GtkTreeView *list_view;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreeSelection *sel;
  GtkWidget *dialog,
            *tab_child;
  GtStrArray *filter_files = NULL;
  GtError *err = gt_error_new();
  GtFeatureNodeIterator *fni;
  GtFeatureNode *curnode;
  GtNodeStream *array_in_stream = NULL,
               *script_filter_stream = NULL,
               *array_out_stream = NULL;
  GtArray *nodes = NULL,
          *filtered_nodes = NULL,
          *tmp_nodes = NULL;
  GtBittab *negate;
  GtGenomeNode *gn;
  GList *rows, *tmp, *children;
  gboolean negate_filter;
  gchar *filter_file,
        fam_name[BUFSIZ],
        filter_message[BUFSIZ];
  gint action, logic, had_err = 0, tab_no;
  const char *attr;
  unsigned long total_candidates = 0,
                unclassified_candidates = 0,
                deleted_candidates = 0,
                i = 0;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfilt->list_view_sel));
  filtered_nodes = gt_array_new(sizeof (GtGenomeNode*));

  if (!gtk_tree_model_get_iter_first(model, &iter)) {
    return;
  } else {
    gtk_widget_hide(GTK_WIDGET(ltrfilt));
    negate =
     gt_bittab_new((unsigned long) gtk_tree_model_iter_n_children(model, NULL));
    filter_files = gt_str_array_new();
    gtk_tree_model_get(model, &iter,
                       LTR_FILTER_LV_FILE, &filter_file,
                       LTR_FILTER_LV_SEL_NOT, &negate_filter, -1);
    if (negate_filter)
      gt_bittab_set_bit(negate, i);
    i++;
    gt_str_array_add_cstr(filter_files, filter_file);
    g_free(filter_file);
    while (gtk_tree_model_iter_next(model, &iter)) {
      gtk_tree_model_get(model, &iter,
                         LTR_FILTER_LV_FILE, &filter_file,
                         LTR_FILTER_LV_SEL_NOT, &negate_filter, -1);
      if (negate_filter)
        gt_bittab_set_bit(negate, i);
      i++;
      gt_str_array_add_cstr(filter_files, filter_file);
      g_free(filter_file);
    }
  }
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ltrfilt->filter_logic)))
    logic = LTR_FILTER_LOGIC_AND;
  else
    logic = LTR_FILTER_LOGIC_OR;

  switch (ltrfilt->range) {
    case LTR_FILTER_RANGE_PROJECT:
      nodes = gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrfilt->ltrfams));
      total_candidates = gt_array_size(nodes);
      break;
    case LTR_FILTER_RANGE_FAMILIES:
      nodes = gt_array_new(sizeof (GtGenomeNode*));
      list_view = gtk_ltr_families_get_list_view_families(
                                            GTK_LTR_FAMILIES(ltrfilt->ltrfams));
      model = gtk_tree_view_get_model(GTK_TREE_VIEW(list_view));
      sel = gtk_tree_view_get_selection(list_view);
      rows = gtk_tree_selection_get_selected_rows(sel, &model);
      tmp = rows;
      while (tmp != NULL) {
        gtk_tree_model_get_iter(model, &iter, (GtkTreePath*) tmp->data);
        gtk_tree_model_get(model, &iter,
                           LTRFAMS_FAM_LV_NODE_ARRAY, &tmp_nodes, -1);
        gt_array_add_array(nodes, tmp_nodes);
        tmp = tmp->next;
      }
      gt_genome_nodes_sort_stable(nodes);
      total_candidates = gt_array_size(nodes);
      g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
      g_list_free(rows);
      break;
    case LTR_FILTER_RANGE_CANDIDATES:
      nodes = gt_array_new(sizeof (GtGenomeNode*));
      notebook =
              gtk_ltr_families_get_notebook(GTK_LTR_FAMILIES(ltrfilt->ltrfams));
      tab_no = gtk_notebook_get_current_page(notebook);
      tab_child = gtk_notebook_get_nth_page(notebook, tab_no);
      children = gtk_container_get_children(GTK_CONTAINER(tab_child));
      list_view = GTK_TREE_VIEW(g_list_first(children)->data);
      model = gtk_tree_view_get_model(list_view);
      sel = gtk_tree_view_get_selection(list_view);
      rows = gtk_tree_selection_get_selected_rows(sel, &model);
      tmp = rows;
      while (tmp != NULL) {
        gtk_tree_model_get_iter(model, &iter, (GtkTreePath*) tmp->data);
        gtk_tree_model_get(model, &iter,
                           LTRFAMS_FAM_LV_NODE_ARRAY, &gn, -1);
        gt_array_add(nodes, gn);
        tmp = tmp->next;
      }
      total_candidates = gt_array_size(nodes);
      gt_genome_nodes_sort_stable(nodes);
      g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
      g_list_free(rows);
      break;
    default:
      break;
  }
  if (!nodes)
    had_err = -1;
  if (!had_err) {
    array_in_stream = gt_array_in_stream_new(nodes, NULL, err);
  }
  if (!array_in_stream)
    had_err = -1;
  if (!had_err)
    script_filter_stream = ltrgui_script_filter_stream_new(array_in_stream,
                                                           filter_files, negate,
                                                           logic, err);
  if (!script_filter_stream)
    had_err = -1;
  if (!had_err)
    array_out_stream = gt_array_out_stream_new(script_filter_stream,
                                               filtered_nodes, err);
  if (!array_out_stream)
    had_err = -1;
  if (!had_err)
    had_err = gt_node_stream_pull(array_out_stream, err);

  gt_node_stream_delete(script_filter_stream);
  gt_node_stream_delete(array_in_stream);
  gt_node_stream_delete(array_out_stream);
  if ((ltrfilt->range == LTR_FILTER_RANGE_FAMILIES) ||
      (ltrfilt->range == LTR_FILTER_RANGE_CANDIDATES)) {
    gt_array_delete(nodes);
  }

  if (!had_err) {
    action = gtk_combo_box_get_active(GTK_COMBO_BOX(ltrfilt->filter_action));
    if (gt_array_size(filtered_nodes) == 0) {
      gt_array_delete(filtered_nodes);
      return;
    }
    switch (action) {
      case LTR_FILTER_ACTION_DELETE:
        for (i = 0; i < gt_array_size(filtered_nodes); i++) {
          gn = *(GtGenomeNode**) gt_array_get(filtered_nodes, i);
          cdata = (CandidateData*) gt_genome_node_get_user_data(gn, "cdata");
          if (!cdata)
            return;
          if (cdata->fam_ref) {
            GtkWidget *main_tab;
            GtkNotebook *noteb;
            GList *children;
            gint main_tab_no;

            delete_from_family(cdata, gn,
             gtk_ltr_families_get_notebook(GTK_LTR_FAMILIES(ltrfilt->ltrfams)));
            unclassified_candidates++;
            noteb =
              gtk_ltr_families_get_notebook(GTK_LTR_FAMILIES(ltrfilt->ltrfams));
            main_tab_no = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(noteb),
                                                            "main_tab"));
            main_tab = gtk_notebook_get_nth_page(noteb, main_tab_no);
            children = gtk_container_get_children(GTK_CONTAINER(main_tab));
            list_view = GTK_TREE_VIEW(g_list_first(children)->data);
            fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);
            curnode = gt_feature_node_iterator_next(fni);
            attr = gt_feature_node_get_attribute(curnode, ATTR_LTRFAM);
            if (attr)
              gt_feature_node_remove_attribute(curnode, ATTR_LTRFAM);
            attr = gt_feature_node_get_attribute(curnode, ATTR_FULLLEN);
            if (attr)
              gt_feature_node_remove_attribute(curnode, ATTR_FULLLEN);
            gtk_ltr_families_notebook_list_view_append_gn(
                                             GTK_LTR_FAMILIES(ltrfilt->ltrfams),
                                                 list_view, gn, NULL, NULL,
                                                 NULL, NULL);
            gt_feature_node_iterator_delete(fni);
            gtk_ltr_families_update_unclassified_cands(
                                             GTK_LTR_FAMILIES(ltrfilt->ltrfams),
                                                       1);
          } else if (!cdata->fam_ref && cdata->cand_ref) {
            nodes =
                 gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrfilt->ltrfams));
            remove_row(cdata->cand_ref);
            remove_node_from_array(nodes, gn);
            delete_gt_genome_node(gn);
            deleted_candidates++;
            gtk_ltr_families_update_unclassified_cands(
                                             GTK_LTR_FAMILIES(ltrfilt->ltrfams),
                                                       -1);

          }
        }
        g_snprintf(filter_message, BUFSIZ, LTR_FILTER_DIALOG,
                   gt_array_size(filtered_nodes), total_candidates,
                   unclassified_candidates, deleted_candidates);
        dialog = gtk_message_dialog_new(
                          GTK_WINDOW(gtk_widget_get_toplevel(ltrfilt->ltrfams)),
                                        GTK_DIALOG_MODAL |
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                        "%s", filter_message);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        break;
      case LTR_FILTER_ACTION_NEW_FAM:
        srand(time(NULL));
        g_snprintf(fam_name, BUFSIZ, "%s%d", LTR_FILTER_NEW_FAM_NAME,
                   rand() % 100);
        for (i = 0; i < gt_array_size(filtered_nodes); i++) {
          gn = *(GtGenomeNode**) gt_array_get(filtered_nodes, i);
          cdata = (CandidateData*) gt_genome_node_get_user_data(gn, "cdata");
          if (!cdata) {
            g_warning("%s", "Programming error!");
            return;
          }
          if (cdata->fam_ref)
            delete_from_family(cdata, gn,
             gtk_ltr_families_get_notebook(GTK_LTR_FAMILIES(ltrfilt->ltrfams)));
          if (cdata->cand_ref) {
            remove_row(cdata->cand_ref);
            cdata->cand_ref = NULL;
            if (!cdata->fam_ref)
              gtk_ltr_families_update_unclassified_cands(
                                             GTK_LTR_FAMILIES(ltrfilt->ltrfams),
                                                         -1);
          }

          fni = gt_feature_node_iterator_new((GtFeatureNode*) gn);
          curnode = gt_feature_node_iterator_next(fni);
          gt_feature_node_set_attribute(curnode, ATTR_LTRFAM, fam_name);
          attr = gt_feature_node_get_attribute(curnode, ATTR_FULLLEN);
          if (attr)
            gt_feature_node_remove_attribute(curnode, ATTR_FULLLEN);
          gt_feature_node_iterator_delete(fni);
        }
        gtk_ltr_families_notebook_list_view_append_array(
                                             GTK_LTR_FAMILIES(ltrfilt->ltrfams),
                                                NULL, filtered_nodes, NULL);
        break;
      default:
        break;
    }
  }
  gt_array_delete(filtered_nodes);
}

static void cancel_clicked(GT_UNUSED GtkButton *button, gpointer user_data)
{
  gtk_widget_hide(GTK_WIDGET(user_data));
}

static void delete_event(GtkWidget *widget, GT_UNUSED GdkEvent *event,
                         GT_UNUSED gboolean user_data)
{
  gtk_widget_hide(widget);
}

static void list_view_all_filter_changed(GtkTreeView *list_view,
                                         GtkLTRFilter *ltrfilt)
{
  GtkTreeSelection *sel;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreePath *path;
  GtError *err;
  GtScriptFilter *script_filter;
  GList *rows;
  gchar *file;
  const char *email, *author, *descr;

  err = gt_error_new();
  sel = gtk_tree_view_get_selection(list_view);
  if (gtk_tree_selection_count_selected_rows(sel) != 1)
    return;
  model = gtk_tree_view_get_model(list_view);
  rows = gtk_tree_selection_get_selected_rows(sel, &model);
  path = (GtkTreePath*) g_list_first(rows)->data;
  gtk_tree_model_get_iter(model, &iter, path);
  gtk_tree_model_get(model, &iter, 0, &file, -1);

  script_filter = gt_script_filter_new(file, err);
  if (!script_filter)
    error_handle(GTK_WIDGET(ltrfilt), err);
  else {
    author = gt_script_filter_get_author(script_filter, err);
    if (gt_error_is_set(err))
      error_handle(GTK_WIDGET(ltrfilt), err);
    email = gt_script_filter_get_email(script_filter, err);
    if (gt_error_is_set(err))
      error_handle(GTK_WIDGET(ltrfilt), err);
    descr = gt_script_filter_get_description(script_filter, err);
    if (gt_error_is_set(err))
      error_handle(GTK_WIDGET(ltrfilt), err);

    gtk_label_set_text(GTK_LABEL(ltrfilt->label_email), email);
    gtk_label_set_text(GTK_LABEL(ltrfilt->label_author), author);
    gtk_label_set_text(GTK_LABEL(ltrfilt->label_descr), descr);
  }

  g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
  g_list_free(rows);
  gt_script_filter_delete(script_filter);
  gt_error_delete(err);
  g_free(file);
}

static void add_clicked(GT_UNUSED GtkWidget *button, GtkLTRFilter *ltrfilt)
{
  GtkWidget *filechooser,
            *dialog;
  GtkFileFilter *lua_file_filter;
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtError *err;
  GtScriptFilter *script_filter;
  GSList *filenames;
  gchar *file = NULL;
  gboolean valid,
           skipped = FALSE;

  filechooser = gtk_file_chooser_dialog_new("Select lua filter files",
                                            GTK_WINDOW(ltrfilt),
                                            GTK_FILE_CHOOSER_ACTION_OPEN,
                                            GTK_STOCK_CANCEL,
                                            GTK_RESPONSE_CANCEL,
                                            GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                            NULL);

  gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(filechooser), TRUE);
  if (ltrfilt->last_dir != NULL)
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(filechooser),
                                        ltrfilt->last_dir);
  else
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(filechooser),
                                        g_get_home_dir());
  lua_file_filter = gtk_file_filter_new();
  gtk_file_filter_set_name(lua_file_filter, LUA_FILTER_PATTERN);
  gtk_file_filter_add_pattern(lua_file_filter, LUA_FILTER_PATTERN);
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filechooser), lua_file_filter);
  gint result = gtk_dialog_run(GTK_DIALOG(filechooser));

  if (result == GTK_RESPONSE_ACCEPT) {
    err = gt_error_new();
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfilt->list_view_all));
    filenames = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(filechooser));
    while (filenames != NULL) {
      file = (gchar*) filenames->data;
      if (!entry_in_list_view(model, file, LTR_FILTER_LV_FILE)) {
        script_filter = gt_script_filter_new(file, err);
        if (script_filter != NULL) {
          valid = gt_script_filter_validate(script_filter, err);
          if (valid) {
            gtk_list_store_append(GTK_LIST_STORE(model), &iter);
            gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, file, -1);
            add_file_to_sqlite(ltrfilt, file);
          } else
            skipped = TRUE;
        } else
          skipped = TRUE;
        gt_script_filter_delete(script_filter);
        gt_error_unset(err);
      }
      filenames = filenames->next;
    }
    if (ltrfilt->last_dir)
      g_free(ltrfilt->last_dir);
    ltrfilt->last_dir = g_path_get_dirname(file);
    g_slist_foreach(filenames, (GFunc) g_free, NULL);
    g_slist_free(filenames);
    gt_error_delete(err);
  }
  gtk_widget_destroy(filechooser);
  if (skipped) {
    dialog = gtk_message_dialog_new(GTK_WINDOW(ltrfilt),
                                    GTK_DIALOG_MODAL |
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                    "%s",
                                    LTR_FILTER_NOT_ADDED_FILES);
    gtk_window_set_title(GTK_WINDOW(dialog), "Attention!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  }
}

static void back_clicked(GT_UNUSED GtkWidget *button,
                         GtkLTRFilter *ltrfilt)
{
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  GtkTreeRowReference *rowref;
  GList *rows, *tmp, *references = NULL;

  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(ltrfilt->list_view_sel));

  if (!gtk_tree_selection_count_selected_rows(sel))
    return;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfilt->list_view_sel));
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
  g_list_foreach(references, (GFunc) gtk_tree_row_reference_free, NULL);
  g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
  g_list_free(references);
  g_list_free(rows);
}

static void forward_clicked(GT_UNUSED GtkWidget *button, GtkLTRFilter *ltrfilt)
{
  GtkTreeModel *model_all, *model_sel;
  GtkTreeSelection *sel;
  GtkTreeIter iter_all, iter_sel;
  GtkTreePath *path;
  GList *rows, *tmp;
  gchar *file;

  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(ltrfilt->list_view_all));
  if (gtk_tree_selection_count_selected_rows(sel) < 1)
    return;
  model_all = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfilt->list_view_all));
  model_sel = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfilt->list_view_sel));
  rows = gtk_tree_selection_get_selected_rows(sel, &model_all);
  tmp = rows;
  while (tmp != NULL) {
    path = (GtkTreePath*) tmp->data;
    gtk_tree_model_get_iter(model_all, &iter_all, path);
    gtk_tree_model_get(model_all, &iter_all, LTR_FILTER_LV_FILE, &file, -1);
    gtk_list_store_append(GTK_LIST_STORE(model_sel), &iter_sel);
    gtk_list_store_set(GTK_LIST_STORE(model_sel), &iter_sel,
                       LTR_FILTER_LV_FILE, file,
                       LTR_FILTER_LV_SEL_NOT, FALSE,
                       -1);
    tmp = tmp->next;
  }
  g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
  g_list_free(rows);
  g_free(file);
}

static void remove_clicked(GT_UNUSED GtkWidget *button, GtkLTRFilter *ltrfilt)
{
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  GtkTreeRowReference *rowref;
  GList *rows, *tmp, *references = NULL;

  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(ltrfilt->list_view_all));

  if (!gtk_tree_selection_count_selected_rows(sel))
    return;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfilt->list_view_all));
  rows = gtk_tree_selection_get_selected_rows(sel, &model);
  tmp = rows;

  while (tmp != NULL) {
    rowref = gtk_tree_row_reference_new(model, (GtkTreePath*) tmp->data);
    references =
                g_list_prepend(references, gtk_tree_row_reference_copy(rowref));
    gtk_tree_row_reference_free(rowref);
    tmp = tmp->next;
  }

  g_list_foreach(references, (GFunc) remove_file_from_sqlite, ltrfilt);
  g_list_foreach(references, (GFunc) remove_row, NULL);
  g_list_foreach(references, (GFunc) gtk_tree_row_reference_free, NULL);
  g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
  g_list_free(references);
  g_list_free(rows);
}

static void edit_clicked(GT_UNUSED GtkWidget *button, GtkLTRFilter *ltrfilt)
{
  GtkTreeSelection *sel;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreePath *path;
  GList *rows;
  gchar *file;

  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(ltrfilt->list_view_all));
  if (gtk_tree_selection_count_selected_rows(sel) < 1)
    return;
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrfilt->list_view_all));
  rows = gtk_tree_selection_get_selected_rows(sel, &model);
  path = (GtkTreePath*) rows->data;
  gtk_tree_model_get_iter(model, &iter, path);
  gtk_tree_model_get(model, &iter, 0, &file, -1);

  ltrfilt->cur_filename = file;
  create_edit_dialog(ltrfilt);

  g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
  g_list_free(rows);
}

static void new_clicked(GT_UNUSED GtkWidget *button, GtkLTRFilter *ltrfilt)
{
  ltrfilt->cur_filename = NULL;
  create_edit_dialog(ltrfilt);
}

static void
list_view_selected_filter_not_toggled(GT_UNUSED GtkCellRendererToggle *rend,
                                      gchar *path, GtkTreeView *treeview)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  gboolean value;

  /* Toggle the cell renderer's current state to the logical not. */
  model = gtk_tree_view_get_model(treeview);
  if (gtk_tree_model_get_iter_from_string (model, &iter, path)) {
    gtk_tree_model_get(model, &iter, LTR_FILTER_LV_SEL_NOT, &value, -1);
    gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                       LTR_FILTER_LV_SEL_NOT, !value, -1);
  }
}

static void list_view_cell_data_func(GT_UNUSED GtkTreeViewColumn *tree_column,
                                     GtkCellRenderer *renderer,
                                     GtkTreeModel *model,
                                     GtkTreeIter *iter,
                                     GT_UNUSED gpointer data)
{
  GtError *err;
  GtScriptFilter *script_filter;
  gchar *file, text[BUFSIZ];

  gtk_tree_model_get(model, iter, LTR_FILTER_LV_FILE, &file, -1);

  err = gt_error_new();
  script_filter = gt_script_filter_new(file, err);
  g_snprintf(text, BUFSIZ, "<b>%s</b> %s\n<small>%s</small>",
             gt_script_filter_get_name(script_filter, err),
             gt_script_filter_get_version(script_filter, err),
             gt_script_filter_get_short_description(script_filter, err));
  g_object_set(renderer, "markup", text, NULL);

  g_free(file);
  gt_error_delete(err);
}

static void gtk_ltr_filter_init(GtkLTRFilter *ltrfilt)
{
  GtkWidget *cancel,
            *vbox,
            *vbox2,
            *hbox,
            *image,
            *sw1,
            *label,
            *hsep2,
            *button,
            *radio;
  GtkListStore *store;
  GtkTreeViewColumn *column;
  GtkTreeSelection *sel;
  GtkCellRenderer *renderer;
  PangoAttrList *pattrl;
  PangoAttribute *pattr;
  GType *types;

  pattr = pango_attr_weight_new(PANGO_WEIGHT_BOLD);
  pattrl = pango_attr_list_new();
  pango_attr_list_insert(pattrl, pattr);

  vbox = gtk_vbox_new(FALSE, 1);
  /* add/remove/edit/new filter buttons */
  hbox = gtk_hbox_new(FALSE, 1);
  vbox2 = gtk_vbox_new(FALSE, 1);
  button = gtk_button_new_from_stock(GTK_STOCK_ADD);
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(add_clicked), ltrfilt);
  gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, FALSE, 1);
  button = gtk_button_new_from_stock(GTK_STOCK_REMOVE);
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(remove_clicked), ltrfilt);
  gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, FALSE, 1);
  button = gtk_button_new_from_stock(GTK_STOCK_NEW);
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(new_clicked), ltrfilt);
  gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, FALSE, 1);
  button = gtk_button_new_from_stock(GTK_STOCK_EDIT);
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(edit_clicked), ltrfilt);
  gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, 1);

  /* filter pool */
  sw1 = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw1),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
  ltrfilt->list_view_all = gtk_tree_view_new();
  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("lua filter files",
                                                    renderer,
                                                    "markup",
                                                    LTR_FILTER_LV_FILE,
                                                    NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrfilt->list_view_all), column);
  gtk_tree_view_column_set_cell_data_func(column, renderer,
                                          list_view_cell_data_func,
                                          NULL, NULL);
  store = gtk_list_store_new(1, G_TYPE_STRING);
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(ltrfilt->list_view_all));
  gtk_tree_selection_set_mode(sel, GTK_SELECTION_MULTIPLE);
  gtk_tree_view_set_model(GTK_TREE_VIEW(ltrfilt->list_view_all),
                          GTK_TREE_MODEL(store));
  g_object_unref(store);
  gtk_container_add(GTK_CONTAINER(sw1), ltrfilt->list_view_all);
  gtk_box_pack_start(GTK_BOX(hbox), sw1, TRUE, TRUE, 1);

  /* assign/unasign filter buttons */
  vbox2 = gtk_vbox_new(FALSE, 1);
  button = gtk_button_new();
  image = gtk_image_new_from_stock(GTK_STOCK_GO_FORWARD,
                                   GTK_ICON_SIZE_SMALL_TOOLBAR);
  gtk_button_set_image(GTK_BUTTON(button), image);

  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(forward_clicked), ltrfilt);
  gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, FALSE, 1);
  button = gtk_button_new();
  image = gtk_image_new_from_stock(GTK_STOCK_GO_BACK,
                                   GTK_ICON_SIZE_SMALL_TOOLBAR);
  gtk_button_set_image(GTK_BUTTON(button), image);
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(back_clicked), ltrfilt);
  gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, 1);

  /* selected filter */
  sw1 = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw1),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
  ltrfilt->list_view_sel = gtk_tree_view_new();
  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Selected lua filter files",
                                                    renderer,
                                                    "markup",
                                                    LTR_FILTER_LV_FILE,
                                                    NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrfilt->list_view_sel), column);
  gtk_tree_view_column_set_cell_data_func(column, renderer,
                                          list_view_cell_data_func, NULL, NULL);
  renderer = gtk_cell_renderer_toggle_new();
  g_object_set((gpointer) renderer, "activatable", TRUE, NULL);
  g_signal_connect(G_OBJECT(renderer), "toggled",
                   G_CALLBACK(list_view_selected_filter_not_toggled),
                   ltrfilt->list_view_sel);
  column = gtk_tree_view_column_new_with_attributes("Invert?",
                                                    renderer,
                                                    "active",
                                                    LTR_FILTER_LV_SEL_NOT,
                                                    NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrfilt->list_view_sel), column);
  column = gtk_tree_view_column_new();
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrfilt->list_view_sel), column);

  types = g_new0(GType, LTR_FILTER_LV_SEL_N_COLUMNS);
  types[0] = G_TYPE_STRING;
  types[1] = G_TYPE_BOOLEAN;
  store = gtk_list_store_newv(LTR_FILTER_LV_SEL_N_COLUMNS, types);
  gtk_tree_view_set_model(GTK_TREE_VIEW(ltrfilt->list_view_sel),
                          GTK_TREE_MODEL(store));
  g_object_unref(store);
  g_free(types);

  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(ltrfilt->list_view_sel));
  gtk_tree_selection_set_mode(sel, GTK_SELECTION_MULTIPLE);
  gtk_container_add(GTK_CONTAINER(sw1), ltrfilt->list_view_sel);
  gtk_box_pack_start(GTK_BOX(hbox), sw1, TRUE, TRUE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 1);

  vbox2 = gtk_vbox_new(FALSE, 1);
  ltrfilt->filter_logic = gtk_radio_button_new_with_label(NULL, LTR_FILTER_AND);
  gtk_box_pack_start(GTK_BOX(vbox2), ltrfilt->filter_logic, FALSE, FALSE, 1);
  radio = gtk_radio_button_new_with_label_from_widget(
                                        GTK_RADIO_BUTTON(ltrfilt->filter_logic),
                                                      LTR_FILTER_OR);
  gtk_box_pack_start(GTK_BOX(vbox2), radio, FALSE, FALSE, 1);

  hbox = gtk_hbox_new(FALSE, 1);
  label =
     gtk_label_new("Action to perform with filtered candidates:");
  gtk_label_set_attributes(GTK_LABEL(label), pattrl);
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
  ltrfilt->filter_action = gtk_combo_box_new_text();
  gtk_combo_box_append_text(GTK_COMBO_BOX(ltrfilt->filter_action),
                            LTR_FILTER_ACTION_DELETE_TEXT);
  gtk_combo_box_set_active(GTK_COMBO_BOX(ltrfilt->filter_action),
                            LTR_FILTER_ACTION_DELETE);
  gtk_combo_box_append_text(GTK_COMBO_BOX(ltrfilt->filter_action),
                            LTR_FILTER_ACTION_NEW_FAM_TEXT);
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), ltrfilt->filter_action, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox2, TRUE, TRUE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 1);

  hsep2 = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(vbox), hsep2, FALSE, FALSE, 1);

  label = gtk_label_new("Further script information");
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  ltrfilt->label_descr = gtk_label_new("");
  gtk_misc_set_alignment(GTK_MISC(ltrfilt->label_descr), 0.0, 0.5);
  ltrfilt->label_author = gtk_label_new("");
  gtk_misc_set_alignment(GTK_MISC(ltrfilt->label_author), 0.0, 0.5);
  ltrfilt->label_email = gtk_label_new("");
  gtk_misc_set_alignment(GTK_MISC(ltrfilt->label_email), 0.0, 0.5);

  label = gtk_label_new("Author:");
  gtk_label_set_attributes(GTK_LABEL(label), pattrl);
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
  hsep2 = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrfilt->label_author, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), hsep2, FALSE, FALSE, 1);

  label = gtk_label_new("Author email:");
  gtk_label_set_attributes(GTK_LABEL(label), pattrl);
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
  hsep2 = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrfilt->label_email, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), hsep2, FALSE, FALSE, 1);

  label = gtk_label_new("Description:");
  gtk_label_set_attributes(GTK_LABEL(label), pattrl);
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
  hsep2 = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrfilt->label_descr, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), hsep2, FALSE, FALSE, 1);

  hbox = gtk_hbox_new(FALSE, 0);
  ltrfilt->apply = gtk_button_new_with_mnemonic("_Apply");
  g_signal_connect(G_OBJECT(ltrfilt->apply), "clicked",
                   G_CALLBACK(apply_clicked), ltrfilt);
  cancel = gtk_button_new_with_mnemonic("_Cancel");
  g_signal_connect(G_OBJECT(cancel), "clicked", G_CALLBACK(cancel_clicked),
                   ltrfilt);
  gtk_box_pack_start(GTK_BOX(hbox), ltrfilt->apply, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), cancel, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 1);

  gtk_widget_show_all(vbox);
  gtk_container_add(GTK_CONTAINER(ltrfilt), vbox);

  /* connect signals */
  g_signal_connect(G_OBJECT(ltrfilt->list_view_all), "cursor-changed",
                   G_CALLBACK(list_view_all_filter_changed), ltrfilt);
  gtk_window_resize(GTK_WINDOW(ltrfilt), 800, 600);

  pango_attr_list_unref(pattrl);
}

static gboolean gtk_ltr_filter_destroy(GtkWidget *widget,
                                       GT_UNUSED GdkEvent *event,
                                       GT_UNUSED gpointer user_data)
{
  GtkLTRFilter *ltrfilt;
  ltrfilt = GTK_LTR_FILTER(widget);
  if (ltrfilt->last_dir)
    g_free(ltrfilt->last_dir);

  return FALSE;
}

GType gtk_ltr_filter_get_type(void)
{
  static GType ltr_filter_type = 0;

  if (!ltr_filter_type) {
    const GTypeInfo ltr_filter_info =
    {
      sizeof (GtkLTRFilterClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      NULL, /*(GClassInitFunc) */
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (GtkLTRFilter),
      0, /* n_preallocs */
      (GInstanceInitFunc) gtk_ltr_filter_init,
    };
    ltr_filter_type = g_type_register_static(GTK_TYPE_WINDOW, "GtkLTRFilter",
                                             &ltr_filter_info, 0);
  }
  return ltr_filter_type;
}

GtkWidget* gtk_ltr_filter_new(GtkWidget *ltrfams)
{
  GtkLTRFilter *ltrfilt;
  gchar title[BUFSIZ];

  ltrfilt = gtk_type_new(GTK_LTR_FILTER_TYPE);
  ltrfilt->last_dir = NULL;
  ltrfilt->ltrfams = ltrfams;
  g_signal_connect(G_OBJECT(ltrfilt), "delete_event",
                   G_CALLBACK(delete_event), NULL);
  g_signal_connect(G_OBJECT(ltrfilt), "destroy",
                   G_CALLBACK(gtk_ltr_filter_destroy), NULL);
  gtk_window_set_position(GTK_WINDOW(ltrfilt), GTK_WIN_POS_CENTER);
  gtk_window_set_modal(GTK_WINDOW(ltrfilt), TRUE);
  g_snprintf(title, BUFSIZ, LTRFILT_WINDOW_TITLE, GUI_NAME);
  gtk_window_set_title(GTK_WINDOW(ltrfilt), title);
  gtk_container_set_border_width(GTK_CONTAINER(ltrfilt), 5);

  return GTK_WIDGET(ltrfilt);
}
