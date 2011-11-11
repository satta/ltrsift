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

#include "gtk_ltr_assistant.h"

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

gdouble gtk_ltr_assistant_get_evalue(GtkLTRAssistant *ltrassi)
{
  return gtk_spin_button_get_value(GTK_SPIN_BUTTON(ltrassi->spinb_evalue));
}

gboolean gtk_ltr_assistant_get_dust(GtkLTRAssistant *ltrassi)
{
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ltrassi->checkb_dust));
}

gint gtk_ltr_assistant_get_gapopen(GtkLTRAssistant *ltrassi)
{
  return
      gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ltrassi->spinb_gapopen));
}

gint gtk_ltr_assistant_get_gapextend(GtkLTRAssistant *ltrassi)
{
  return
    gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ltrassi->spinb_gapextend));
}

gdouble gtk_ltr_assistant_get_xdrop(GtkLTRAssistant *ltrassi)
{
  return gtk_spin_button_get_value(GTK_SPIN_BUTTON(ltrassi->spinb_xdrop));
}

gint gtk_ltr_assistant_get_penalty(GtkLTRAssistant *ltrassi)
{
  return
      gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ltrassi->spinb_penalty));
}

gint gtk_ltr_assistant_get_reward(GtkLTRAssistant *ltrassi)
{
  return
       gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ltrassi->spinb_reward));
}

gint gtk_ltr_assistant_get_threads(GtkLTRAssistant *ltrassi)
{
  return
      gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ltrassi->spinb_threads));
}

gint gtk_ltr_assistant_get_wordsize(GtkLTRAssistant *ltrassi)
{
  return
      gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ltrassi->spinb_words));
}

gdouble gtk_ltr_assistant_get_seqid(GtkLTRAssistant *ltrassi)
{
  return gtk_spin_button_get_value(GTK_SPIN_BUTTON(ltrassi->spinb_seqid));
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

gdouble gtk_ltr_assistant_get_candtol(GtkLTRAssistant *ltrassi)
{
  return gtk_spin_button_get_value(GTK_SPIN_BUTTON(ltrassi->spinb_candtol));
}

GtkTreeView* gtk_ltr_assistant_get_list_view_features(GtkLTRAssistant *ltrassi)
{
  return GTK_TREE_VIEW(ltrassi->list_view_features);
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

static void free_hash(void *elem)
{
  gt_free(elem);
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
  gtk_list_store_append(GTK_LIST_STORE(model), &iter);
  gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                     0, feature,
                     -1);
  return 0;
}

static void get_feature_list(GtkLTRAssistant *ltrassi)
{
  GtkTreeView *list_view;
  GtkTreeIter iter;
  GtkTreeSelection *sel;
  GtkTreeModel *model;
  GList *rows, *tmp;
  GtNodeStream *last_stream = NULL,
               *gff3_in_stream = NULL,
               *ltrgui_preprocess_stream = NULL;
  GtHashmap *features;
  GtError *err;
  const char **gff3_files;
  gint num_of_files, had_err = 0, i = 0;
  unsigned long n = 0;

  err = gt_error_new();
  list_view = GTK_TREE_VIEW(ltrassi->list_view_gff3files);
  sel = gtk_tree_view_get_selection(list_view);
  gtk_tree_selection_select_all(sel);
  num_of_files = gtk_tree_selection_count_selected_rows(sel);
  if (num_of_files == 0)
    return;
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
  features = gt_hashmap_new(GT_HASH_STRING, free_hash, NULL);
  last_stream = ltrgui_preprocess_stream =
                                  gt_ltrgui_preprocess_stream_new(last_stream,
                                                                  features,
                                                                  &n,
                                                                  TRUE,
                                                                  err);
  had_err = gt_node_stream_pull(last_stream, err);

  gtk_list_store_clear(GTK_LIST_STORE(
                         gtk_tree_view_get_model(
                                        GTK_TREE_VIEW(
                                                ltrassi->list_view_features))));
  gt_hashmap_foreach(features, fill_feature_list,
                     (void*) ltrassi->list_view_features, err);

  gt_error_delete(err);
  gt_hashmap_delete(features);
  gt_node_stream_delete(gff3_in_stream);
  gt_node_stream_delete(ltrgui_preprocess_stream);
  for (i = 0; i < num_of_files; i++)
    g_free((gpointer) gff3_files[i]);
  g_free(gff3_files);
}

static gboolean file_in_list_view(GtkTreeModel *model, const gchar *file)
{
  GtkTreeIter iter;
  gchar *name;
  gboolean valid;

  valid = gtk_tree_model_get_iter_first(model, &iter);
  if (valid) {
    gtk_tree_model_get(model, &iter,
                       0, &name,
                       -1);
    if (g_strcmp0(name, file) == 0) {
      g_free(name);
      return TRUE;
    }
    while (gtk_tree_model_iter_next(model, &iter)) {
      gtk_tree_model_get(model, &iter,
                         0, &name,
                         -1);
      if (g_strcmp0(name, file) == 0) {
        g_free(name);
        return TRUE;
      }
      g_free(name);
    }
  }
  return FALSE;
}

static void remove_list_view_row(GtkTreeRowReference *rowref,
                                 GtkTreeModel *model)
{
  GtkTreeIter iter;
  GtkTreePath *path;

  path = gtk_tree_row_reference_get_path(rowref);
  gtk_tree_model_get_iter(model, &iter, path);

  gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
}

static void update_gff3_label(GtkTreeView *list_view, GtkLabel *gff3_label)
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
    text = g_path_get_basename(buffer);
    g_free(buffer);
    while (gtk_tree_model_iter_next(model, &iter)) {
      gtk_tree_model_get(model, &iter,
                         0, &buffer,
                         -1);
      text = g_strjoin("\n", text, g_path_get_basename(buffer), NULL);
      g_free(buffer);
    }
    gtk_label_set_label(gff3_label, text);
    g_free(text);
  }
}

