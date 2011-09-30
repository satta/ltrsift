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

#include "notebook_families.h"
#include "treeview_families.h"
#include "treeview_families_dnd.h"

static void ltrfam_lv_popup_menu_remove_clicked(GT_UNUSED GtkWidget *menuitem,
                                                  GUIData *ltrgui)
{
  /* TODO: check for main_tab -> modify warning */
  GtkWidget *dialog,
            *main_tab,
            *tab_child;
  GtkTreeView *treeview;
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  GtkTreeIter iter;
  gint main_tab_no,
      tab_no;

  tab_no = gtk_notebook_get_current_page(GTK_NOTEBOOK(ltrgui->nb_families));
  tab_child = gtk_notebook_get_nth_page(GTK_NOTEBOOK(ltrgui->nb_families),
                                        tab_no);
  treeview =
           GTK_TREE_VIEW(gtk_ltr_family_get_list_view(GTKLTRFAMILY(tab_child)));
  model = gtk_tree_view_get_model(treeview);
  sel = gtk_tree_view_get_selection(treeview);
  if (gtk_tree_selection_count_selected_rows(sel) == 0)
    return;
  gtk_tree_selection_get_selected(sel, &model, &iter);

  main_tab_no =
      GPOINTER_TO_INT(
             g_object_get_data(G_OBJECT(ltrgui->nb_families), "main_tab"));

  dialog = gtk_message_dialog_new(NULL,
                          GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
                                  "Are you sure?");
  gtk_window_set_title(GTK_WINDOW(dialog), "Attention!");
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_NO) {
    gtk_widget_destroy(dialog);
    return;
  } else {
    GtGenomeNode **gn;
    GtkTreeRowReference *rowref;
    gtk_tree_model_get(model, &iter,
                       LTRFAM_LV_NODE, &gn,
                       LTRFAM_LV_ROWREF, &rowref,
                       -1);
    main_tab =
       gtk_notebook_get_nth_page(GTK_NOTEBOOK(ltrgui->nb_families),
                                 main_tab_no);
    gtk_ltr_family_list_view_append(GTKLTRFAMILY(main_tab), gn,
                                    ltrgui->features, NULL, NULL);
    if (rowref) {
      GtkTreeModel *model2;
      GtkTreePath *tv_path;
      GtkTreeIter tv_iter;
      GtArray *tmp_nodes;
      const char *tmp_oldname;
      char tmp_curname[BUFSIZ];

      model2 = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->tv_families));
      tv_path = gtk_tree_row_reference_get_path(rowref);
      gtk_tree_model_get_iter(model2, &tv_iter, tv_path);
      gtk_tree_model_get(model2, &tv_iter,
                         TV_FAM_NODE_ARRAY, &tmp_nodes,
                         TV_FAM_OLDNAME, &tmp_oldname,
                         -1);

      remove_node_from_array(tmp_nodes, gn);
      g_snprintf(tmp_curname, BUFSIZ, "%s (%lu)",
                 tmp_oldname, gt_array_size(tmp_nodes));
      gtk_tree_store_set(GTK_TREE_STORE(model2), &tv_iter,
                         TV_FAM_CURNAME, tmp_curname,
                         -1);
      gtk_tree_path_free(tv_path);
    }
    gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
  }
  gtk_widget_destroy(dialog);
}

static void ltrfam_lv_popup_menu(GT_UNUSED GtkWidget *treeview,
                                   GdkEventButton *event,
                                   GUIData *ltrgui)
{
  GtkWidget *menu, *menuitem;

  menu = gtk_menu_new();

  menuitem = gtk_menu_item_new_with_label("Remove selected");
  g_signal_connect(menuitem, "activate",
                   (GCallback) ltrfam_lv_popup_menu_remove_clicked, ltrgui);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  gtk_widget_show_all(menu);
  gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,
                 (event != NULL) ? event->button : 0,
                 gdk_event_get_time((GdkEvent*)event));
}

