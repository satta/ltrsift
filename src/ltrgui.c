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

#include "error.h"
#include "ltrgui.h"
#include "menubar_main.h"
#include "project_wizard.h"
#include "statusbar_main.h"

void free_hash(void *elem)
{
  gt_free(elem);
}

static void free_gui(GUIData *ltrgui)
{
  /*int i;

  for (i = 0; i < gt_array_size(ltrgui->nodes); i++) {
    gt_genome_node_delete(*(GtGenomeNode**) gt_array_get(ltrgui->nodes, i));
  }
  gt_array_delete(ltrgui->nodes);
  gt_hashmap_delete(ltrgui->features);*/
  g_slice_free(GUIData, ltrgui);
}

gboolean init_gui(GUIData *ltrgui, GError **err)
{
  GtkBuilder *builder;

  builder = gtk_builder_new();

  if (gtk_builder_add_from_file(builder, GUI_FILE, err) == 0)
    return FALSE;

  /* Get objects from UI */
#define GW(name) LTR_GET_WIDGET(builder, name, ltrgui)
  GW(main_window);
  GW(mb_main);
  GW(mb_main_project_new);
  GW(mb_main_project_open);
  GW(mb_main_project_save);
  GW(mb_main_project_save_as);
  GW(mb_main_project_quit);
  GW(sw_main);
  GW(vbox1_main);
  GW(sb_main);
  GW(pw_window);
  GW(pw_label_projectname);
  GW(pw_label_encseq);
  GW(pw_treeview_gff3);
  GW(pw_do_classification_cb);
#undef GW
  sb_main_init(ltrgui);
  mb_main_init(ltrgui); 
  pw_init(ltrgui);

  ltrgui->ltr_families = gtk_ltr_families_new();
  gtk_box_pack_start(GTK_BOX(ltrgui->vbox1_main), ltrgui->ltr_families,
                     TRUE, TRUE, 0);

  gtk_window_set_transient_for(GTK_WINDOW(ltrgui->pw_window),
                               GTK_WINDOW(ltrgui->main_window));

  gtk_builder_connect_signals(builder, ltrgui);

  g_object_unref(G_OBJECT(builder));

  ltrgui->project_filename = NULL;
  //ltrgui->project_files = NULL;

  return TRUE;
}

gint main(gint argc, gchar *argv[])
{
  GUIData *ltrgui;
  GError *err = NULL;

   /* allocate the memory needed by GUIData */
  ltrgui = g_slice_new(GUIData);

  /* initialize libraries */
  gtk_init(&argc, &argv);
  gt_lib_init();

  if (!init_gui(ltrgui, &err)) {
    fprintf(stderr, "ERROR: %s\n", err->message);
    g_error_free(err);
    g_slice_free(GUIData, ltrgui);
    return 1;
  }
  /* show the window */
  gtk_widget_show(ltrgui->main_window);

  /* enter GTK+ main loop */
  gtk_main();

  /* free memory allocated for GUIData struct */
  free_gui(ltrgui);
  /*if (gt_lib_clean())
    return GT_EXIT_PROGRAMMING_ERROR;  programmer error */
  return 0;
}
