#ifndef PROJECT_WIZARD_H
#define PROJECT_WIZARD_H

#include "ltrgui.h"

typedef enum {
  PW_INTRODUCTION = 0,
  PW_SELECTFILES,
  PW_CLASSIFICATION,
  PW_SUMMARY
} ProjectWizardPages;

#define GFF3_PATTERN "*.gff3"
#define PW_SELECT_FILE "Select files..."

void pw_init(GUIData *ltrgui);

void pw_cancel(GtkAssistant *assistant, GUIData *ltrgui);

gint pw_forward(gint current_page, GUIData *ltrgui);

#endif // PROJECT_WIZARD_H
