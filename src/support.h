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

#ifndef SUPPORT_H
#define SUPPORT_H

#include "ltrgui.h"

#define FILE_EXISTS_DIALOG "A file named \"%s\" already exists.\n\nDo you "\
                           "want to replace it?"

#define DEFAULT_STYLE  "../style/default.style"
#define GUI_NAME       "LTRGui"
#define FAS_PATTERN    ".fas"
#define GFF3_PATTERN   ".gff3"
#define ESQ_PATTERN    ".esq"
#define SQLITE_PATTERN ".sqlite"

void       free_gt_hash_elem(void *elem);

void       create_recently_used_resource(const gchar *filename);

void       reset_progressbar(GtkWidget *progressbar);

GtkWidget* unsaved_changes_dialog(GUIData *ltrgui, const gchar *text);

#endif
