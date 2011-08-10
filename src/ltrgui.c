#include "ltrgui.h"
#include "statusbar_main.h"
#include "menubar_main.h"
#include "project_wizard.h"
#include "error.h"

gboolean init_gui(GUIData *ltrgui, GError **err)
{
  GtkBuilder *builder;

  builder = gtk_builder_new();

  if (gtk_builder_add_from_file(builder, GUI_FILE, err) == 0) return FALSE;

  /* Get objects from UI */
#define GW(name) LTR_GET_WIDGET(builder, name, ltrgui)
  GW(main_window);
  GW(sb_main);
#undef GW
  sb_main_init(ltrgui);
  mb_main_get_widgets(builder, ltrgui);
  pw_get_widgets(builder, ltrgui);

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

  /* initialize GTK+ libraries */
  gtk_init(&argc, &argv);

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

  return 0;
}
