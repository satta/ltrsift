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

#ifndef STATUSBAR_MAIN_H
#define STATUSBAR_MAIN_H

#include "ltrgui.h"

#define SB_MAIN_CONTEXT            "LTRGui"
#define SB_MAIN_MSG_WELCOME        "Welcome to LTRGui"
#define SB_MAIN_MENU_HINT          "menuhint"
#define SB_MAIN_MENU_HINT_NEW      "Create a new project."
#define SB_MAIN_MENU_HINT_OPEN     "Open an existing project."
#define SB_MAIN_MENU_HINT_SAVE     "Save the current project."
#define SB_MAIN_MENU_HINT_SAVE_AS  "Save the current project " \
                                   "with a different name."
#define SB_MAIN_MENU_HINT_IMPORT   "Import GFF3 annotation file. NOTE: "\
                                   "This file should contain at least cluster "\
                                   "numbers for features."
#define SB_MAIN_MENU_HINT_EGFF3    "Export the complete data as GFF3 "\
                                   "annotation."
#define SB_MAIN_MENU_HINT_FASTA    "Export sequences for the current project."
#define SB_MAIN_MENU_HINT_CLOSE    "Close the current project."
#define SB_MAIN_MENU_HINT_QUIT     "Quit the program."
#define SB_MAIN_MENU_HINT_SETTINGS "View settings for the current project."
#define SB_MAIN_MENU_HINT_COLUMNS  "Show/Hide feature columns."

void sb_main_init(GUIData *ltrgui);

void sb_main_set_status(GUIData *ltrgui, gchar *status_msg);

gboolean sb_main_menuhints(GtkMenuItem *menuitem, GdkEvent *event,
                               GUIData *ltrgui);

#endif
