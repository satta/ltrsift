#ifndef PROJECT_WIZARD_PAGE_BASIC_SETTINGS_H
#define PROJECT_WIZARD_PAGE_BASIC_SETTINGS_H

#include "ltrgui.h"
#include "project_wizard.h"

void pw_basic_files_page_reset_defaults(GUIData *ltrgui);

void pw_choose_projectname_button_clicked(GtkButton *button, GUIData *ltrgui);

void pw_add_gff3_button_clicked(GtkButton *button, GUIData *ltrgui);

void pw_remove_gff3_button_clicked(GtkButton *button, GUIData *ltrgui);

void pw_choose_encseq_button_clicked(GtkButton *button, GUIData *ltrgui);

#endif // PROJECT_WIZARD_PAGE_BASIC_SETTINGS_H