static void update_features_label(GtkTreeView *list_view,
                                  GtkLabel *feature_label)
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
    gtk_label_set_label(feature_label, text);
    g_free(text);
  }
}

void list_view_features_selection_changed(GtkTreeSelection *sel,
                                          GtkLTRAssistant *ltrassi)
{
  if (gtk_tree_selection_count_selected_rows(sel) == 0)
    gtk_assistant_set_page_complete(GTK_ASSISTANT(ltrassi),
                        gtk_assistant_get_nth_page(GTK_ASSISTANT(ltrassi),
                                                   PAGE_CLASSIFICATION), FALSE);
  else
    gtk_assistant_set_page_complete(GTK_ASSISTANT(ltrassi),
                         gtk_assistant_get_nth_page(GTK_ASSISTANT(ltrassi),
                                                    PAGE_CLASSIFICATION), TRUE);
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

  g_list_foreach(references, (GFunc) remove_list_view_row, model);
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

  filechooser = gtk_file_chooser_dialog_new(SELECT_GFF3_FILES,
                                            GTK_WINDOW(ltrassi),
                                            GTK_FILE_CHOOSER_ACTION_OPEN,
                                            GTK_STOCK_CANCEL,
                                            GTK_RESPONSE_CANCEL,
                                            GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                            NULL);

  gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(filechooser), TRUE);
  gff3_file_filter = gtk_file_filter_new();
  gtk_file_filter_set_name(gff3_file_filter, GFF3_PATTERN);
  gtk_file_filter_add_pattern(gff3_file_filter, GFF3_PATTERN);
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filechooser), gff3_file_filter);
  gint result = gtk_dialog_run(GTK_DIALOG(filechooser));

  if (result == GTK_RESPONSE_ACCEPT) {
    model =
           gtk_tree_view_get_model(GTK_TREE_VIEW(ltrassi->list_view_gff3files));
    filenames = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(filechooser));
    while (filenames != NULL) {
      gchar *file = (gchar*) filenames->data;

      if (!file_in_list_view(model, file)) {
        gtk_list_store_append(GTK_LIST_STORE(model), &iter);
        gtk_list_store_set(GTK_LIST_STORE(model), &iter,
                           0, file,
                           -1);
      }
      filenames = filenames->next;
    }
    update_gff3_label(GTK_TREE_VIEW(ltrassi->list_view_gff3files),
                      GTK_LABEL(ltrassi->label_gff3files));
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

  if (gtk_dialog_run(GTK_DIALOG(filechooser)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser));
    if (g_strcmp0(gtk_button_get_label(button), BROWSE_PROJECT) == 0) {
      gtk_label_set_label(GTK_LABEL(ltrassi->label_projectfile), filename);
      gtk_label_set_label(GTK_LABEL(ltrassi->label_projectfile2),
                          g_path_get_basename(filename));
    } else {
      gtk_label_set_label(GTK_LABEL(ltrassi->label_indexname), filename);
      gtk_label_set_label(GTK_LABEL(ltrassi->label_indexname2),
                          g_path_get_basename(filename));
    }
    g_free(filename);
  }
  gtk_widget_destroy(filechooser);
  check_complete_page_general(ltrassi);
}

