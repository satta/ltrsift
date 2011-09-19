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

#include "project_wizard.h"
#include "project_wizard_page_basic_settings.h"
#include "statusbar_main.h"

void pw_init(GUIData *ltrgui)
{
  gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(
                                                  ltrgui->pw_treeview_gff3)),
                                                        GTK_SELECTION_MULTIPLE);

  gtk_assistant_set_forward_page_func(GTK_ASSISTANT(ltrgui->pw_window),
                               (GtkAssistantPageFunc) pw_forward, ltrgui, NULL);
}

void pw_reset_defaults(GUIData *ltrgui)
{
  pw_basic_files_page_reset_defaults(ltrgui);
  gtk_assistant_set_page_complete(GTK_ASSISTANT(ltrgui->pw_window),
      gtk_assistant_get_nth_page(GTK_ASSISTANT(ltrgui->pw_window),
                                                        PW_SELECTFILES), FALSE);
  gtk_assistant_set_page_complete(GTK_ASSISTANT(ltrgui->pw_window),
      gtk_assistant_get_nth_page(GTK_ASSISTANT(ltrgui->pw_window),
                                                     PW_CLASSIFICATION), FALSE);
}

void pw_cancel(GtkAssistant *assistant, GUIData *ltrgui)
{
  gtk_widget_hide(GTK_WIDGET(assistant));
  sb_main_set_status(ltrgui, "Welcome to LTRGui");
  pw_reset_defaults(ltrgui);
}

gint pw_forward(gint current_page, GUIData *ltrgui)
{
  gint next_page;

  switch (current_page) {
    case PW_INTRODUCTION:
      next_page = PW_SELECTFILES;
      break;
    case PW_SELECTFILES:
      next_page = (gtk_toggle_button_get_active(
                   GTK_TOGGLE_BUTTON(ltrgui->pw_do_classification_cb))
                                              ? PW_CLASSIFICATION : PW_SUMMARY);
      break;
    case PW_CLASSIFICATION:
      next_page = PW_SUMMARY;
      break;
    default:
      next_page = -1;
  }
  return next_page;
}
