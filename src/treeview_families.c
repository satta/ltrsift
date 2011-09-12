#include "treeview_families.h"
#include "notebook_main_families.h"
#include "unused.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* drag'n'drop crap starts here */
static guint n_targets = 1;

void on_drag_data_get(GtkWidget *widget, G_UNUSED GdkDragContext *drag_context,
                      GtkSelectionData *sdata, G_UNUSED guint info, G_UNUSED guint time,
                      G_UNUSED gpointer user_data)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkTreeSelection *sel;

  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
  gtk_tree_selection_get_selected(sel, &model, &iter);

  struct DATA *tmp = malloc(sizeof(struct DATA));

  gtk_tree_model_get(model, &iter,
                     LTRFAM_LV_SEQID, &tmp->row,
                     LTRFAM_LV_NODE, &tmp->gn,
                     -1);

  gtk_selection_data_set(sdata,
                         gdk_atom_intern("struct DATA pointer", false),
                         8,
                         (void*) &tmp,
                         sizeof(tmp));
}

void on_drag_data_received(G_UNUSED GtkWidget *widget, G_UNUSED GdkDragContext *drag_context,
                        G_UNUSED gint x, G_UNUSED gint y, GtkSelectionData *sdata, G_UNUSED guint info,
                        G_UNUSED guint time, gpointer user_data)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreeSelection *sel;
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(user_data));
  gtk_tree_selection_get_selected(sel,&model,&iter);
  gtk_list_store_remove(GTK_LIST_STORE(model),&iter);

  GtkTreeModel *model2;
  GtkTreeIter iter2;
  model2 = gtk_tree_view_get_model(GTK_TREE_VIEW(widget));
  gtk_tree_store_append(GTK_TREE_STORE(model2), &iter2, NULL);

  struct DATA *tmp = NULL;
  memcpy(&tmp, sdata->data, sizeof(tmp));

  gtk_tree_store_set(GTK_TREE_STORE(model2), &iter2,
                     TV_FAM_NODE, tmp->gn,
                     TV_FAM_NAME, tmp->row,
                     -1);

}
/* drag'n'drop crap ends here */

static void tv_families_popup_menu_edit_clicked(G_UNUSED GtkWidget *menuitem,
                                                gpointer userdata)
{
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  GtkTreeViewColumn *column;
  GtkTreeIter iter;
  GtkTreePath *path;
  GtkCellRenderer *renderer;
  GList *tmp;
  GtkTreeView *treeview = GTK_TREE_VIEW(userdata);

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

void tv_families_refresh_notebook_nums(GtkNotebook *notebook)
{
  GtkWidget *tab_child, *tab_label;
  gint n_pages, i, old_pagenum;
  n_pages = gtk_notebook_get_n_pages(notebook);
  for (i = 0; i < n_pages; i++) {
    tab_child = gtk_notebook_get_nth_page(notebook, i);
    tab_label = gtk_notebook_get_tab_label(notebook, tab_child);
    old_pagenum =
        GPOINTER_TO_INT(
           gtk_label_close_get_button_data(GTKLABELCLOSE(tab_label), "nbpage"));
    if (old_pagenum != i)
      gtk_label_close_set_button_data(GTKLABELCLOSE(tab_label),
                                      "nbpage",
                                      GINT_TO_POINTER(i));
  }
}

static void tv_families_popup_menu_remove_clicked(G_UNUSED GtkWidget *menuitem,
                                                  GUIData *ltrgui)
{
  GtkWidget *dialog;
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  GtkTreeIter iter;
  GtkTreeView *treeview = GTK_TREE_VIEW(ltrgui->tv_families);
  gint iterd;

  model = gtk_tree_view_get_model(treeview);
  sel = gtk_tree_view_get_selection(treeview);
  if (gtk_tree_selection_count_selected_rows(sel) == 0)
    return;
  gtk_tree_selection_get_selected(sel, &model, &iter);
  iterd = gtk_tree_store_iter_depth(GTK_TREE_STORE(model), &iter);

  switch (iterd) {
    case 0:
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
        GtkTreeIter child;
        GtkWidget *tab_label;
        if (gtk_tree_model_iter_children(model, &child, &iter)) {
          while (gtk_tree_model_iter_next(model, &child))
            gtk_tree_store_remove(GTK_TREE_STORE(model), &child);
        }
        gtk_tree_model_get(model, &iter, TV_FAM_TAB_LABEL, &tab_label, -1);
        if (tab_label) {
          gtk_notebook_remove_page(GTK_NOTEBOOK(ltrgui->nb_main_families),
                      GPOINTER_TO_INT(
                       gtk_label_close_get_button_data(GTKLABELCLOSE(tab_label),
                                                       "nbpage")));
          tv_families_refresh_notebook_nums(
                                        GTK_NOTEBOOK(ltrgui->nb_main_families));
        }
        gtk_tree_store_remove(GTK_TREE_STORE(model), &iter);
      }
      gtk_widget_destroy(dialog);
      break;
    case 1:
      g_warning("depth: %d", iterd);
      break;
    default:
      g_warning("depth: %d", iterd);
      break;
  }
}

