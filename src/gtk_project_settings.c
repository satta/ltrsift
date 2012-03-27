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
#include "error.h"
#include "gtk_project_settings.h"
#include "message_strings.h"

const gchar* gtk_project_settings_get_indexname(GtkProjectSettings *projset)
{
  return gtk_label_get_text(GTK_LABEL(projset->label_indexname));
}

void gtk_project_settings_update_indexname(GtkProjectSettings *projset,
                                           const gchar *indexname)
{
  GtRDBStmt *stmt = NULL;
  GtError *err;
  gchar query[BUFSIZ];
  gint had_err = 0;

  gtk_label_set_text(GTK_LABEL(projset->label_indexname), indexname);
  if (projset->rdb != NULL) {
    err = gt_error_new();
    g_snprintf(query, BUFSIZ,
               "UPDATE project_settings SET indexname = \"%s\"",
               gtk_label_get_text(GTK_LABEL(projset->label_indexname)));
    had_err = gt_rdb_prepare(projset->rdb, query, -1, &stmt, err);
    if (!had_err) {
      had_err = gt_rdb_stmt_exec(stmt, err);
      gt_rdb_stmt_delete(stmt);
    }
    if (had_err == -1)
      error_handle(GTK_WIDGET(projset), err);
    gt_error_delete(err);
  }
}

void gtk_project_settings_update_projectfile(GtkProjectSettings *projset,
                                             const gchar *projectfile)
{
  gtk_label_set_text(GTK_LABEL(projset->label_projectfile), projectfile);
}

void gtk_project_settings_set_data(GtkProjectSettings *projset,
                                   const gchar *projectfile,
                                   gchar **gff3files,
                                   const gchar *indexname,
                                   gboolean clustering,
                                   gdouble evalue,
                                   gboolean dust,
                                   gint gapopen,
                                   gint gapextend,
                                   gdouble xdrop,
                                   gint penalty,
                                   gint reward,
                                   gint threads,
                                   gint wordsize,
                                   gdouble seqid,
                                   const gchar *moreblast,
                                   gint psmall,
                                   gint plarge,
                                   gboolean classification,
                                   gdouble ltrtol,
                                   gdouble candtol,
                                   gchar **features)
{
  gchar *tmp, buffer[BUFSIZ];

  gtk_label_set_text(GTK_LABEL(projset->label_projectfile), projectfile);
  tmp = g_strjoinv("\n", gff3files);
  gtk_label_set_text(GTK_LABEL(projset->label_gff3files), tmp);
  g_free(tmp);
  gtk_label_set_text(GTK_LABEL(projset->label_indexname), indexname);
  gtk_label_set_text(GTK_LABEL(projset->label_didclustering),
                               clustering ? "yes" : "no");
  if (evalue == GT_UNDEF_DOUBLE)
    gtk_label_set_text(GTK_LABEL(projset->label_evalue), USED_DEFAULT);
  else {
    g_snprintf(buffer, BUFSIZ, "%.6f", evalue);
    gtk_label_set_text(GTK_LABEL(projset->label_evalue), buffer);
  }
  gtk_label_set_text(GTK_LABEL(projset->label_dust), dust ? "yes" : "no");
  if (gapopen == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(projset->label_gapopen), USED_DEFAULT);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", gapopen);
    gtk_label_set_text(GTK_LABEL(projset->label_gapopen), buffer);
  }
  if (gapextend == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(projset->label_gapextend), USED_DEFAULT);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", gapextend);
    gtk_label_set_text(GTK_LABEL(projset->label_gapextend), buffer);
  }
  if (xdrop == GT_UNDEF_DOUBLE)
    gtk_label_set_text(GTK_LABEL(projset->label_xdrop), USED_DEFAULT);
  else {
    g_snprintf(buffer, BUFSIZ, "%.2f", xdrop);
    gtk_label_set_text(GTK_LABEL(projset->label_xdrop), buffer);
  }
  if (penalty == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(projset->label_penalty), USED_DEFAULT);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", penalty);
    gtk_label_set_text(GTK_LABEL(projset->label_penalty), buffer);
  }
  if (reward == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(projset->label_reward), USED_DEFAULT);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", reward);
    gtk_label_set_text(GTK_LABEL(projset->label_reward), buffer);
  }
  if (threads == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(projset->label_threads), USED_DEFAULT);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", threads);
    gtk_label_set_text(GTK_LABEL(projset->label_threads), buffer);
  }
  if (wordsize == GT_UNDEF_INT)
    gtk_label_set_text(GTK_LABEL(projset->label_wordsize), USED_DEFAULT);
  else {
    g_snprintf(buffer, BUFSIZ, "%d", wordsize);
    gtk_label_set_text(GTK_LABEL(projset->label_wordsize), buffer);
  }
  if (seqid == GT_UNDEF_DOUBLE)
    gtk_label_set_text(GTK_LABEL(projset->label_seqidentity), USED_DEFAULT);
  else {
    g_snprintf(buffer, BUFSIZ, "%.2f", seqid);
    gtk_label_set_text(GTK_LABEL(projset->label_seqidentity), buffer);
  }
  gtk_label_set_text(GTK_LABEL(projset->label_moreblast), moreblast);
  g_snprintf(buffer, BUFSIZ, "%d", psmall);
  gtk_label_set_text(GTK_LABEL(projset->label_psmall), buffer);
  g_snprintf(buffer, BUFSIZ, "%d", plarge);
  gtk_label_set_text(GTK_LABEL(projset->label_plarge), buffer);
  gtk_label_set_text(GTK_LABEL(projset->label_didclassification),
                               classification ? "yes" : "no");
  g_snprintf(buffer, BUFSIZ, "%.1f", ltrtol);
  gtk_label_set_text(GTK_LABEL(projset->label_ltrtolerance), buffer);
  g_snprintf(buffer, BUFSIZ, "%.1f", candtol);
  gtk_label_set_text(GTK_LABEL(projset->label_candtolerance), buffer);
  tmp = g_strjoinv("\n", features);
  gtk_label_set_text(GTK_LABEL(projset->label_usedfeatures), tmp);
  g_free(tmp);
}

