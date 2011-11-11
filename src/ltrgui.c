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

void create_recently_used_resource(const gchar *filename)
{
  GtkRecentManager *manager;
  GtkRecentData *data;
  static gchar *groups[2] = {GUI_RECENT_GROUP, NULL};
  gchar *uri;

  /* Create a new recently used resource. */
  data = g_slice_new(GtkRecentData);
  data->display_name = NULL;
  data->description = NULL;
  data->mime_type = "text/plain";
  data->app_name = (gchar*) g_get_application_name ();
  data->app_exec = g_strjoin(" ", g_get_prgname (), "%u", NULL);
  data->groups = groups;
  data->is_private = FALSE;
  uri = g_filename_to_uri(filename, NULL, NULL);
  manager = gtk_recent_manager_get_default();
  gtk_recent_manager_add_full(manager, uri, data);
  g_free(uri);
  g_free(data->app_exec);
  g_slice_free(GtkRecentData, data);
}

void free_hash(void *elem)
{
  gt_free(elem);
}

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

  if (gtk_ltr_families_get_nodes(GTK_LTR_FAMILIES(ltrgui->ltr_families))) {
    if (!gtk_ltr_families_get_projectfile(
                                        GTK_LTR_FAMILIES(ltrgui->ltr_families))) {
      dialog = unsaved_changes_dialog(ltrgui, NO_PROJECT_DIALOG);
      response = gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
    } else if (gtk_ltr_families_get_modified(GTK_LTR_FAMILIES(ltrgui->ltr_families))) {
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
#undef GW
  sb_main_init(ltrgui);
  mb_main_init(ltrgui); 

  gtk_window_set_title(GTK_WINDOW(ltrgui->main_window), GUI_NAME);
  ltrgui->ltr_families = gtk_ltr_families_new(ltrgui->sb_main);
  gtk_box_pack_start(GTK_BOX(ltrgui->vbox1_main), ltrgui->ltr_families,
                     TRUE, TRUE, 0);
  ltrgui->ltr_filter = gtk_ltr_filter_new();
  ltrgui->projset = gtk_project_settings_new();
  ltrgui->assistant = NULL;

  gtk_window_set_transient_for(GTK_WINDOW(ltrgui->ltr_filter),
                               GTK_WINDOW(ltrgui->main_window));

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
  g_thread_init(NULL);
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
  gtk_main();

  /* free memory allocated for GUIData struct */
  free_gui(ltrgui);
  /*if (gt_lib_clean())
    return GT_EXIT_PROGRAMMING_ERROR;  programmer error */
  return EXIT_SUCCESS;
}
