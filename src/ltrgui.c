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
#include "menubar_main.h"
#include "statusbar_main.h"
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
      mb_main_file_save_activate(NULL, ltrgui);
      return TRUE;
      break;
    case GTK_RESPONSE_REJECT:
      break;
    default:
      break;
  }
  return FALSE;
}

static gboolean init_gui(GUIData *ltrgui)
{
  GtkBuilder *builder;

  builder = gtk_builder_new();

  if (gtk_builder_add_from_file(builder, GUI_FILE, &ltrgui->err) == 0)
    return FALSE;

  /* Get objects from UI */
#define GW(name) LTR_GET_WIDGET(builder, name, ltrgui)
  GW(main_window);
  GW(mb_main);
  GW(mb_main_file_new);
  GW(mb_main_file_open);
  GW(mb_main_file_open_recent);
  GW(mb_main_file_save);
  GW(mb_main_file_save_as);
  GW(mb_main_file_import);
  GW(mb_main_file_close);
  GW(mb_main_file_quit);
  GW(mb_main_file_export_gff3);
  GW(mb_main_file_export_fasta);
  GW(mb_main_project_settings);
  GW(mb_main_view_columns);
  GW(vbox1_main);
  GW(sb_main);
  GW(progressbar);
#undef GW
  sb_main_init(ltrgui);
  mb_main_init(ltrgui);

  gtk_window_set_title(GTK_WINDOW(ltrgui->main_window), GUI_NAME);
  ltrgui->projset = gtk_project_settings_new();
  ltrgui->ltrfams = gtk_ltr_families_new(ltrgui->sb_main,
                                         ltrgui->progressbar,
                                         ltrgui->projset);
  gtk_box_pack_start(GTK_BOX(ltrgui->vbox1_main), ltrgui->ltrfams,
                     TRUE, TRUE, 0);
  ltrgui->ltr_filter = NULL; /*gtk_ltr_filter_new();*/
  ltrgui->assistant = NULL;

  gtk_builder_connect_signals(builder, ltrgui);

  g_object_unref(G_OBJECT(builder));

  return TRUE;
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

  if (!init_gui(ltrgui)) {
    fprintf(stderr, "ERROR: %s\n", ltrgui->err->message);
    g_slice_free(GUIData, ltrgui);
    return 1;
  }
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