gint gtk_project_settings_set_data_from_sqlite(GtkProjectSettings *projset,
                                               const gchar *projectfile,
                                               GtError *err)
{
  GtRDBStmt *stmt;
  GtStr *result;
  gint had_err = 0;

  gtk_label_set_text(GTK_LABEL(projset->label_projectfile), projectfile);

  had_err = gt_rdb_prepare(projset->rdb,
                           "SELECT gff3files, indexname, clustering, evalue, "
                            "dust, gapopen, gapextend, xdrop, penalty, reward, "
                            "threads, wordsize, seq_identity, psmall, plarge, "
                            "classification, ltr_tolerance, cand_tolerance, "
                            "features, moreblast FROM project_settings",
                           -1, &stmt, err);
  if (had_err)
    return -1;
  if ((had_err = gt_rdb_stmt_exec(stmt, err)) < 0) {
    gt_rdb_stmt_delete(stmt);
    return -1;
  }
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 0, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_gff3files), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 1, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_indexname), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 2, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_didclustering),
                       gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 3, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_evalue), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 4, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_dust), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 5, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_gapopen), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 6, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_gapextend), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 7, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_xdrop), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 8, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_penalty), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 9, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_reward), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 10, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_threads), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 11, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_wordsize), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 12, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_seqidentity),
                       gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 13, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_psmall), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 14, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_plarge), gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 15, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_didclassification),
                       gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 16, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_ltrtolerance),
                       gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 17, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_candtolerance),
                       gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 18, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_usedfeatures),
                       gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  result = gt_str_new();
  had_err = gt_rdb_stmt_get_string(stmt, 19, result, err);
  if (!had_err)
    gtk_label_set_text(GTK_LABEL(projset->label_moreblast),
                       gt_str_get(result));
  else {
    gt_rdb_stmt_delete(stmt);
    gt_str_delete(result);
    return -1;
  }
  gt_str_delete(result);
  gt_rdb_stmt_delete(stmt);

  return 0;
}

