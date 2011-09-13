#include "ltrgui.h"
#include "statusbar_main.h"
#include "menubar_main.h"
#include "project_wizard.h"
#include "notebook_main_families.h"
#include "treeview_families.h"
#include "error.h"

gboolean init_gui(GUIData *ltrgui, GError **err)
{
  GtkBuilder *builder;

  builder = gtk_builder_new();

  if (gtk_builder_add_from_file(builder, GUI_FILE, err) == 0) return FALSE;

  /* Get objects from UI */
#define GW(name) LTR_GET_WIDGET(builder, name, ltrgui)
  GW(main_window);
  GW(mb_main);
  GW(mb_main_project_new);
  GW(mb_main_project_open);
  GW(mb_main_project_save);
  GW(mb_main_project_save_as);
  GW(mb_main_project_quit);
  GW(sw_main);
  GW(hbox1_main);
  GW(sb_main);
  GW(pw_window);
  GW(pw_label_projectname);
  GW(pw_label_encseq);
  GW(pw_treeview_gff3);
  GW(pw_do_classification_cb);
#undef GW
  sb_main_init(ltrgui);
  mb_main_init(ltrgui);
  tv_families_init(ltrgui);
  pw_init(ltrgui);

  gtk_widget_hide_all(ltrgui->hbox1_main);

  ltrgui->features = gt_hashmap_new(GT_HASH_STRING, NULL, NULL);
  ltrgui->n_features = 0;

  gtk_window_set_transient_for(GTK_WINDOW(ltrgui->pw_window),
                               GTK_WINDOW(ltrgui->main_window));

  gtk_builder_connect_signals(builder, ltrgui);

  g_object_unref(G_OBJECT(builder));

  ltrgui->project_filename = NULL;
  //ltrgui->project_files = NULL;

  return TRUE;
}

gint main(gint argc, gchar *argv[])
{
  GUIData *ltrgui;
  GError *err = NULL;

   /* allocate the memory needed by our GUIData struct */
  ltrgui = g_slice_new(GUIData);

  /* initialize libraries */
  gtk_init(&argc, &argv);
  gt_lib_init();

  if (!init_gui(ltrgui, &err)) {
    fprintf(stderr, "ERROR: %s\n", err->message);
    g_error_free(err);
    g_slice_free(GUIData, ltrgui);
    return 1;
  }
  /* show the window */
  gtk_widget_show(ltrgui->main_window);

  /* enter GTK+ main loop */
  gtk_main();

  /* free memory we allocated for GUIData struct */
  g_slice_free(GUIData, ltrgui);
  /*if (gt_lib_clean())
    return GT_EXIT_PROGRAMMING_ERROR;  programmer error */
  return 0;
}
