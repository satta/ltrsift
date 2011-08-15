#ifndef PROJECT_WIZARD_H
#define PROJECT_WIZARD_H

#include "ltrgui.h"

typedef enum {
  PW_INTRODUCTION = 0,
  PW_SELECTFILES,
  PW_CLASSIFICATION,
  PW_SUMMARY
} ProjectWizardPages;

void pw_init(GUIData *ltrgui);

void pw_reset_defaults(GUIData *ltrgui);

void pw_cancel(GtkAssistant *assistant, GUIData *ltrgui);

gint pw_forward(gint current_page, GUIData *ltrgui);

void pw_file_add_button_clicked(GtkButton *button, GUIData *ltrgui);

void pw_remove_row(GtkTreeRowReference *rowref, GUIData *ltrgui);

void pw_file_remove_button_clicked(GtkButton *button, GUIData *ltrgui);

void pw_update_label_projectname(GUIData *ltrgui);

void pw_filech_projectfolder_selection_changed(GtkFileChooser *chooser,
                                               GUIData *ltrgui);

void pw_entry_projectname_changed(GtkEntry *entry, GUIData *ltrgui);

#endif // PROJECT_WIZARD_H