gint gtk_project_settings_save_data(GtkProjectSettings *projset,
                                    GtError *err)
{
  GtRDB *rdb = NULL;
  GtRDBStmt *stmt;
  GtError *tmp_err;
  gchar query[BUFSIZ];
  gint had_err = 0;
  tmp_err = gt_error_new();

  rdb =
    gt_rdb_sqlite_new(gtk_label_get_text(GTK_LABEL(projset->label_projectfile)),
                      tmp_err);
  if (!rdb) {
    gt_error_set(err, "Could not save projekt settings: %s",
                 gt_error_get(tmp_err));
    gt_error_delete(err);
    return -1;
  }

  had_err = gt_rdb_prepare(rdb,
                           "CREATE TABLE IF NOT EXISTS project_settings "
                           "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "projectfile TEXT, "
                            "gff3files TEXT, "
                            "indexname TEXT, "
                            "clustering TEXT, "
                            "evalue TEXT, "
                            "dust TEXT, "
                            "gapopen TEXT, "
                            "gapextend TEXT, "
                            "xdrop TEXT, "
                            "penalty TEXT, "
                            "reward TEXT, "
                            "threads TEXT, "
                            "wordsize TEXT, "
                            "seq_identity TEXT, "
                            "moreblast TEXT, "
                            "psmall TEXT, "
                            "plarge TEXT, "
                            "classification TEXT, "
                            "ltr_tolerance TEXT, "
                            "cand_tolerance TEXT, "
                            "features TEXT)",
                           -1, &stmt, tmp_err);

  if (had_err) {
    gt_error_set(err, "Could not save projekt settings: %s",
                 gt_error_get(tmp_err));
    gt_error_delete(err);
    gt_rdb_delete(rdb);
    return -1;
  }
  if ((had_err = gt_rdb_stmt_exec(stmt, tmp_err)) < 0) {
    gt_error_set(err, "Could not save projekt settings: %s",
                 gt_error_get(tmp_err));
    gt_error_delete(err);
    gt_rdb_stmt_delete(stmt);
    gt_rdb_delete(rdb);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);

  had_err = gt_rdb_prepare(rdb, "DELETE FROM project_settings",
                           -1, &stmt, tmp_err);

  if (had_err) {
    gt_error_set(err, "Could not save projekt settings: %s",
                 gt_error_get(tmp_err));
    gt_error_delete(err);
    gt_rdb_delete(rdb);
    return -1;
  }
  if ((had_err = gt_rdb_stmt_exec(stmt, tmp_err)) < 0) {
    gt_error_set(err, "Could not save projekt settings: %s",
                 gt_error_get(tmp_err));
    gt_error_delete(err);
    gt_rdb_stmt_delete(stmt);
    gt_rdb_delete(rdb);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);

  g_snprintf(query, BUFSIZ,
             "INSERT INTO project_settings (projectfile, gff3files, "
              "indexname, clustering, evalue, dust, gapopen, "
              "gapextend, xdrop, penalty, reward, threads, wordsize, "
              "seq_identity, moreblast, psmall, plarge, classification, "
              "ltr_tolerance, cand_tolerance, features) values (\"%s\", "
              "\"%s\", \"%s\", \"%s\", "
              "\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", "
              "\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", "
              "\"%s\")",
             gtk_label_get_text(GTK_LABEL(projset->label_projectfile)),
             gtk_label_get_text(GTK_LABEL(projset->label_gff3files)),
             gtk_label_get_text(GTK_LABEL(projset->label_indexname)),
             gtk_label_get_text(GTK_LABEL(projset->label_didclustering)),
             gtk_label_get_text(GTK_LABEL(projset->label_evalue)),
             gtk_label_get_text(GTK_LABEL(projset->label_dust)),
             gtk_label_get_text(GTK_LABEL(projset->label_gapopen)),
             gtk_label_get_text(GTK_LABEL(projset->label_gapextend)),
             gtk_label_get_text(GTK_LABEL(projset->label_xdrop)),
             gtk_label_get_text(GTK_LABEL(projset->label_penalty)),
             gtk_label_get_text(GTK_LABEL(projset->label_reward)),
             gtk_label_get_text(GTK_LABEL(projset->label_threads)),
             gtk_label_get_text(GTK_LABEL(projset->label_wordsize)),
             gtk_label_get_text(GTK_LABEL(projset->label_seqidentity)),
             gtk_label_get_text(GTK_LABEL(projset->label_moreblast)),
             gtk_label_get_text(GTK_LABEL(projset->label_psmall)),
             gtk_label_get_text(GTK_LABEL(projset->label_plarge)),
             gtk_label_get_text(GTK_LABEL(projset->label_didclassification)),
             gtk_label_get_text(GTK_LABEL(projset->label_ltrtolerance)),
             gtk_label_get_text(GTK_LABEL(projset->label_candtolerance)),
             gtk_label_get_text(GTK_LABEL(projset->label_usedfeatures)));
  had_err = gt_rdb_prepare(rdb, query, -1, &stmt, tmp_err);

  if (had_err) {
    gt_error_set(err, "Could not save projekt settings: %s",
                 gt_error_get(tmp_err));
    gt_error_delete(err);
    gt_rdb_delete(rdb);
    return -1;
  }
  if ((had_err = gt_rdb_stmt_exec(stmt, tmp_err)) < 0) {
    gt_error_set(err, "Could not save projekt settings: %s",
                 gt_error_get(tmp_err));
    gt_error_delete(err);
    gt_rdb_stmt_delete(stmt);
    gt_rdb_delete(rdb);
    return -1;
  }
  gt_rdb_stmt_delete(stmt);
  gt_error_delete(err);
  gt_rdb_delete(rdb);
  return 0;
}