static void checkb_clustering_toggled(GtkToggleButton *togglebutton,
                                      GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_label_set_label(GTK_LABEL(ltrassi->label_doclustering),
                      active ? "yes" : "no");
  /*if (active) {
    GtkSpinButton *sbutton;
    gchar buf[6];

    sbutton = GTK_SPIN_BUTTON(ltrgui->pw_spinbutton_psmall);
    g_snprintf(buf, 6, "%d", gtk_spin_button_get_value_as_int(sbutton));
    gtk_label_set_label(GTK_LABEL(ltrgui->pw_label_psmall), buf);
    sbutton = GTK_SPIN_BUTTON(ltrgui->pw_spinbutton_plarge);
    g_snprintf(buf, 6, "%d", gtk_spin_button_get_value_as_int(sbutton));
    gtk_label_set_label(GTK_LABEL(ltrgui->pw_label_plarge), buf);
    sbutton = GTK_SPIN_BUTTON(ltrgui->pw_spinbutton_xdrop);
    g_snprintf(buf, 6, "%d", gtk_spin_button_get_value_as_int(sbutton));
    gtk_label_set_label(GTK_LABEL(ltrgui->pw_label_xdrop), buf);
    sbutton = GTK_SPIN_BUTTON(ltrgui->pw_spinbutton_words);
    g_snprintf(buf, 6, "%d", gtk_spin_button_get_value_as_int(sbutton));
    gtk_label_set_label(GTK_LABEL(ltrgui->pw_label_words), buf);
    sbutton = GTK_SPIN_BUTTON(ltrgui->pw_spinbutton_seqid);
    g_snprintf(buf, 6, "%.1f", gtk_spin_button_get_value(sbutton));
    gtk_label_set_label(GTK_LABEL(ltrgui->pw_label_seqid), buf);
  }*/
  /* pw_page_basic_settings_complete(ltrgui); */
}

static void checkb_classification_toggled(GtkToggleButton *togglebutton,
                                   GtkLTRAssistant *ltrassi)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_label_set_label(GTK_LABEL(ltrassi->label_doclassification),
                      active ? "yes" : "no");
}

