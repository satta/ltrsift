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

#include "menubar_main.h"
#include "statusbar_main.h"
#include "gtk_ltr_families.h"

void mb_main_init(GUIData *ltrgui)
{
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
  g_object_set_data(G_OBJECT(ltrgui->mb_main_file_close),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_CLOSE);
  g_object_set_data(G_OBJECT(ltrgui->mb_main_file_quit),
                    SB_MAIN_MENU_HINT, (gpointer) SB_MAIN_MENU_HINT_QUIT);
}

static gchar* mb_main_file_import_get_filename(GUIData *ltrgui)
{
  GtkWidget *filechooser;
  GtkFileFilter *gff3_file_filter;
  gchar *filename = NULL;

  filechooser = gtk_file_chooser_dialog_new(GUI_DIALOG_OPEN,
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
  }

  gtk_widget_destroy(filechooser);

  return filename;
}

static gchar* mb_main_file_save_as_get_filename(GUIData *ltrgui)
{
  GtkWidget *filechooser;
  gchar *filename = NULL;

  filechooser = gtk_file_chooser_dialog_new(GUI_DIALOG_SAVE_AS,
                                            GTK_WINDOW (ltrgui->main_window),
                                            GTK_FILE_CHOOSER_ACTION_SAVE,
                                            GTK_STOCK_CANCEL,
                                            GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE,
                                            GTK_RESPONSE_ACCEPT, NULL);

  if (gtk_dialog_run(GTK_DIALOG(filechooser)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser));
  }

  gtk_widget_destroy(filechooser);

  return filename;
}

void mb_main_file_save_activate(GT_UNUSED GtkMenuItem *menuitem,
                                   GUIData *ltrgui)
{
  gchar *filename;

  if (ltrgui->project_filename == NULL) {
    filename = mb_main_file_save_as_get_filename(ltrgui);
    if (filename != NULL) ltrgui->project_filename = filename;
    /* TODO: if (filename != NULL) Save project data */
  } /* TODO: else Save as project data*/
}

void mb_main_file_save_as_activate(GT_UNUSED GtkMenuItem *menuitem,
                                      GUIData *ltrgui)
{
  gchar *filename;

  filename = mb_main_file_save_as_get_filename(ltrgui);

  if (filename != NULL) {
    if (ltrgui->project_filename != NULL) g_free(ltrgui->project_filename);
    ltrgui->project_filename = filename;
  }
  /* TODO: - Check for overwriting existing file
           - Save project data */
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

static int get_keys(void *key, GT_UNUSED void *values, void *cptns,
                         GT_UNUSED GtError *err)
{
  GtStrArray *captions = (GtStrArray*) cptns;
  const char *caption = (const char*) key;

  gt_str_array_add_cstr(captions, caption);

  return 0;
}

void mb_main_view_columns_set_submenu(GUIData *ltrgui,
                                      GtHashmap *features,
                                      GtError *err)
{
  GtkWidget *menu,
            *menuitem;
  GtStrArray *captions;
  unsigned long i;

  captions = gt_str_array_new();
  menu = gtk_menu_new();

  gt_hashmap_foreach(features, get_keys, (void*) captions, err);

  for (i = 0; i < gt_str_array_size(captions); i++) {
    menuitem =
              gtk_check_menu_item_new_with_label(gt_str_array_get(captions, i));
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem), TRUE);
    g_signal_connect(G_OBJECT(menuitem), "toggled",
                     G_CALLBACK(mb_main_view_columns_toggled),
                     ltrgui->ltr_families);
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
  gt_str_array_delete(captions);
}

void mb_main_file_import_activate(GT_UNUSED GtkMenuItem *menuitem,
                                  GUIData *ltrgui)
{
  gchar *filename;

  /* TODO: check for modified project (check for save) */

  filename = mb_main_file_import_get_filename(ltrgui);

  if (filename != NULL) {
    /*if (ltrgui->project_filename != NULL)
      g_free(ltrgui->project_filename);
    ltrgui->project_filename = filename;*/

    GtArray *nodes;
    GtHashmap *features;
    GtNodeStream *last_stream = NULL,
                 *gff3_in_stream = NULL,
                 *preprocess_stream = NULL,
                 *array_out_stream = NULL;
    GtError *err = gt_error_new();
    int had_err = 0;
    unsigned long n_features;

    nodes = gt_array_new(sizeof(GtGenomeNode*));
    features = gt_hashmap_new(GT_HASH_STRING, free_hash, NULL);
    n_features = LTRFAMS_LV_N_COLUMS;
    last_stream = gff3_in_stream = gt_gff3_in_stream_new_sorted(filename);
    last_stream = preprocess_stream =
                  gt_ltrgui_preprocess_stream_new(last_stream,
                                                  features,
                                                  &n_features, err);
    last_stream = array_out_stream = gt_ltrgui_array_out_stream_new(last_stream,
                                                                    nodes,
                                                                    err);
    had_err = gt_node_stream_pull(last_stream, err);
    if (!had_err) {
      gtk_ltr_families_fill_with_data(GTK_LTR_FAMILIES(ltrgui->ltr_families),
                                      nodes,
                                      features,
                                      n_features);
      mb_main_view_columns_set_submenu(ltrgui, features, err);
    }
    /* TODO: else -> handle error */
    gt_node_stream_delete(gff3_in_stream);
    gt_node_stream_delete(preprocess_stream);
    gt_node_stream_delete(array_out_stream);
    gt_error_delete(err);
  }
}

void mb_main_file_open_activate(GT_UNUSED GtkMenuItem *menuitem,
                                GT_UNUSED GUIData *ltrgui)
{
  return;
}

void mb_main_file_new_activate(GT_UNUSED GtkMenuItem *menuitem, GUIData *ltrgui)
{
  gtk_widget_show(ltrgui->project_wizard);
}

void mb_main_file_close_activate(GT_UNUSED GtkMenuItem *menuitem,
                                 GUIData *ltrgui)
{
  gtk_widget_set_sensitive(ltrgui->mb_main_view_columns, false);
  gtk_widget_destroy(ltrgui->ltr_families);
  ltrgui->ltr_families = gtk_ltr_families_new();
  gtk_box_pack_start(GTK_BOX(ltrgui->vbox1_main), ltrgui->ltr_families,
                     TRUE, TRUE, 0);
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
