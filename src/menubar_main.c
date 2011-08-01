#include "menubar_main.h"

void menubar_main_get_widgets(GtkBuilder *builder, LTRData *ltrgui)
{

#define GW(name) LTR_GET_WIDGET(builder, name, ltrgui)
  GW(menubar_main_project_new);
  GW(menubar_main_project_open);
  GW(menubar_main_project_save);
  GW(menubar_main_project_save_as);
  GW(menubar_main_project_quit);
#undef GW

  g_object_set_data(G_OBJECT(ltrgui->menubar_main_project_new), "menuhint",
                    (gpointer) "Create a new project.");
  g_object_set_data(G_OBJECT(ltrgui->menubar_main_project_open), "menuhint",
                    (gpointer) "Open an existing project.");
  g_object_set_data(G_OBJECT(ltrgui->menubar_main_project_save), "menuhint",
                    (gpointer) "Save current project.");
  g_object_set_data(G_OBJECT(ltrgui->menubar_main_project_save_as), "menuhint",
                    (gpointer) "Save current project.");
  g_object_set_data(G_OBJECT(ltrgui->menubar_main_project_quit), "menuhint",
                    (gpointer) "Quit the application.");
}
