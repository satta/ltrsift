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

#ifndef PROJECT_WIZARD_PAGE_BASIC_SETTINGS_H
#define PROJECT_WIZARD_PAGE_BASIC_SETTINGS_H

#include "ltrgui.h"
#include "project_wizard.h"

void pw_page_basic_settings_reset_defaults(GUIData *ltrgui);

void pw_choose_projectname_button_clicked(GtkButton *button, GUIData *ltrgui);

void pw_add_gff3_button_clicked(GtkButton *button, GUIData *ltrgui);

void pw_remove_gff3_button_clicked(GtkButton *button, GUIData *ltrgui);

void pw_choose_encseq_button_clicked(GtkButton *button, GUIData *ltrgui);

void pw_do_clustering_cb_toggled(GtkToggleButton *togglebutton,
                                 GUIData *ltrgui);

#endif /* PROJECT_WIZARD_PAGE_BASIC_SETTINGS_H */
