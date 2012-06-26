/*
  Copyright (c) 2012 Sascha Kastens <sascha.kastens@studium.uni-hamburg.de>
  Copyright (c) 2012 Center for Bioinformatics, University of Hamburg

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

#include "gtk_blastn_params_refseq.h"
#include "message_strings.h"

const gchar*
gtk_blastn_params_refseq_get_refseq_file(GtkBlastnParamsRefseq *blastparref)
{
  return gtk_label_get_text(GTK_LABEL(blastparref->label_refseq));
}

gdouble
gtk_blastn_params_refseq_get_match_len(GtkBlastnParamsRefseq *blastparref)
{
  return gtk_spin_button_get_value(GTK_SPIN_BUTTON(blastparref->spinb_mlen));
}

gboolean
gtk_blastn_params_refseq_get_flcands(GtkBlastnParamsRefseq *blastparref)
{
  return
   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(blastparref->checkb_flcands));
}

void gtk_blastn_params_refseq_set_sensitive(GtkBlastnParamsRefseq *blastparref)
{
  gtk_widget_set_sensitive(blastparref->spinb_mlen, TRUE);
  gtk_blastn_params_set_sensitive(GTK_BLASTN_PARAMS(blastparref));
}

void
gtk_blastn_params_refseq_unset_sensitive(GtkBlastnParamsRefseq *blastparref)
{
  gtk_widget_set_sensitive(blastparref->spinb_mlen, FALSE);
  gtk_blastn_params_unset_sensitive(GTK_BLASTN_PARAMS(blastparref));
}

void gtk_blastn_params_refseq_set_paramset(GtkBlastnParamsRefseq *blastparref,
                                           gdouble evalue,
                                           gboolean dust,
                                           gint gapopen,
                                           gint gapextend,
                                           gdouble xdrop,
                                           gint penalty,
                                           gint reward,
                                           gint threads,
                                           gint wordsize,
                                           gdouble seqid,
                                           const gchar *moreblast,
                                           gdouble mlen)
{
  gtk_blastn_params_set_paramset(GTK_BLASTN_PARAMS(blastparref), evalue, dust,
                                 gapopen, gapextend, xdrop, penalty, reward,
                                 threads, wordsize, seqid, moreblast);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(blastparref->spinb_mlen), mlen);
}

void
gtk_blastn_params_refseq_set_extra_widget(GtkBlastnParamsRefseq *blastparref,
                                          GtkWidget *widget, gint position)
{
  if (blastparref->extra_widget)
    gtk_widget_destroy(blastparref->extra_widget);
  blastparref->extra_widget = widget;
  gtk_box_pack_start(GTK_BOX(blastparref), widget, FALSE, FALSE, 1);
  if (position > -1)
    gtk_box_reorder_child(GTK_BOX(blastparref), widget, position);
  gtk_widget_show_all(widget);
}

static void browse_refseq_clicked(GT_UNUSED GtkWidget *button,
                                  GtkBlastnParamsRefseq *blastparref)
{
  GtkWidget *fc;
  gchar *filename;

  fc = gtk_file_chooser_dialog_new("Select...", NULL,
                                   GTK_FILE_CHOOSER_ACTION_SAVE,
                                   GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                   GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, NULL);
  if (blastparref->last_dir)
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(fc),
                                        blastparref->last_dir);
  GtkFileFilter *fas_file_filter;
  fas_file_filter = gtk_file_filter_new();
  gtk_file_filter_set_name(fas_file_filter, FASTA_FILTER_NAME);
  gtk_file_filter_add_pattern(fas_file_filter, FAS_FILTER_PATTERN);
  gtk_file_filter_add_pattern(fas_file_filter, FASTA_FILTER_PATTERN);
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(fc), fas_file_filter);
  gtk_file_chooser_set_create_folders(GTK_FILE_CHOOSER(fc), FALSE);

  if (gtk_dialog_run(GTK_DIALOG(fc)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fc));
    gtk_label_set_label(GTK_LABEL(blastparref->label_refseq), filename);
    if (blastparref->last_dir)
      g_free(blastparref->last_dir);
    blastparref->last_dir = g_path_get_dirname(filename);
    g_free(filename);
  }
  gtk_widget_destroy(fc);
}

static gboolean gtk_blastn_params_refseq_destroy(GtkWidget *widget,
                                                 GT_UNUSED GdkEvent *event,
                                                 GT_UNUSED gpointer user_data)
{
  GtkBlastnParamsRefseq *blastparref;
  blastparref = GTK_BLASTN_PARAMS_REFSEQ(widget);
  if (blastparref->last_dir)
    g_free(blastparref->last_dir);

  return FALSE;
}

static void gtk_blastn_params_refseq_init(GtkBlastnParamsRefseq *blastparref)
{
  GtkWidget *label, *button, *hbox, *hsep;
  GtkObject *adjust;

  label = gtk_label_new("File containing reference sequences:");
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
  gtk_box_pack_start(GTK_BOX(blastparref), label, FALSE, FALSE, 1);
  gtk_box_reorder_child(GTK_BOX(blastparref), label, 0);
  hbox = gtk_hbox_new(FALSE, 1);
  button = gtk_button_new_with_mnemonic("Bro_wse...");
  blastparref->label_refseq = gtk_label_new("");
  gtk_misc_set_alignment(GTK_MISC(blastparref->label_refseq), 0.0, 0.5);
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(browse_refseq_clicked), blastparref);
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), blastparref->label_refseq, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(blastparref), hbox, FALSE, FALSE, 1);
  gtk_box_reorder_child(GTK_BOX(blastparref), hbox, 1);
  hsep = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(blastparref), hsep, FALSE, FALSE, 1);
  gtk_box_reorder_child(GTK_BOX(blastparref), hsep, 2);

  hbox = gtk_hbox_new(FALSE, 1);
  label = gtk_label_new("% of the candidate seq. a match needs to cover:");
  gtk_misc_set_alignment(GTK_MISC(blastparref->label_refseq), 0.0, 0.5);
  adjust = gtk_adjustment_new(30.0, 0.0, 100.0, 0.1, 1.0, 0.0);
  blastparref->spinb_mlen = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 0.1, 1);
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), blastparref->spinb_mlen, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(blastparref), hbox, FALSE, FALSE, 1);
  blastparref->checkb_flcands =
          gtk_check_button_new_with_label("Match full length candidates only?");
  /* gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(blastparref->checkb_flcands),
                              FALSE); */
  gtk_box_pack_start(GTK_BOX(blastparref), blastparref->checkb_flcands, FALSE,
                     FALSE, 1);
  g_signal_connect(G_OBJECT(blastparref), "destroy",
                   G_CALLBACK(gtk_blastn_params_refseq_destroy), NULL);
}

GType gtk_blastn_params_refseq_get_type(void)
{
  static GType blastn_params_refseq_type = 0;

  if (!blastn_params_refseq_type) {
    const GTypeInfo blastn_params_refseq_info =
    {
      sizeof (GtkBlastnParamsRefseqClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      NULL, /*(GClassInitFunc) */
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (GtkBlastnParamsRefseq),
      0, /* n_preallocs */
      (GInstanceInitFunc) gtk_blastn_params_refseq_init,
    };
    blastn_params_refseq_type = g_type_register_static(GTK_BLASTN_PARAMS_TYPE,
                                                       "GtkBlastnParamsRefseq",
                                                     &blastn_params_refseq_info,
                                                       0);
  }
  return blastn_params_refseq_type;
}

GtkWidget* gtk_blastn_params_refseq_new()
{
  GtkBlastnParamsRefseq *blastparref;
  blastparref = gtk_type_new(GTK_BLASTN_PARAMS_REFSEQ_TYPE);
  blastparref->last_dir = NULL;

  return GTK_WIDGET(blastparref);
}
