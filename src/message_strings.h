#ifndef MESSAGE_STRINGS_H
#define MESSAGE_STRINGS_H

/* support.h */
#define FILE_EXISTS_DIALOG "A file named \"%s\" already exists.\n\nDo you "\
                           "want to replace it?"

#define GUI_NAME       "LTRSift"
#define FAS_PATTERN    ".fas"
#define GFF3_PATTERN   ".gff3"
#define ESQ_PATTERN   ".esq"
#define SQLITE_PATTERN ".sqlite"

#define GFF3_FILTER_PATTERN "*.gff3"
#define ESQ_FILTER_PATTERN  "*.esq"

#define NO_INDEX_DIALOG    "No indexname found. Do you want to select the "\
                           "indename?"
#define SELECT_INDEX       "Select indexname..."
#define EXPORT_ANNO_TEXT   "Export annotation..."
#define EXPORT_SEQS_TEXT   "Export sequences..."
#define CHOOSE_FILEN_ANNO  "Choose filename for annotation file..."
#define CHOOSE_FILEN_SEQS  "Choose filename for sequence file..."
#define CHOOSE_PREFIX_ANNO "Choose prefix for annotation files..."
#define CHOOSE_PREFIX_SEQS "Choose prefix for sequence files..."

#define ATTR_RID       "ID"
#define ATTR_CLUSTID   "clid"
#define ATTR_PFAMN     "name"
#define ATTR_FULLLEN   "flcand"
#define ATTR_LTRFAM    "ltrfam"
#define ATTR_PARENT    "Parent"
#define FNT_PROTEINM   "protein_match"
#define FNT_LTR        "long_terminal_repeat"
#define FNT_REPEATR    "repeat_region"
#define FNT_LLTR       "lLTR"
#define FNT_RLTR       "rLTR"
#define FNT_LTRRETRO   "LTR_retrotransposon"

#define LTR_TOLERANCE  "Allowed LTR length deviation from group median:"
#define LEN_TOLERANCE  "Allowed candidate length deviation from group median:"
#define FLCAND_RESULT  "Found %lu full length candidates (marked with *)"

/* gtk_blastn_params.h */
#define BLASTN_STRAND        "-strand"
#define BLASTN_CULLING       "-culling_limit"
#define BLASTN_BESTHIT_OVERH "-best_hit_overhang"
#define BLASTN_BESTHIT_SCORE "-best_hit_score"
#define BLASTN_MAXT_SEQS     "-max_target_seqs"
#define BLASTN_XDROP_UNG     "-xdrop_ungap"
#define BLASTN_XDROP_GAP     "-xdrop_gap"
#define BLASTN_NO_GREEDY     "-no_greedy"
#define BLASTN_MIN_RAW       "-min_raw_gapped_score"
#define BLASTN_UNGAPPED      "-ungapped"
#define BLASTN_WINDOWS       "-window_size"
#define BLASTN_OFF_DIAG      "-off_diagonal_range"

#define USE_DEFAULT    "Use default?"

/* gtk_blastn_params_refseq.h */
#define FAS_FILTER_PATTERN "*.fas"
#define FASTA_FILTER_PATTERN "*.fasta"
#define FASTA_FILTER_NAME "FASTA files"

/* gtk_ltr_assistant.h */
#define SELECT_GFF3_FILES "Select GFF3 files..."

#define BROWSE_PROJECT "Bro_wse..."
#define BROWSE_INDEX   "Brow_se..."
#define USE_DEFAULT_A  "Use default"
#define NEW_FAM_PREFIX "newfam_"

#define LTRASSI_WINDOW_TITLE "%s - Project Wizard"