gboolean ltrfam_lv_button_pressed(GtkWidget *treeview,
                                    GdkEventButton *event,
                                    GUIData *ltrgui)
{
  if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3)
  {
    GtkTreeSelection *selection;
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    if (gtk_tree_selection_count_selected_rows(selection)  <= 1)
    {
      GtkTreePath *path;
      if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview),
                                        (gint) event->x,
                                        (gint) event->y,
                                        &path, NULL, NULL, NULL)) {
        gtk_tree_selection_unselect_all(selection);
        gtk_tree_selection_select_path(selection, path);
        gtk_tree_path_free(path);
      } else
        return TRUE;
    }
    ltrfam_lv_popup_menu(treeview, event, ltrgui);
    return TRUE;
  }
  return FALSE;
}

gboolean ltrfam_lv_on_popup_menu(GtkWidget *treeview, GUIData *ltrgui)
{
  ltrfam_lv_popup_menu(treeview, NULL, ltrgui);
  return TRUE;
}













static gboolean tv_families_check_if_name_exists(GUIData *ltrgui,
                                                 const gchar *new_name,
                                                 const gchar *tmp)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  gchar *famname,
        *iter_str;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->tv_families));
  gtk_tree_model_get_iter_first(model, &iter);
  gtk_tree_model_get(model, &iter,
                     TV_FAM_OLDNAME, &famname,
                     -1);
  iter_str = gtk_tree_model_get_string_from_iter(model, &iter);
  if ((g_strcmp0(famname, new_name) == 0) && (g_strcmp0(iter_str, tmp) != 0))
    return TRUE;
  g_free(famname);
  g_free(iter_str);
  while (gtk_tree_model_iter_next(model, &iter)) {
    iter_str = gtk_tree_model_get_string_from_iter(model, &iter);
    gtk_tree_model_get(model, &iter,
                       TV_FAM_OLDNAME, &famname,
                       -1);
    if ((g_strcmp0(famname, new_name) == 0) &&
        (g_strcmp0(iter_str, tmp) != 0))
      return TRUE;
    g_free(famname);
    g_free(iter_str);
  }
  return FALSE;
}

static void tv_families_popup_menu_edit_clicked(GT_UNUSED GtkWidget *menuitem,
                                                gpointer userdata)
{
  GtkTreeView *treeview;
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  GtkTreeViewColumn *column;
  GtkTreeIter iter;
  GtkTreePath *path;
  GtkCellRenderer *renderer;
  GList *tmp;

  treeview = GTK_TREE_VIEW(userdata);
  model = gtk_tree_view_get_model(treeview);
  sel = gtk_tree_view_get_selection(treeview);
  if (gtk_tree_selection_count_selected_rows(sel) == 0)
    return;
  column = gtk_tree_view_get_column(treeview, 0);
  tmp = gtk_cell_layout_get_cells(GTK_CELL_LAYOUT(column));
  renderer = (GtkCellRenderer*) g_list_nth_data(tmp, 0);
  g_object_set(renderer, "editable", TRUE, NULL);
  gtk_tree_selection_get_selected(sel, &model, &iter);
  path = gtk_tree_model_get_path(model, &iter);
  gtk_tree_view_set_cursor(treeview, path, column, TRUE);
}

void remove_node_from_array(GtArray *nodes, GtGenomeNode **gn)
{
  GtGenomeNode **tmp;
  unsigned long i;

  for (i = 0; i < gt_array_size(nodes); i++) {
    tmp = *(GtGenomeNode***) gt_array_get(nodes, i);
    if (gn == tmp) {
      gt_array_rem(nodes, i);
      return;
    }
  }
}