static void gtk_ltr_assistant_init(GtkLTRAssistant *ltrassi)
{
  LTRAssistantPageInfo page_info[N_PAGES] = {
    {NULL, -1, "Introduction", GTK_ASSISTANT_PAGE_INTRO, TRUE},
    {NULL, -1, "General settings", GTK_ASSISTANT_PAGE_CONTENT, FALSE},
    {NULL, -1, "Matching/Clustering", GTK_ASSISTANT_PAGE_CONTENT, TRUE},
    {NULL, -1, "Classification", GTK_ASSISTANT_PAGE_CONTENT, FALSE},
    {NULL, -1, "Overview", GTK_ASSISTANT_PAGE_CONFIRM, TRUE}};

  GtkWidget *label,
            *hsep,
            *button,
            *hbox,
            *vbox,
            *vbox1,
            *vbox2,
            *sw,
            *align;
  GtkTreeViewColumn *column;
  GtkTreeSelection *sel;
  GtkCellRenderer *renderer;
  GtkListStore *store;
  GtkObject *adjust;
  gint i;

  /* Introduction page */
  page_info[PAGE_INTRODUCTION].widget = gtk_label_new("PLACEHOLDER");

  /* General settings page */
  page_info[PAGE_GENERAL].widget = gtk_vbox_new(FALSE, 5);
  label = gtk_label_new("PLACEHOLDER");
  hsep = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_GENERAL].widget),
                     label, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_GENERAL].widget),
                     hsep, FALSE, FALSE, 1);
  label = gtk_label_new("Please choose a directory and a name for the project");
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
  label = gtk_label_new("Please select GFF3 file(s)");
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
  label = gtk_label_new("Please select the associated indexname");
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
                              gtk_check_button_new_with_label("Do clustering?");
  g_signal_connect(G_OBJECT(ltrassi->checkb_clustering), "toggled",
                   G_CALLBACK(checkb_clustering_toggled), (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_GENERAL].widget),
                     ltrassi->checkb_clustering, FALSE, FALSE, 1);

  /* Matching/Clustering settings page */
  page_info[PAGE_CLUSTERING].widget = gtk_vbox_new(FALSE, 5);
  label = gtk_label_new("Matches will be generated with BLASTN. Please set "
                        "the following parameters");
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLUSTERING].widget),
                     label, FALSE, FALSE, 1);
  hbox = gtk_hbox_new(FALSE, 5);
  vbox = gtk_vbox_new(TRUE, 1);
  label = gtk_label_new("Expectation value (E) threshold:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Filtered query sequence with DUST?");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
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
  label = gtk_label_new("X-dropoff value (in bits):");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Penalty for a nucleotide mismatch:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Reward for a nucleotide match:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Number of threads (CPUs) used:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Word size for wordfinder algorithm:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Percent identity:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 1);

  vbox = gtk_vbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(10.0, 0.0001, 50.0, 0.0001, 0.1, 0.0);
  ltrassi->spinb_evalue = gtk_spin_button_new(GTK_ADJUSTMENT(adjust),
                                              0.0001, 4);
  ltrassi->checkb_dust = gtk_check_button_new();
  adjust = gtk_adjustment_new(0.0, 0.0, 100.0, 1.0, 10.0, 0.0);
  ltrassi->spinb_gapopen = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  adjust = gtk_adjustment_new(0.0, 0.0, 100.0, 1.0, 10.0, 0.0);
  ltrassi->spinb_gapextend = gtk_spin_button_new(GTK_ADJUSTMENT(adjust),
                                                 1.0, 0);
  adjust = gtk_adjustment_new(20.0, 5.0, 100.0, 1.0, 10.0, 0.0);
  ltrassi->spinb_xdrop = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  adjust = gtk_adjustment_new(0.0, -100.0, 0.0, -1.0, -10.0, 0.0);
  ltrassi->spinb_penalty = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), -1.0, 0);
  adjust = gtk_adjustment_new(0.0, 0.0, 100.0, 1.0, 10.0, 0.0);
  ltrassi->spinb_reward = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  adjust = gtk_adjustment_new(1.0, 1.0, 8.0, 1.0, 2.0, 0.0);
  ltrassi->spinb_threads = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  adjust = gtk_adjustment_new(4.0, 4.0, 20.0, 1.0, 4.0, 0.0);
  ltrassi->spinb_words = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  adjust = gtk_adjustment_new(80.0, 30.0, 100.0, 0.1, 1.0, 0.0);
  ltrassi->spinb_seqid = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 0.1, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrassi->spinb_evalue, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrassi->checkb_dust, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrassi->spinb_gapopen, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrassi->spinb_gapextend, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrassi->spinb_xdrop, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrassi->spinb_penalty, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrassi->spinb_reward, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrassi->spinb_threads, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrassi->spinb_words, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrassi->spinb_seqid, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 1);
  hsep = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLUSTERING].widget),
                     hbox, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLUSTERING].widget),
                     hsep, FALSE, FALSE, 1);
  label = gtk_label_new("Please set the following paramters for clustering");
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLUSTERING].widget),
                     label, FALSE, FALSE, 1);
  hbox = gtk_hbox_new(FALSE, 5);
  vbox = gtk_vbox_new(TRUE, 1);
  label = gtk_label_new("Value for psmall:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Value for psmall:");
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
                          gtk_check_button_new_with_label("Do classification?");
  g_signal_connect(G_OBJECT(ltrassi->checkb_classification), "toggled",
                   G_CALLBACK(checkb_classification_toggled),
                   (gpointer) ltrassi);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLUSTERING].widget),
                     hbox, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLUSTERING].widget),
                     ltrassi->checkb_classification, FALSE, FALSE, 1);

  /* Classification settings page */
  page_info[PAGE_CLASSIFICATION].widget = gtk_vbox_new(FALSE, 5);
  label = gtk_label_new("PLACEHOLDER");
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLASSIFICATION].widget),
                     label, FALSE, FALSE, 1);
  hbox = gtk_hbox_new(FALSE, 5);
  vbox = gtk_vbox_new(TRUE, 1);
  label = gtk_label_new("Allowed LTR length deviation from group median:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  label = gtk_label_new("Allowed candidate length deviation "
                        "from group median:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 1);

  vbox = gtk_vbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(0.0, 0.0, 100.0, 0.01, 1.0, 0.0);
  ltrassi->spinb_ltrtol = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 0.01, 2);
  adjust = gtk_adjustment_new(0.0, 0.0, 100.0, 0.01, 1.0, 0.0);
  ltrassi->spinb_candtol = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 0.01, 2);
  gtk_box_pack_start(GTK_BOX(vbox), ltrassi->spinb_ltrtol, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), ltrassi->spinb_candtol, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLASSIFICATION].widget),
                     hbox, FALSE, FALSE, 1);
  hsep = gtk_hseparator_new();
  label = gtk_label_new("Select features for classification");
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
                   (GCallback) list_view_features_selection_changed, ltrassi);
  gtk_container_add(GTK_CONTAINER(sw), ltrassi->list_view_features);
  gtk_box_pack_start(GTK_BOX(page_info[PAGE_CLASSIFICATION].widget), sw,
                     TRUE, TRUE, 1);

  /* Summary page */
  ltrassi->notebook = page_info[PAGE_SUMMARY].widget = gtk_notebook_new();
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
  label = ltrassi->label_doclustering = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, 1);
  label = gtk_label_new("General");
  gtk_notebook_append_page(GTK_NOTEBOOK(ltrassi->notebook), hbox, label);

  vbox1 = gtk_vbox_new(FALSE, 5);
  label = gtk_label_new("Expectation value (E) threshold:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox1), align, FALSE, FALSE, 1);
  label = gtk_label_new("Filter query sequence with DUST?");
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
  label = ltrassi->label_evalue = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_dust = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
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
  label = ltrassi->label_xdrop = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_penalty = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_reward = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_threads = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_wordsize = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
  label = ltrassi->label_seqidentity = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
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
  label = ltrassi->label_doclassification = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
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
  label = gtk_label_new("Used features:");
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
  label = ltrassi->label_usedfeatures = gtk_label_new("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  align = gtk_alignment_new(1.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), label);
  gtk_box_pack_start(GTK_BOX(vbox2), align, FALSE, FALSE, 1);
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
}

gint page_forward(gint current_page, GtkLTRAssistant *ltrassi)
{
  gint next_page;
  gboolean active;

  switch (current_page) {
    case PAGE_INTRODUCTION:
      next_page = PAGE_GENERAL;
      break;
    case PAGE_GENERAL:
      active =  gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(
                                               ltrassi->checkb_clustering));
      next_page = (active ? PAGE_CLUSTERING : PAGE_SUMMARY);
      break;
    case PAGE_CLUSTERING:
      active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(
                                              ltrassi->checkb_classification));
      if (active)
        get_feature_list(ltrassi);
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

static void gtk_ltr_assistant_cancel(GtkAssistant *assistant,
                                     GT_UNUSED gpointer data)

{
  gtk_widget_hide(GTK_WIDGET(assistant));
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
  ltrassi = gtk_type_new(GTK_LTR_ASSISTANT_TYPE);
  gtk_assistant_set_forward_page_func(GTK_ASSISTANT(ltrassi),
                                      (GtkAssistantPageFunc) page_forward,
                                      ltrassi, NULL);
  gtk_window_set_position(GTK_WINDOW(ltrassi), GTK_WIN_POS_CENTER_ALWAYS);
  g_signal_connect(G_OBJECT(ltrassi), "cancel",
                   G_CALLBACK(gtk_ltr_assistant_cancel), NULL);
  g_signal_connect(G_OBJECT(ltrassi), "close",
                   G_CALLBACK(gtk_ltr_assistant_cancel), NULL);

  return GTK_WIDGET(ltrassi);
}
