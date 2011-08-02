#include "assistant_project.h"

void assistant_project_cancel(GtkAssistant *assistant, gpointer *data)
{
  gtk_widget_hide(GTK_WIDGET(assistant));
  /* TODO: -reset assistant to default */
}