static void tv_families_popup_menu_remove_clicked(GT_UNUSED GtkWidget *menuitem,
                                                  GUIData *ltrgui)
{
  GtkWidget *dialog,
            *main_tab,
            *tab_label;
  GtkTreeView *treeview;
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  GtkTreeIter iter;
  gint main_tab_no;
  unsigned long i;

  treeview = GTK_TREE_VIEW(ltrgui->tv_families);
  model = gtk_tree_view_get_model(treeview);
  sel = gtk_tree_view_get_selection(treeview);
  if (gtk_tree_selection_count_selected_rows(sel) == 0)
    return;
  gtk_tree_selection_get_selected(sel, &model, &iter);

  main_tab_no =
      GPOINTER_TO_INT(
             g_object_get_data(G_OBJECT(ltrgui->nb_families), "main_tab"));

  dialog = gtk_message_dialog_new(NULL,
                          GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
                                  "Are you sure?");
  gtk_window_set_title(GTK_WINDOW(dialog), "Attention!");
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_NO) {
    gtk_widget_destroy(dialog);
    return;
  } else {
    GtArray *nodes;
    gtk_tree_model_get(model, &iter,
                       TV_FAM_NODE_ARRAY, &nodes,
                       -1);
    main_tab =
       gtk_notebook_get_nth_page(GTK_NOTEBOOK(ltrgui->nb_families),
                                 main_tab_no);
    for (i = 0; i < gt_array_size(nodes); i++) {
      GtGenomeNode **tmp_gn;
      tmp_gn = *(GtGenomeNode***) gt_array_get(nodes, i);
      gtk_ltr_family_list_view_append(GTKLTRFAMILY(main_tab), tmp_gn,
                                      ltrgui->features, NULL, NULL);
    }

    gtk_tree_model_get(model, &iter, TV_FAM_TAB_LABEL, &tab_label, -1);
    if (tab_label) {
      gtk_notebook_remove_page(GTK_NOTEBOOK(ltrgui->nb_families),
                  GPOINTER_TO_INT(
                   gtk_label_close_get_button_data(GTKLABELCLOSE(tab_label),
                                                   "nbpage")));
      nb_families_refresh_notebook_nums(
                                    GTK_NOTEBOOK(ltrgui->nb_families));
    }
    gtk_tree_store_remove(GTK_TREE_STORE(model), &iter);
  }
  gtk_widget_destroy(dialog);
}

static void tv_families_popup_menu(GtkWidget *treeview,
                                   GdkEventButton *event,
                                   GUIData *ltrgui)
{
  GtkWidget *menu, *menuitem;

  menu = gtk_menu_new();
  menuitem = gtk_menu_item_new_with_label("Edit name");
  g_signal_connect(menuitem, "activate",
                   (GCallback) tv_families_popup_menu_edit_clicked, treeview);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  menuitem = gtk_menu_item_new_with_label("Remove selected");
  g_signal_connect(menuitem, "activate",
                   (GCallback) tv_families_popup_menu_remove_clicked, ltrgui);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  gtk_widget_show_all(menu);
  gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,
                 (event != NULL) ? event->button : 0,
                 gdk_event_get_time((GdkEvent*)event));
}

gboolean tv_families_button_pressed(GtkWidget *treeview,
                                    GdkEventButton *event,
                                    GUIData *ltrgui)
{
  if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3)
  {
    GtkTreeSelection *selection;
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    if (gtk_tree_selection_count_selected_rows(selection)  <= 1)
    {
      GtkTreePath *path;
      if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview),
                                        (gint) event->x,
                                        (gint) event->y,
                                        &path, NULL, NULL, NULL)) {
        gtk_tree_selection_unselect_all(selection);
        gtk_tree_selection_select_path(selection, path);
        gtk_tree_path_free(path);
      } else
        return TRUE;
    }
    tv_families_popup_menu(treeview, event, ltrgui);
    return TRUE;
  }
  return FALSE;
}

gboolean tv_families_on_popup_menu(GtkWidget *treeview, GUIData *ltrgui)
{
  tv_families_popup_menu(treeview, NULL, ltrgui);
  return TRUE;
}

static void tv_families_cell_edit_started(GT_UNUSED GtkCellRenderer *renderer,
                                          GtkCellEditable *editable,
                                          gchar *path_str,
                                          GUIData *ltrgui)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreePath *path = NULL;
  const char *text = NULL;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->tv_families));

  path = gtk_tree_path_new_from_string (path_str);
  gtk_tree_model_get_iter (model, &iter, path);
  gtk_tree_path_free (path);
  gtk_tree_model_get(model, &iter,
                     TV_FAM_OLDNAME, &text,
                     -1);
  if (GTK_IS_ENTRY(editable) && (text != NULL)) {
    GtkEntry *entry = GTK_ENTRY (editable);
    gtk_entry_set_text(entry, text);
  }
}

