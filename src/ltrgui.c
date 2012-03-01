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
#include "ltrgui.h"
#include "menubar.h"
#include "message_strings.h"
#include "statusbar.h"
#include "support.h"

static void free_gui(GUIData *ltrgui)
{
  g_slice_free(GUIData, ltrgui);
}

gboolean main_window_delete_event(GT_UNUSED GtkWidget *widget,
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
  ltrgui->projset = gtk_project_settings_new();
  ltrgui->ltrfams = gtk_ltr_families_new(ltrgui->statusbar,
                                         ltrgui->progressbar,
                                         ltrgui->projset);
  gtk_box_pack_start(GTK_BOX(ltrgui->vbox), ltrgui->ltrfams, TRUE, TRUE, 0);
  ltrgui->ltrfilt = gtk_ltr_filter_new(ltrgui->ltrfams);
  gtk_ltr_families_set_filter_widget(GTK_LTR_FAMILIES(ltrgui->ltrfams),
                                     ltrgui->ltrfilt);
  ltrgui->assistant = NULL;
  gtk_widget_show_all(ltrgui->main_window);
  gtk_widget_hide(ltrgui->progressbar);
}

gint main(gint argc, gchar *argv[])
{
  GUIData *ltrgui;

  /* allocate the memory needed by GUIData */
  ltrgui = g_slice_new(GUIData);
  ltrgui->err = NULL;
  /* initialize libraries */
  if (!g_thread_supported())
    g_thread_init(NULL);
  gdk_threads_init();
  gtk_init(&argc, &argv);
  gt_lib_init();

  init_gui(ltrgui);
  /* show the window */
  gtk_widget_show(ltrgui->main_window);

  /* enter GTK+ main loop */
  gdk_threads_enter();
  gtk_main();
  gdk_threads_leave();
  /* free memory allocated for GUIData struct */
  free_gui(ltrgui);
  /*if (gt_lib_clean())
    return GT_EXIT_PROGRAMMING_ERROR;  programmer error */
  return EXIT_SUCCESS;
}