/* gtk_ltr_families.h */
#define LTRFAMS_LV_CAPTION_SEQID   "SeqID"
#define LTRFAMS_LV_CAPTION_STRAND  "S"
#define LTRFAMS_LV_CAPTION_START   "Start"
#define LTRFAMS_LV_CAPTION_END     "End"
#define LTRFAMS_LV_CAPTION_LLTRLEN "lLTR length"
#define LTRFAMS_LV_CAPTION_ELEMLEN "Element length"
#define LTRFAMS_TV_CAPTION_INFO    "Attributes"
#define LTRFAMS_TV_CAPTION_TYPE    "Feature name"

#define LTRFAM_DETINFO "Detailed Information"

#define TB_FAMS_ADD       "Add new family"
#define TB_FAMS_REMOVE    "Remove families with less than three members"
#define TB_NB_NEW_FAM     "Search new families for selected candidates"
#define TB_NB_FL_CANDS    "Determine full length candidates for current family"
#define TB_FAMS_REF_MATCH "Match selection against reference sequences"

#define FAMS_RM_DIALOG    "You are about to remove %d family/families. All "\
                          "members (if any) will be unclassified after this "\
                          "action and added to the 'Unclassified' tab.\n\n "\
                          "Are you sure?"
#define FAMS_EMPTY_DIALOG "Family name must not be empty!"
#define FAMS_EXIST_DIALOG "Family name already exists!"
#define NEW_FAM_DIALOG    "Please select at least three candidates for "\
                          "classification"

#define CAND_RM_DIALOG "All selected candidates will be deleted from the "\
                       "project. This action cannot be undone!\nAre you sure?"
#define CAND_UC_DIALOG "All selected members will be unclassified after this "\
                       "operation and added to the 'Unclassified' tab.\n"\
                       "Are you sure?"

#define ONE_FILE_DIALOG  "_One file..."
#define SEP_FILES_DIALOG "_Separate files..."

#define DELETE_SELECTED  "Delete selection"
#define UNCLASS_SELECTED "Unclassify selection"

#define LTR_FAMILIES_FILTER_SELECTION "Filter selection..."
#define LTR_FAMILIES_MERGE_SELECTION  "Merge selection..."
#define LTR_FAMILIES_MATCH_SELECTION  "Match selection..."
#define FAMS_EXPORT_SEQS_ONE  "Export sequences (one file)..."
#define FAMS_EXPORT_SEQS_MULT "Export sequences (multiple files)..."
#define FAMS_EXPORT_ANNO_ONE  "Export annotation (one file)..."
#define FAMS_EXPORT_ANNO_MULT "Export annotation (multiple files)..."
#define FAMS_EXPORT_FLCANDS   "Export _full length candidates only."
#define FAMS_EDIT_NAME   "Edit name"
#define FAMS_REMOVE_SEL  "Remove selection"

#define MAIN_TAB_LABEL "Unclassified"

#define SEQFILE_FOR_REFSEQ "%s/tmp/fam_seqs_for_refseq_match.fas"

/* gtk_ltr_filter.h */
#define LTR_FILTER_TEMPLATE "name        = \"Name of the script here\"\n" \
                            "author      = \"Your name here\"\n" \
                            "version     = \"Script version here\"\n" \
                            "email       = \"Your email here\"\n" \
                            "short_descr = \"Short description here\"\n" \
                            "description = \"Script desription here\"\n\n" \
                            "function filter(gn)\n"\
                            "  return true -- return false\n" \
                            "end"

#define LTR_FILTER_NOT_ADDED_FILES "Some files could not be added!\n" \
                                   "Possible reasons: Missing metadata, " \
                                   "missing function 'filter' or syntax " \
                                   "errors."
#define LTR_FILTER_NOT_SAVED_FILE  "File could not be saved!\n" \
                                   "Possible reasons: Missing metadata, " \
                                   "missing function 'filter' or syntax " \
                                   "errors."
#define LTR_FILTER_UNSAVED_CHANGES "You have unsaved changes. Are you sure " \
                                   "you want to close the window?"
#define LTR_FILTER_DIALOG          "Filtered %lu candidates (out of %lu "\
                                   "selected). %lu were unclassified and %lu "\
                                   "were deleted."