static void tv_families_cell_edit_canceled(GT_UNUSED GtkCellRenderer *renderer,
                                           GUIData *ltrgui)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreeSelection *sel;
  gchar *name;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->tv_families));
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(ltrgui->tv_families));
  gtk_tree_selection_get_selected(sel, &model, &iter);
  gtk_tree_model_get(model, &iter, TV_FAM_CURNAME, &name, -1);

  if(!name)
    gtk_tree_store_remove(GTK_TREE_STORE(model), &iter);
  g_free(name);
 }

static void tv_families_cell_edited(GT_UNUSED GtkCellRendererText *cell,
                                    gchar *path_string, gchar *new_name,
                                    GUIData *ltrgui)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkTreeViewColumn *column;
  GtkCellRenderer *renderer;
  GList *tmp;
  gchar *old_name;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->tv_families));
  gtk_tree_model_get_iter_from_string(model, &iter, path_string);
  gtk_tree_model_get(model, &iter,
                     TV_FAM_OLDNAME, &old_name,
                     -1);

  if (!old_name && (g_strcmp0(new_name, "") == 0))
    gtk_tree_store_remove(GTK_TREE_STORE(model), &iter);
  else if (g_strcmp0(new_name, "") == 0) {
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(NULL,
                              GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                    "Family name must not be empty!");
    gtk_window_set_title(GTK_WINDOW(dialog), "Information");
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  } else {
    GtkWidget *label;   
    GtArray *nodes;
    gchar *iter_str;
    gboolean exists;
    iter_str = gtk_tree_model_get_string_from_iter(model, &iter);
    exists = tv_families_check_if_name_exists(ltrgui, new_name, iter_str);
    if (exists) {
      GtkWidget *dialog;
      dialog = gtk_message_dialog_new(NULL,
                                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                      GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                      "Family name already exists!");
      gtk_window_set_title(GTK_WINDOW(dialog), "Information");
      gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
      gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
      column = gtk_tree_view_get_column(GTK_TREE_VIEW(ltrgui->tv_families), 0);
      tmp = gtk_cell_layout_get_cells(GTK_CELL_LAYOUT(column));
      renderer = (GtkCellRenderer*) g_list_nth_data(tmp, 0);
      g_signal_emit_by_name(renderer, "editing-canceled");
    } else {
      char tmp_name[BUFSIZ];
      gtk_tree_model_get(model, &iter,
                         TV_FAM_NODE_ARRAY, &nodes,
                         -1);
      g_snprintf(tmp_name, BUFSIZ, "%s (%lu)", new_name, gt_array_size(nodes));
      gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
                         TV_FAM_CURNAME, tmp_name,
                         -1);
      gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
                         TV_FAM_OLDNAME, new_name,
                         -1);
      gtk_tree_model_get(model, &iter,
                         TV_FAM_TAB_LABEL, &label,
                         -1);
      if (label)
        gtk_label_close_set_text(GTKLABELCLOSE(label), new_name);

      if (!nodes) {
        nodes = gt_array_new(sizeof(GtGenomeNode*));
        gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
                           TV_FAM_NODE_ARRAY, nodes,
                           -1);
      }
      g_free(iter_str);
    }
  }
  column = gtk_tree_view_get_column(GTK_TREE_VIEW(ltrgui->tv_families), 0);
  tmp = gtk_cell_layout_get_cells(GTK_CELL_LAYOUT(column));
  renderer = (GtkCellRenderer*) g_list_nth_data(tmp, 0);
  g_object_set(renderer, "editable", FALSE, NULL);
  g_list_free(tmp);
  g_free(old_name);
}

