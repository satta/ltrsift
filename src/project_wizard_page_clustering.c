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

#include "project_wizard_page_clustering.h"

void pw_do_classification_cb_toggled(GtkToggleButton *togglebutton,
                                     GUIData *ltrgui)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_label_set_label(GTK_LABEL(ltrgui->pw_label_classification),
                      active ? "yes" : "no");
}

void pw_spinbutton_psmall_value_changed(GtkSpinButton *sbutton, GUIData *ltrgui)
{
  gchar buf[4];
  g_snprintf(buf, 4, "%d", gtk_spin_button_get_value_as_int(sbutton));
  gtk_label_set_label(GTK_LABEL(ltrgui->pw_label_psmall), buf);
}

void pw_spinbutton_plarge_value_changed(GtkSpinButton *sbutton, GUIData *ltrgui)
{
  gchar buf[4];
  g_snprintf(buf, 4, "%d", gtk_spin_button_get_value_as_int(sbutton));
  gtk_label_set_label(GTK_LABEL(ltrgui->pw_label_plarge), buf);
}

void pw_spinbutton_xdrop_value_changed(GtkSpinButton *sbutton, GUIData *ltrgui)
{
  gchar buf[4];
  g_snprintf(buf, 4, "%d", gtk_spin_button_get_value_as_int(sbutton));
  gtk_label_set_label(GTK_LABEL(ltrgui->pw_label_xdrop), buf);
}

void pw_spinbutton_words_value_changed(GtkSpinButton *sbutton, GUIData *ltrgui)
{
  gchar buf[4];
  g_snprintf(buf, 4, "%d", gtk_spin_button_get_value_as_int(sbutton));
  gtk_label_set_label(GTK_LABEL(ltrgui->pw_label_words), buf);
}

void pw_spinbutton_seqid_value_changed(GtkSpinButton *sbutton, GUIData *ltrgui)
{
  gchar buf[6];
  g_snprintf(buf, 6, "%.1f", gtk_spin_button_get_value(sbutton));
  gtk_label_set_label(GTK_LABEL(ltrgui->pw_label_seqid), buf);
}
