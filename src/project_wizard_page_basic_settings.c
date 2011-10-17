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

#include "project_wizard_page_basic_settings.h"

void pw_basic_files_page_reset_defaults(GUIData *ltrgui)
{
  GtkTreeModel *model;
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->pw_treeview_gff3));
  gtk_list_store_clear(GTK_LIST_STORE(model));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(
                               ltrgui->pw_do_clustering_cb), FALSE);
  gtk_label_set_label(GTK_LABEL(ltrgui->pw_label_projectname), "");
  gtk_label_set_label(GTK_LABEL(ltrgui->pw_label_encseq), "");
}


void update_gff3_label(GtkTreeView *list_view, GtkLabel *gff3_label)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  gboolean valid;
  gchar *text,
        *buffer;

  model = gtk_tree_view_get_model(list_view);
  valid = gtk_tree_model_get_iter_first(model, &iter);
  if (valid) {
    gtk_tree_model_get(model, &iter,
                       0, &buffer,
                       -1);
    text = g_strdup(buffer);
    g_free(buffer);
    while (gtk_tree_model_iter_next(model, &iter)) {
      gtk_tree_model_get(model, &iter,
                         0, &buffer,
                         -1);
      text = g_strjoin("\n", text, buffer, NULL);
      g_free(buffer);
    }
    gtk_label_set_label(gff3_label, text);
    g_free(text);
  }
}

/* not needed function unless the user should be able to select more than
  one seq file

void pw_add_encseq_button_clicked(GT_UNUSED GtkButton *button, GUIData *ltrgui)
{
  GtkWidget *filechooser;
  gchar *filename = NULL;

  filechooser = gtk_file_chooser_dialog_new(PW_SELECT_FILE,
                                          GTK_WINDOW(ltrgui->project_wizard),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);

  gint result = gtk_dialog_run(GTK_DIALOG(filechooser));

  if (result == GTK_RESPONSE_ACCEPT) {
    GtkTreeIter iter;
    GtkTreeModel *model =
            gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->pw_treeview_encseq));
    gtk_list_store_clear(GTK_LIST_STORE(model));
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser));

    if (filename) {
      gtk_list_store_append(GTK_LIST_STORE(model), &iter);
      gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, filename, -1);
      g_free(filename);
    }

    if (g_strcmp0(gtk_entry_get_text(
        GTK_ENTRY(ltrgui->pw_entry_projectname)), "") != 0) {
      gtk_assistant_set_page_complete(GTK_ASSISTANT(ltrgui->project_wizard),
              gtk_assistant_get_nth_page(GTK_ASSISTANT(ltrgui->project_wizard),
                                         PW_SELECTFILES), TRUE);
    }
  }
  gtk_widget_destroy(filechooser);
}
*/

static void pw_basic_files_page_complete(GUIData *ltrgui)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkLabel *label_gff3 = GTK_LABEL(ltrgui->pw_label_projectname);
  GtkLabel *label_encseq = GTK_LABEL(ltrgui->pw_label_encseq);
  gboolean empty_gff3;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->pw_treeview_gff3));
  empty_gff3 = gtk_tree_model_get_iter_first(model, &iter);

  if ((g_strcmp0(gtk_label_get_label(label_gff3), "") != 0) && empty_gff3 &&
      (g_strcmp0(gtk_label_get_label(label_encseq), "") != 0)) {
    gtk_assistant_set_page_complete(GTK_ASSISTANT(ltrgui->project_wizard),
        gtk_assistant_get_nth_page(GTK_ASSISTANT(ltrgui->project_wizard),
                                                        PW_SELECTFILES), TRUE);
  } else {
    gtk_assistant_set_page_complete(GTK_ASSISTANT(ltrgui->project_wizard),
        gtk_assistant_get_nth_page(GTK_ASSISTANT(ltrgui->project_wizard),
                                                        PW_SELECTFILES), FALSE);
  }
}

void pw_do_clustering_cb_toggled(GtkToggleButton *togglebutton, GUIData *ltrgui)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(ltrgui->pw_exp_clustering, active);
  gtk_expander_set_expanded(GTK_EXPANDER(ltrgui->pw_exp_clustering), active);
  gtk_label_set_label(GTK_LABEL(ltrgui->pw_label_clustering),
                      active ? "yes" : "no");
}

void pw_choose_projectname_button_clicked(GT_UNUSED GtkButton *button,
                                          GUIData *ltrgui)
{
  GtkWidget *filechooser;
  gchar *filename = NULL;

  filechooser = gtk_file_chooser_dialog_new("Select",
                                            GTK_WINDOW(ltrgui->project_wizard),
                                            GTK_FILE_CHOOSER_ACTION_SAVE,
                                            GTK_STOCK_CANCEL,
                                            GTK_RESPONSE_CANCEL, GTK_STOCK_OK,
                                            GTK_RESPONSE_ACCEPT, NULL);

  if (gtk_dialog_run(GTK_DIALOG(filechooser)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser));
    gtk_label_set_label(GTK_LABEL(ltrgui->pw_label_projectname), filename);
    gtk_label_set_label(GTK_LABEL(ltrgui->pw_label_projectname2), filename);
    g_free(filename);
  }
  gtk_widget_destroy(filechooser);
  pw_basic_files_page_complete(ltrgui);
}