void tv_families_row_activated(GtkTreeView *treeview,
                               GtkTreePath *path,
                               GT_UNUSED GtkTreeViewColumn *column,
                               GUIData *ltrgui)
{
  GtArray *nodes;
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkNotebook *notebook;
  GtkWidget *tab_label;
  gint nbpage;

  model = gtk_tree_view_get_model(treeview);
  gtk_tree_model_get_iter(model, &iter, path);
  gtk_tree_model_get(model, &iter,
                     TV_FAM_TAB_LABEL, &tab_label,
                     TV_FAM_NODE_ARRAY, &nodes,
                     -1);
  notebook = GTK_NOTEBOOK(ltrgui->nb_families);  

  if (tab_label) {
    nbpage =
          GPOINTER_TO_INT(
           gtk_label_close_get_button_data(GTKLABELCLOSE(tab_label), "nbpage"));
    gtk_notebook_set_current_page(notebook, nbpage);
  } else {
    nb_families_add_tab(model, &iter, nodes, ltrgui);
  }
}

void tv_families_add_family_button_clicked(GT_UNUSED GtkWidget *button,
                                           GUIData *ltrgui)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkTreeStore *store;
  GtkTreeViewColumn *column;
  GtkTreePath *path;
  GtkCellRenderer *renderer;
  GList *tmp;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->tv_families));
  store = GTK_TREE_STORE(model);
  column = gtk_tree_view_get_column(GTK_TREE_VIEW(ltrgui->tv_families), 0);
  tmp = gtk_cell_layout_get_cells(GTK_CELL_LAYOUT(column));
  renderer = (GtkCellRenderer*) g_list_nth_data(tmp, 0);
  g_object_set(renderer, "editable", TRUE, NULL);

  gtk_tree_store_append(store, &iter, NULL);
  gtk_tree_store_set(store, &iter,
                     TV_FAM_NODE_ARRAY, NULL,
                     TV_FAM_TAB_CHILD, NULL,
                     TV_FAM_TAB_LABEL, NULL,                    
                     -1);

  path = gtk_tree_model_get_path(model, &iter);

  gtk_tree_view_set_cursor(GTK_TREE_VIEW(ltrgui->tv_families),
                           path, column, TRUE);
  g_list_free(tmp);
}

void tv_families_init(GUIData *ltrgui)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkTreeStore *store;

  ltrgui->tv_families = gtk_tree_view_new();
  renderer = gtk_cell_renderer_text_new();
  g_signal_connect(G_OBJECT(renderer), "edited",
                   G_CALLBACK(tv_families_cell_edited), ltrgui);
  g_signal_connect(G_OBJECT(renderer), "editing-canceled",
                   G_CALLBACK(tv_families_cell_edit_canceled), ltrgui);
  g_signal_connect(G_OBJECT(renderer), "editing-started",
                   G_CALLBACK(tv_families_cell_edit_started), ltrgui);
  g_signal_connect(G_OBJECT(ltrgui->tv_families), "button-press-event",
                   G_CALLBACK(tv_families_button_pressed), ltrgui);
  g_signal_connect(G_OBJECT(ltrgui->tv_families), "popup-menu",
                   G_CALLBACK(tv_families_on_popup_menu), ltrgui);
  g_signal_connect(G_OBJECT(ltrgui->tv_families), "row-activated",
                   G_CALLBACK(tv_families_row_activated), ltrgui);
  column  = gtk_tree_view_column_new_with_attributes("LTR Families",
                                                     renderer, "text",
                                                     TV_FAM_CURNAME, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrgui->tv_families), column);

  store = gtk_tree_store_new(TV_FAM_N_COLUMS,
                             G_TYPE_POINTER,
                             G_TYPE_POINTER,
                             G_TYPE_POINTER,
                             G_TYPE_STRING,
                             G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW(ltrgui->tv_families),
                          GTK_TREE_MODEL(store));
  g_object_unref(store);

  /* Set <tv_families> as the destination of the Drag-N-Drop operation */
  gtk_drag_dest_set(ltrgui->tv_families,
                    GTK_DEST_DEFAULT_ALL,
                    family_drag_targets,
                    G_N_ELEMENTS(family_drag_targets),
                    GDK_ACTION_COPY|GDK_ACTION_MOVE);
  g_signal_connect(G_OBJECT(ltrgui->tv_families), "drag-motion",
                   G_CALLBACK(tv_families_on_drag_motion), NULL);

  gtk_container_add(GTK_CONTAINER(ltrgui->sw_main), ltrgui->tv_families);
  gtk_widget_show(ltrgui->tv_families);
}