static void tv_families_popup_menu(GtkWidget *treeview,
                                   GdkEventButton *event,
                                   GUIData *ltrgui)
{
  GtkWidget *menu, *menuitem;
  GtkTreeIter iter;
  gboolean valid;

  valid = gtk_tree_model_get_iter_first(
            gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->tv_families)), &iter);
  menu = gtk_menu_new();
  menuitem = gtk_menu_item_new_with_label("Edit name");
  g_signal_connect(menuitem, "activate",
                   (GCallback) tv_families_popup_menu_edit_clicked, treeview);
  if (!valid)
    gtk_widget_set_sensitive(menuitem, false);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

  menuitem = gtk_menu_item_new_with_label("Remove selected");
  g_signal_connect(menuitem, "activate",
                   (GCallback) tv_families_popup_menu_remove_clicked, ltrgui);
  if (!valid)
    gtk_widget_set_sensitive(menuitem, false);
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
  /* TODO deactivate menu as long as the tree is empty */
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
      }
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

static void tv_families_cell_edit_canceled(G_UNUSED GtkCellRenderer *renderer,
                                         GUIData *ltrgui)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreeSelection *sel;
  gchar *name;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(ltrgui->tv_families));
  sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(ltrgui->tv_families));
  gtk_tree_selection_get_selected(sel, &model, &iter);
  gtk_tree_model_get(model, &iter, TV_FAM_NAME, &name, -1);

  if(!name)
    gtk_tree_store_remove(GTK_TREE_STORE(model), &iter);
  g_free(name);
 }

static void tv_families_cell_edited(G_UNUSED GtkCellRendererText *cell,
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
  gtk_tree_model_get(model, &iter, TV_FAM_NAME, &old_name, -1);

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
    gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
                       TV_FAM_NAME, new_name,
                       -1);
    gtk_tree_model_get(model, &iter,
                       TV_FAM_TAB_LABEL, &label,
                       -1);
    if (label)
      gtk_label_close_set_text(GTKLABELCLOSE(label), new_name);
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
                               G_UNUSED GtkTreeViewColumn *column,
                               GUIData *ltrgui)
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkNotebook *notebook;
  GtkWidget *tab_label;
  gint nbpage;

  model = gtk_tree_view_get_model(treeview);
  gtk_tree_model_get_iter(model, &iter, path);
  gtk_tree_model_get(model, &iter,
                     TV_FAM_TAB_LABEL, &tab_label,
                     -1);
  notebook = GTK_NOTEBOOK(ltrgui->nb_main_families);  

  if (tab_label) {
    nbpage =
          GPOINTER_TO_INT(
           gtk_label_close_get_button_data(GTKLABELCLOSE(tab_label), "nbpage"));
    gtk_notebook_set_current_page(notebook, nbpage);
  } else {
    GtkWidget *child, *label;
    gchar *name;
    child = gtk_ltr_family_new(ltrgui->features, NULL, ltrgui->n_features);
    gtk_widget_show(child);
    gtk_tree_model_get(model, &iter,
                       TV_FAM_NAME, &name,
                       -1);
    label = gtk_label_close_new(name,
                                G_CALLBACK(nb_main_families_close_tab_clicked),
                                ltrgui);
    /*gtk_label_close_hide_close(GTKLABELCLOSE(label));*/
    nbpage = gtk_notebook_append_page(notebook, child, label);
    gtk_label_close_set_button_data(GTKLABELCLOSE(label),
                                    "nbpage",
                                    GINT_TO_POINTER(nbpage));
    gtk_notebook_set_tab_reorderable(notebook, child, TRUE);
    gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
                       TV_FAM_TAB_CHILD, child,
                       TV_FAM_TAB_LABEL, label,
                       -1);
    gtk_notebook_set_current_page(notebook, nbpage);
    g_free(name);
  }
}

void add_family_button_clicked(G_UNUSED GtkWidget *button, GUIData *ltrgui)
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
  g_signal_connect(renderer, "edited",
                   G_CALLBACK(tv_families_cell_edited), ltrgui);
  g_signal_connect(renderer, "editing-canceled",
                   G_CALLBACK(tv_families_cell_edit_canceled), ltrgui);
  g_signal_connect(GTK_TREE_VIEW(ltrgui->tv_families), "button-press-event",
                   G_CALLBACK(tv_families_button_pressed), ltrgui);
  g_signal_connect(GTK_TREE_VIEW(ltrgui->tv_families), "popup-menu",
                   G_CALLBACK(tv_families_on_popup_menu), ltrgui);
  g_signal_connect(G_OBJECT(ltrgui->tv_families), "row-activated",
                   G_CALLBACK(tv_families_row_activated), ltrgui);
  column  = gtk_tree_view_column_new_with_attributes("LTR Families",
                                                     renderer, "text",
                                                     TV_FAM_NAME, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(ltrgui->tv_families), column);

  store = gtk_tree_store_new(TV_FAM_N_COLUMS,
                             G_TYPE_POINTER,
                             G_TYPE_POINTER,
                             G_TYPE_POINTER,
                             G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW(ltrgui->tv_families),
                          GTK_TREE_MODEL(store));
  g_object_unref(store);

  /* Set treeview 2 as the destination of the Drag-N-Drop operation */
  gtk_drag_dest_set(ltrgui->tv_families,GTK_DEST_DEFAULT_ALL,&drag_targets,n_targets,
          GDK_ACTION_COPY|GDK_ACTION_MOVE);

  gtk_container_add(GTK_CONTAINER(ltrgui->sw_main), ltrgui->tv_families);
  gtk_widget_show(ltrgui->tv_families);
}