static void delete_event(GtkWidget *widget, GT_UNUSED GdkEvent *event,
                         GT_UNUSED gboolean user_data)
{
  gtk_widget_hide(widget);
}

static void change_index_clicked(GT_UNUSED GtkWidget *button,
                                 GtkProjectSettings *projset)
{
  GtkWidget *filechooser;
  GtkFileFilter *esq_file_filter;

  filechooser = gtk_file_chooser_dialog_new("Select indexname...",
                                            GTK_WINDOW(projset),
                                            GTK_FILE_CHOOSER_ACTION_SAVE,
                                            GTK_STOCK_CANCEL,
                                            GTK_RESPONSE_CANCEL, GTK_STOCK_OK,
                                            GTK_RESPONSE_ACCEPT, NULL);
  esq_file_filter = gtk_file_filter_new();
  gtk_file_filter_set_name(esq_file_filter, ESQ_FILTER_PATTERN);
  gtk_file_filter_add_pattern(esq_file_filter, ESQ_FILTER_PATTERN);
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filechooser), esq_file_filter);
  gtk_file_chooser_set_create_folders(GTK_FILE_CHOOSER(filechooser), FALSE);
  if (gtk_dialog_run(GTK_DIALOG(filechooser)) == GTK_RESPONSE_ACCEPT) {
    gchar *filename, *tmp;
    const gchar *projectfile;
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser));
    gtk_widget_destroy(filechooser);
    tmp = g_strndup(filename, strlen(filename) - strlen(ESQ_PATTERN));
    gtk_label_set_label(GTK_LABEL(projset->label_indexname), tmp);
    g_free(filename);
    g_free(tmp);

    projectfile = gtk_label_get_text(GTK_LABEL(projset->label_projectfile));
    if (g_strcmp0(projectfile, "") != 0) {
      GtRDBStmt *stmt;
      GtError *err = gt_error_new();
      gchar query[BUFSIZ];
      gint had_err;

      if (projset->rdb != NULL) {
        g_snprintf(query, BUFSIZ,
                   "UPDATE project_settings SET indexname = \"%s\"",
                   gtk_label_get_text(GTK_LABEL(projset->label_indexname)));
        had_err = gt_rdb_prepare(projset->rdb, query, -1, &stmt, err);
        if (!had_err) {
          had_err = gt_rdb_stmt_exec(stmt, err);
          gt_rdb_stmt_delete(stmt);
        }
        if (had_err == -1)
          error_handle(GTK_WIDGET(projset), err);
      }
      gt_error_delete(err);
    }
  }
}