void pw_add_gff3_button_clicked(GT_UNUSED GtkButton *button, GUIData *ltrgui)
{
  GtkWidget *filechooser;
  GtkFileFilter *gff3_file_filter;
  GSList *filenames;

  filechooser = gtk_file_chooser_dialog_new(PW_SELECT_FILE,
                                          GTK_WINDOW(ltrgui->project_wizard),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);

  gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(filechooser), TRUE);
  gff3_file_filter = gtk_file_filter_new();
  gtk_file_filter_set_name(gff3_file_filter, GFF3_PATTERN);
  gtk_file_filter_add_pattern(gff3_file_filter, GFF3_PATTERN);
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filechooser), gff3_file_filter);
  gint result = gtk_dialog_run(GTK_DIALOG(filechooser));

  if (result == GTK_RESPONSE_ACCEPT) {
    GtkTreeIter iter;
    GtkTreeModel *model =
            gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->pw_treeview_gff3));
    gtk_list_store_clear(GTK_LIST_STORE(model));
    /*ltrgui->project_files = */
    filenames = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(filechooser));

    while (filenames != NULL) {
      gchar *file = (gchar*) filenames->data;

      gtk_list_store_append(GTK_LIST_STORE(model), &iter);
      gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, file, -1);

      filenames = filenames->next;
    }
    update_gff3_label(GTK_TREE_VIEW(ltrgui->pw_treeview_gff3),
                      GTK_LABEL(ltrgui->pw_label_gff3_files));
  }
  gtk_widget_destroy(filechooser);
  pw_basic_files_page_complete(ltrgui);
}

static void pw_remove_gff3_row(GtkTreeRowReference *rowref, GUIData *ltrgui)
{
  GtkTreeIter iter, tmp;
  GtkTreePath *path;
  GtkTreeModel *model;
  gboolean empty;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->pw_treeview_gff3));

  path = gtk_tree_row_reference_get_path(rowref);
  gtk_tree_model_get_iter(model, &iter, path);

  gtk_list_store_remove(GTK_LIST_STORE(model), &iter);

  empty = gtk_tree_model_get_iter_first(model, &tmp);

  pw_basic_files_page_complete(ltrgui);
}

void pw_remove_gff3_button_clicked(GT_UNUSED GtkButton *button, GUIData *ltrgui)
{
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  GtkTreeRowReference *rowref;
  GList *rows, *tmp, *references = NULL;

  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(ltrgui->pw_treeview_gff3));

  if (!gtk_tree_selection_count_selected_rows(sel))
    return;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->pw_treeview_gff3));
  rows = gtk_tree_selection_get_selected_rows(sel, &model);

  tmp = rows;

  while (tmp != NULL) {
    rowref = gtk_tree_row_reference_new(model, (GtkTreePath*) tmp->data);
    references =
                g_list_prepend(references, gtk_tree_row_reference_copy(rowref));
    gtk_tree_row_reference_free(rowref);
    tmp = tmp->next;
  }

  g_list_foreach(references, (GFunc) pw_remove_gff3_row, ltrgui);
  update_gff3_label(GTK_TREE_VIEW(ltrgui->pw_treeview_gff3),
                    GTK_LABEL(ltrgui->pw_label_gff3_files));
  g_list_foreach(references, (GFunc) gtk_tree_row_reference_free, NULL);
  g_list_foreach(rows, (GFunc) gtk_tree_path_free, NULL);
  g_list_free(references);
  g_list_free(rows);
}

void pw_choose_encseq_button_clicked(GT_UNUSED GtkButton *button,
                                     GUIData *ltrgui)
{
  GtkWidget *filechooser;
  gchar *filename = NULL;

  filechooser = gtk_file_chooser_dialog_new("Select",
                                            GTK_WINDOW(ltrgui->project_wizard),
                                            GTK_FILE_CHOOSER_ACTION_OPEN,
                                            GTK_STOCK_CANCEL,
                                            GTK_RESPONSE_CANCEL, GTK_STOCK_OK,
                                            GTK_RESPONSE_ACCEPT, NULL);

  if (gtk_dialog_run(GTK_DIALOG(filechooser)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser));
    gtk_label_set_label(GTK_LABEL(ltrgui->pw_label_encseq), filename);
    gtk_label_set_label(GTK_LABEL(ltrgui->pw_label_encseq2), filename);
    g_free(filename);
  }
  gtk_widget_destroy(filechooser);
  pw_basic_files_page_complete(ltrgui);
}