#define LTR_FILTER_AND "All filters have to match (logical AND)"
#define LTR_FILTER_OR  "Any filter has to match (logical OR)"

#define LUA_PATTERN        ".lua"
#define LUA_FILTER_PATTERN "*.lua"

#define LTR_FILTER_ACTION_DELETE_TEXT  "Unclassify/Delete"
#define LTR_FILTER_ACTION_NEW_FAM_TEXT "Create new family"
#define LTR_FILTER_NEW_FAM_NAME        "filtered"

#define LTRFILT_WINDOW_TITLE "%s - Filter"

/* gtk_project_settings */
#define USED_DEFAULT "Used default."
#define PROJSET_WINDOW_TITLE "%s - Project Settings"

/* ltrgui.h */
#define GUI_RECENT_GROUP    "ltrsift"
#define GUI_DIALOG_OPEN     "Open file..."
#define GUI_DIALOG_IMPORT   "Import GFF3 file..."
#define GUI_DIALOG_SAVE_AS  "Save file as..."

/* Information used by gtk_about_dialog */
#define GUI_VERSION         "0.0.1"
#define GUI_COPYRIGHT       "(c) 2011-2012 Sascha Kastens"
#define GUI_COMMENTS        "COMMENT PLACEHOLDER"
#define GUI_LICENSE         "LICENSE PLACEHOLDER"
#define GUI_WEBSITE         "http://www.genometools.org"
#define GUI_LOGO            "../logo.png"

#define UNTITLED_PROJECT "Untitled project"

#define NO_DATA_DIALOG          "No data for export selected!"
#define UNSAVED_CHANGES_DIALOG  "The project data has been modified.\nDo you "\
                                "want to save your changes or discard them?"
#define NO_PROJECT_DIALOG       "The data has not been saved as a project yet."\
                                "\n Do you want to save the data as a project "\
                                "or discard them?"

/* statusbar_main.h */
#define STATUSBAR_CONTEXT            "LTRSift"
#define STATUSBAR_MSG_WELCOME        "Welcome to %s"
#define STATUSBAR_MENU_HINT          "menuhint"
#define STATUSBAR_MENU_HINT_NEW      "Create a new project."
#define STATUSBAR_MENU_HINT_OPEN     "Open an existing project."
#define STATUSBAR_MENU_HINT_SAVE     "Save the current project."
#define STATUSBAR_MENU_HINT_SAVE_AS  "Save the current project " \
                                     "with a different name."
#define STATUSBAR_MENU_HINT_IMPORT   "Import GFF3 annotation file. NOTE: "\
                                     "This file should contain at least "\
                                     "cluster numbers for features."
#define STATUSBAR_MENU_HINT_EGFF3    "Export the complete data as GFF3 "\
                                     "annotation."
#define STATUSBAR_MENU_HINT_FASTA    "Export sequences for the current project."
#define STATUSBAR_MENU_HINT_CLOSE    "Close the current project."
#define STATUSBAR_MENU_HINT_QUIT     "Quit the program."
#define STATUSBAR_MENU_HINT_SETTINGS "View settings for the current project."
#define STATUSBAR_MENU_HINT_FILTER   "Apply filter to the current project."
#define STATUSBAR_MENU_HINT_COLUMNS  "Show/Hide feature columns."
#define STATUSBAR_NUM_OF_CANDS       "Total number of candidates: %lu"

/* Misc */
#define DEFAULT_STYLE "../style/default.style"
#define PREFIX_EXISTS "Prefix already exists.\nPlease choose a "\
                      "different one\nto avoid duplicate entries"
#define START_CLASSIF "Starting classification"
#define MATCH_DIALOG  "Match against reference sequences"
#define ATTENTION     "Attention!"
#define INFORMATION   "Information"

#define MERGE_FAMILIES_DIALOG  "Merge selection"
#define ENTER_NEW_FAMILIE_NAME "Enter name for new family"

#endif