static void close_clicked(GT_UNUSED GtkWidget *button,
                          GtkProjectSettings *projset)
{
  gtk_widget_hide(GTK_WIDGET(projset));
}

static void gtk_project_settings_init(GtkProjectSettings *projset)
{
  GtkWidget *hbox,
            *vbox,
            *vbox1,
            *vbox2,
            *label,
            *align,
            *button;

  projset->notebook = gtk_notebook_new();

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
  button = gtk_button_new_with_mnemonic("_Change/Set indexname");
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(change_index_clicked), projset);
  gtk_box_pack_start(GTK_BOX(vbox1), button, FALSE, FALSE, 1);
  vbox2 = gtk_vbox_new(FALSE, 5);
  label = projset->label_projectfile = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_gff3files = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_indexname = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_didclustering = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, 1);
  label = gtk_label_new("General settings");
  gtk_notebook_append_page(GTK_NOTEBOOK(projset->notebook), hbox, label);

  vbox1 = gtk_vbox_new(FALSE, 5);
  label = gtk_label_new("Expectation value (E) threshold:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Filtered query sequence with DUST?");
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
  label = projset->label_evalue = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_dust = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_gapopen = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_gapextend = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_xdrop = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_penalty = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_reward = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_threads = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_wordsize = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_seqidentity = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_moreblast = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_psmall = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_plarge = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_didclassification = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, 1);
  label = gtk_label_new("Cluster settings");
  gtk_notebook_append_page(GTK_NOTEBOOK(projset->notebook), hbox, label);

  vbox1 = gtk_vbox_new(FALSE, 5);
  label = gtk_label_new("allowed LTR length deviation:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("allowed candidate length deviation:");
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
  label = projset->label_ltrtolerance = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_candtolerance = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = projset->label_usedfeatures = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, 1);
  label = gtk_label_new("Classification settings");
  gtk_notebook_append_page(GTK_NOTEBOOK(projset->notebook), hbox, label);

  vbox = gtk_vbox_new(FALSE, 5);
  gtk_box_pack_start(GTK_BOX(vbox), projset->notebook, TRUE, TRUE, 1);
  button = gtk_button_new_with_mnemonic("_Close");
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(close_clicked), projset);

  align = gtk_alignment_new(1.0, 0.5, 0.3, 0.0);
  gtk_container_add(GTK_CONTAINER(align), button);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  gtk_widget_show_all(vbox);

  gtk_container_add(GTK_CONTAINER(projset), vbox);
}

GType gtk_project_settings_get_type(void)
{
  static GType project_settings_type = 0;

  if (!project_settings_type) {
    const GTypeInfo project_settings_info =
    {
      sizeof (GtkProjectSettingsClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      NULL, /*(GClassInitFunc) */
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (GtkProjectSettings),
      0, /* n_preallocs */
      (GInstanceInitFunc) gtk_project_settings_init,
    };
    project_settings_type = g_type_register_static(GTK_TYPE_WINDOW,
                                                   "GtkProjectSettings",
                                                   &project_settings_info, 0);
  }
  return project_settings_type;
}

GtkWidget* gtk_project_settings_new(GtRDB *rdb)
{
  GtkProjectSettings *projset;
  gchar title[BUFSIZ];

  projset = gtk_type_new(GTK_PROJECT_SETTINGS_TYPE);
  g_signal_connect(G_OBJECT(projset), "delete_event",
                   G_CALLBACK(delete_event), NULL);
  gtk_window_set_position(GTK_WINDOW(projset), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_window_set_modal(GTK_WINDOW(projset), TRUE);
  g_snprintf(title, BUFSIZ, PROJSET_WINDOW_TITLE, GUI_NAME);
  gtk_window_set_title(GTK_WINDOW(projset), title);
  gtk_container_set_border_width(GTK_CONTAINER(projset), 5);
  projset->rdb = rdb;

  return GTK_WIDGET(projset);
}
