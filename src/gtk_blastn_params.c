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

#include "gtk_blastn_params.h"
#include "message_strings.h"

gdouble gtk_blastn_params_get_evalue(GtkBlastnParams *blastpar)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(blastpar->checkb_evalue)))
    return GT_UNDEF_DOUBLE;
  return gtk_spin_button_get_value(GTK_SPIN_BUTTON(blastpar->spinb_evalue));
}

gboolean gtk_blastn_params_get_dust(GtkBlastnParams *blastpar)
{
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(blastpar->checkb_dust));
}

gint gtk_blastn_params_get_gapopen(GtkBlastnParams *blastpar)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(blastpar->checkb_gapopen)))
    return GT_UNDEF_INT;
  return
     gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(blastpar->spinb_gapopen));
}

gint gtk_blastn_params_get_gapextend(GtkBlastnParams *blastpar)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(
                                     blastpar->checkb_gapextend)))
    return GT_UNDEF_INT;
  return
   gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(blastpar->spinb_gapextend));
}

gdouble gtk_blastn_params_get_xdrop(GtkBlastnParams *blastpar)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(blastpar->checkb_xdrop)))
    return GT_UNDEF_DOUBLE;
  return gtk_spin_button_get_value(GTK_SPIN_BUTTON(blastpar->spinb_xdrop));
}

gint gtk_blastn_params_get_penalty(GtkBlastnParams *blastpar)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(blastpar->checkb_penalty)))
    return GT_UNDEF_INT;
  return
     gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(blastpar->spinb_penalty));
}

gint gtk_blastn_params_get_reward(GtkBlastnParams *blastpar)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(blastpar->checkb_reward)))
    return GT_UNDEF_INT;
  return
      gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(blastpar->spinb_reward));
}

gint gtk_blastn_params_get_threads(GtkBlastnParams *blastpar)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(blastpar->checkb_threads)))
    return GT_UNDEF_INT;
  return
     gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(blastpar->spinb_threads));
}

gint gtk_blastn_params_get_wordsize(GtkBlastnParams *blastpar)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(blastpar->checkb_words)))
    return GT_UNDEF_INT;
  return
      gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(blastpar->spinb_words));
}

gdouble gtk_blastn_params_get_seqid(GtkBlastnParams *blastpar)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(blastpar->checkb_seqid)))
    return GT_UNDEF_DOUBLE;
  return gtk_spin_button_get_value(GTK_SPIN_BUTTON(blastpar->spinb_seqid));
}

const gchar* gtk_blastn_params_get_moreblast(GtkBlastnParams *blastpar)
{
  return gtk_label_get_text(GTK_LABEL(blastpar->label_moreblast));
}

void gtk_blastn_params_set_paramset(GtkBlastnParams *blastpar, gdouble evalue,
                                    gboolean dust, gint gapopen, gint gapextend,
                                    gdouble xdrop, gint penalty, gint reward,
                                    gint threads, gint wordsize, gdouble seqid,
                                    const gchar *moreblast)
{
  if (evalue == GT_UNDEF_DOUBLE)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(blastpar->checkb_evalue),
                                 TRUE);
  else {
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(blastpar->spinb_evalue), evalue);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(blastpar->checkb_evalue),
                                 FALSE);
  }
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(blastpar->checkb_dust), dust);
  if (gapopen == GT_UNDEF_INT)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(blastpar->checkb_gapopen),
                                 TRUE);
  else {
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(blastpar->spinb_gapopen),
                              gapopen);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(blastpar->checkb_gapopen),
                                 FALSE);
  }
  if (gapextend == GT_UNDEF_INT)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(blastpar->checkb_gapextend),
                                 TRUE);
  else {
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(blastpar->spinb_gapextend),
                              gapextend);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(blastpar->checkb_gapextend),
                                 FALSE);
  }
  if (xdrop == GT_UNDEF_DOUBLE)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(blastpar->checkb_xdrop),
                                 TRUE);
  else {
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(blastpar->spinb_xdrop), xdrop);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(blastpar->checkb_xdrop),
                                 FALSE);
  }
  if (penalty == GT_UNDEF_INT)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(blastpar->checkb_penalty),
                                 TRUE);
  else {
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(blastpar->spinb_penalty),
                              penalty);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(blastpar->checkb_penalty),
                                 FALSE);
  }
  if (reward == GT_UNDEF_INT)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(blastpar->checkb_reward),
                                 TRUE);
  else {
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(blastpar->spinb_reward), reward);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(blastpar->checkb_reward),
                                 FALSE);
  }
  if (threads == GT_UNDEF_INT)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(blastpar->checkb_threads),
                                 TRUE);
  else {
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(blastpar->spinb_threads),
                              threads);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(blastpar->checkb_threads),
                                 FALSE);
  }
  if (wordsize == GT_UNDEF_INT)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(blastpar->checkb_words),
                                 TRUE);
  else {
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(blastpar->spinb_words), wordsize);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(blastpar->checkb_words),
                                 FALSE);
  }
  if (seqid == GT_UNDEF_DOUBLE)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(blastpar->checkb_seqid),
                                 TRUE);
  else {
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(blastpar->spinb_seqid), seqid);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(blastpar->checkb_seqid),
                                 FALSE);
  }
  gtk_label_set_text(GTK_LABEL(blastpar->label_moreblast), moreblast);
}

static void checkb_evalue_toggled(GtkToggleButton *togglebutton,
                                   GtkBlastnParams *blastpar)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(blastpar->spinb_evalue, !active);
}

static void checkb_gapopen_toggled(GtkToggleButton *togglebutton,
                                   GtkBlastnParams *blastpar)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(blastpar->spinb_gapopen, !active);
}

static void checkb_gapextend_toggled(GtkToggleButton *togglebutton,
                                   GtkBlastnParams *blastpar)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(blastpar->spinb_gapextend, !active);
}

static void checkb_xdrop_toggled(GtkToggleButton *togglebutton,
                                 GtkBlastnParams *blastpar)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(blastpar->spinb_xdrop, !active);
}

static void checkb_penalty_toggled(GtkToggleButton *togglebutton,
                                   GtkBlastnParams *blastpar)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(blastpar->spinb_penalty, !active);
}

static void checkb_reward_toggled(GtkToggleButton *togglebutton,
                                   GtkBlastnParams *blastpar)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(blastpar->spinb_reward, !active);
}

static void checkb_threads_toggled(GtkToggleButton *togglebutton,
                                   GtkBlastnParams *blastpar)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(blastpar->spinb_threads, !active);
}

static void checkb_words_toggled(GtkToggleButton *togglebutton,
                                   GtkBlastnParams *blastpar)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(blastpar->spinb_words, !active);
}

static void checkb_seqid_toggled(GtkToggleButton *togglebutton,
                                   GtkBlastnParams *blastpar)
{
  gboolean active;
  active = gtk_toggle_button_get_active(togglebutton);
  gtk_widget_set_sensitive(blastpar->spinb_seqid, !active);
}

static void build_string(gpointer k, gpointer v, gpointer data)
{
  GString *string = (GString*) data;
  gchar *key = (gchar*) k,
        *value = (gchar*) v,
        *buffer;

  g_string_append(string, " ");
  buffer = g_strjoin(" ", key, value, NULL);
  g_string_append(string, buffer);
  g_free(buffer);
}

static void update_label_moreblast(GtkBlastnParams *blastpar)
{
  GString *string;
  gchar *buffer;

  string = g_string_new("");

  g_hash_table_foreach(blastpar->hasht_blastparams, (GHFunc) build_string,
                       (gpointer) string);
  buffer = g_string_free(string, FALSE);
  gtk_label_set_text(GTK_LABEL(blastpar->label_moreblast), buffer);
  g_free(buffer);
}

static void gtk_blastn_params_combob_blast_changed(GtkComboBox *combob,
                                                   GtkBlastnParams *blastpar)
{
  gchar *param;
  gpointer value;

  param = gtk_combo_box_get_active_text(combob);
  if (g_strcmp0(param, "") == 0) {
    gtk_widget_set_sensitive(blastpar->entry_blastvalues, FALSE);
    gtk_widget_set_sensitive(blastpar->button_addblastparam, FALSE);
    gtk_widget_set_sensitive(blastpar->button_rmblastparam, FALSE);
    g_free(param);
    return;
  }
  gtk_widget_set_sensitive(blastpar->entry_blastvalues, TRUE);
  gtk_widget_set_sensitive(blastpar->button_addblastparam, TRUE);
  gtk_widget_set_sensitive(blastpar->button_rmblastparam, TRUE);

  value = g_hash_table_lookup(blastpar->hasht_blastparams,
                              (gconstpointer) param);
  if (value)
    gtk_entry_set_text(GTK_ENTRY(blastpar->entry_blastvalues), (gchar*) value);
  else
    gtk_entry_set_text(GTK_ENTRY(blastpar->entry_blastvalues), "");
  g_free(param);
}

static void
gtk_blastn_params_add_blast_param_clicked(GT_UNUSED GtkButton *button,
                                          GtkBlastnParams *blastpar)
{
  gchar *key;
  const gchar *value;

  key =
     gtk_combo_box_get_active_text(GTK_COMBO_BOX(blastpar->combob_blastparams));
  value = gtk_entry_get_text(GTK_ENTRY(blastpar->entry_blastvalues));
  if ((g_strcmp0(key, BLASTN_NO_GREEDY) == 0) ||
      (g_strcmp0(key, BLASTN_UNGAPPED) == 0))
    value = "";
  else if (g_strcmp0(value, "") == 0) {
    g_free(key);
    return;
  }
  g_hash_table_insert(blastpar->hasht_blastparams,
                      (gpointer) key, (gpointer) g_strdup(value));
  update_label_moreblast(blastpar);
}

static void
gtk_blastn_params_rm_blast_param_clicked(GT_UNUSED GtkButton *button,
                                         GtkBlastnParams *blastpar)
{
  gchar *key;

  key =
     gtk_combo_box_get_active_text(GTK_COMBO_BOX(blastpar->combob_blastparams));
  if (!g_hash_table_remove(blastpar->hasht_blastparams, (gconstpointer) key)) {
    g_free(key);
    return;
  }
  gtk_entry_set_text(GTK_ENTRY(blastpar->entry_blastvalues), "");
  update_label_moreblast(blastpar);
  g_free(key);
}

void gtk_blastn_params_set_extra_widget(GtkBlastnParams *blastpar,
                                        GtkWidget *widget)
{
  if (blastpar->extra_widget)
    gtk_widget_destroy(blastpar->extra_widget);
  blastpar->extra_widget = widget;
  gtk_box_pack_start(GTK_BOX(blastpar), widget, FALSE, FALSE, 1);
  gtk_widget_show_all(widget);
}

GtkWidget* gtk_blastn_params_get_extra_widget(GtkBlastnParams *blastpar)
{
  return blastpar->extra_widget;
}

static gboolean gtk_blastn_params_destroy(GtkWidget *widget,
                                          GT_UNUSED GdkEvent *event,
                                          GT_UNUSED gpointer user_data)
{
  GtkBlastnParams *blastpar;
  blastpar = GTK_BLASTN_PARAMS(widget);

  g_hash_table_destroy(blastpar->hasht_blastparams);

  return FALSE;
}

static void gtk_blastn_params_init(GtkBlastnParams *blastpar)
{
  GtkWidget *hbox, *vbox, *label, *tmpbox, *checkb, *hsep, *combob,
            *align, *image, *button;
  GtkObject *adjust;

  gtk_box_set_spacing(GTK_BOX(blastpar), 5);

  hbox = gtk_hbox_new(FALSE, 5);
  vbox = gtk_vbox_new(TRUE, 1);
  label = gtk_label_new("Expectation value (E) threshold:");
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  label = gtk_label_new("Filter query sequence with DUST?");
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  label = gtk_label_new("Cost to open a gap:");
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  label = gtk_label_new("Cost to extend a gap:");
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  label = gtk_label_new("X-dropoff value (in bits):");
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  label = gtk_label_new("Penalty for a nucleotide mismatch:");
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  label = gtk_label_new("Reward for a nucleotide match:");
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  label = gtk_label_new("Number of threads (CPUs) used:");
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  label = gtk_label_new("Word size for wordfinder algorithm:");
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  label = gtk_label_new("Percent identity:");
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  label = gtk_label_new("Select additional BLASTN parameter:");
  gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  combob = blastpar->combob_blastparams = gtk_combo_box_new_text();
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), "");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_STRAND);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_CULLING);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_BESTHIT_OVERH);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_BESTHIT_SCORE);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_MAXT_SEQS);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_XDROP_UNG);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_XDROP_GAP);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_NO_GREEDY);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_MIN_RAW);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_UNGAPPED);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_WINDOWS);
  gtk_combo_box_append_text(GTK_COMBO_BOX(combob), BLASTN_OFF_DIAG);
  g_signal_connect(G_OBJECT(combob), "changed",
                   G_CALLBACK(gtk_blastn_params_combob_blast_changed),
                   blastpar);
  gtk_box_pack_start(GTK_BOX(vbox), combob, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 1);

  vbox = gtk_vbox_new(TRUE, 1);
  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(10.0, 0.0001, 50.0, 0.0001, 0.1, 0.0);
  blastpar->spinb_evalue = gtk_spin_button_new(GTK_ADJUSTMENT(adjust),
                                              0.0001, 4);
  gtk_widget_set_sensitive(blastpar->spinb_evalue, FALSE);
  checkb =
         blastpar->checkb_evalue = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_evalue_toggled), (gpointer) blastpar);
  gtk_box_pack_start(GTK_BOX(tmpbox), blastpar->spinb_evalue, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(FALSE, 1);
  blastpar->checkb_dust = gtk_check_button_new();
  align = gtk_alignment_new(0.0, 0.5, 0.0, 0.0);
  gtk_container_add(GTK_CONTAINER(align), blastpar->checkb_dust);
  gtk_box_pack_start(GTK_BOX(tmpbox), align, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(0.0, 0.0, 100.0, 1.0, 10.0, 0.0);
  blastpar->spinb_gapopen = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  gtk_widget_set_sensitive(blastpar->spinb_gapopen, FALSE);
  checkb =
        blastpar->checkb_gapopen = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_gapopen_toggled), (gpointer) blastpar);
  gtk_box_pack_start(GTK_BOX(tmpbox), blastpar->spinb_gapopen, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(0.0, 0.0, 100.0, 1.0, 10.0, 0.0);
  blastpar->spinb_gapextend = gtk_spin_button_new(GTK_ADJUSTMENT(adjust),
                                                 1.0, 0);
  gtk_widget_set_sensitive(blastpar->spinb_gapextend, FALSE);
  checkb =
      blastpar->checkb_gapextend = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_gapextend_toggled),
                   (gpointer) blastpar);
  gtk_box_pack_start(GTK_BOX(tmpbox), blastpar->spinb_gapextend,
                     FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(20.0, 5.0, 100.0, 1.0, 10.0, 0.0);
  blastpar->spinb_xdrop = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  gtk_widget_set_sensitive(blastpar->spinb_xdrop, FALSE);
  checkb =
          blastpar->checkb_xdrop = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_xdrop_toggled), (gpointer) blastpar);
  gtk_box_pack_start(GTK_BOX(tmpbox), blastpar->spinb_xdrop, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(0.0, -100.0, 0.0, -1.0, -10.0, 0.0);
  blastpar->spinb_penalty =
                           gtk_spin_button_new(GTK_ADJUSTMENT(adjust), -1.0, 0);
  gtk_widget_set_sensitive(blastpar->spinb_penalty, FALSE);
  checkb =
        blastpar->checkb_penalty = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_penalty_toggled), (gpointer) blastpar);
  gtk_box_pack_start(GTK_BOX(tmpbox), blastpar->spinb_penalty, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(0.0, 0.0, 100.0, 1.0, 10.0, 0.0);
  blastpar->spinb_reward = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  gtk_widget_set_sensitive(blastpar->spinb_reward, FALSE);
  checkb =
         blastpar->checkb_reward = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_reward_toggled), (gpointer) blastpar);
  gtk_box_pack_start(GTK_BOX(tmpbox), blastpar->spinb_reward, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(1.0, 1.0, 8.0, 1.0, 2.0, 0.0);
  blastpar->spinb_threads = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  gtk_widget_set_sensitive(blastpar->spinb_threads, FALSE);
  checkb =
        blastpar->checkb_threads = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_threads_toggled), (gpointer) blastpar);
  gtk_box_pack_start(GTK_BOX(tmpbox), blastpar->spinb_threads, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(4.0, 4.0, 20.0, 1.0, 4.0, 0.0);
  blastpar->spinb_words = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1.0, 0);
  gtk_widget_set_sensitive(blastpar->spinb_words, FALSE);
  checkb =
          blastpar->checkb_words = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_words_toggled), (gpointer) blastpar);
  gtk_box_pack_start(GTK_BOX(tmpbox), blastpar->spinb_words, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(TRUE, 1);
  adjust = gtk_adjustment_new(80.0, 30.0, 100.0, 0.1, 1.0, 0.0);
  blastpar->spinb_seqid = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 0.1, 1);
  gtk_widget_set_sensitive(blastpar->spinb_seqid, FALSE);
  checkb =
          blastpar->checkb_seqid = gtk_check_button_new_with_label(USE_DEFAULT);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkb), TRUE);
  g_signal_connect(G_OBJECT(checkb), "toggled",
                   G_CALLBACK(checkb_seqid_toggled), (gpointer) blastpar);
  gtk_box_pack_start(GTK_BOX(tmpbox), blastpar->spinb_seqid, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(tmpbox), checkb, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  label = gtk_label_new("Value for additional BLASTN paramter:");
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  tmpbox = gtk_hbox_new(FALSE, 1);
  blastpar->entry_blastvalues = gtk_entry_new();
  gtk_widget_set_sensitive(blastpar->entry_blastvalues, FALSE);
  gtk_box_pack_start(GTK_BOX(tmpbox), blastpar->entry_blastvalues,
                     TRUE, TRUE, 1);
  image = gtk_image_new_from_stock(GTK_STOCK_ADD, GTK_ICON_SIZE_SMALL_TOOLBAR);
  button = blastpar->button_addblastparam = gtk_button_new();
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(gtk_blastn_params_add_blast_param_clicked),
                   blastpar);
  gtk_widget_set_sensitive(button, FALSE);
  gtk_button_set_image(GTK_BUTTON(button), image);
  gtk_box_pack_start(GTK_BOX(tmpbox), button, FALSE, FALSE, 1);
  image = gtk_image_new_from_stock(GTK_STOCK_REMOVE,
                                   GTK_ICON_SIZE_SMALL_TOOLBAR);
  button = blastpar->button_rmblastparam = gtk_button_new();
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(gtk_blastn_params_rm_blast_param_clicked),
                   blastpar);
  gtk_widget_set_sensitive(button, FALSE);
  gtk_button_set_image(GTK_BUTTON(button), image);
  gtk_box_pack_start(GTK_BOX(tmpbox), button, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), tmpbox, FALSE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 1);
  hsep = gtk_hseparator_new();
  gtk_box_pack_start(GTK_BOX(blastpar),
                     hbox, FALSE, FALSE, 1);
  blastpar->label_moreblast = gtk_label_new("");
  gtk_label_set_line_wrap(GTK_LABEL(blastpar->label_moreblast), TRUE);
  gtk_label_set_line_wrap_mode(GTK_LABEL(blastpar->label_moreblast),
                               PANGO_WRAP_WORD_CHAR);
  gtk_misc_set_alignment(GTK_MISC(blastpar->label_moreblast), 0.0, 0.5);
  gtk_box_pack_start(GTK_BOX(blastpar),
                     blastpar->label_moreblast, TRUE, FALSE, 1);
  gtk_box_pack_start(GTK_BOX(blastpar),
                     hsep, FALSE, FALSE, 1);
  blastpar->hasht_blastparams = g_hash_table_new_full(g_str_hash, g_str_equal,
                                                      g_free, g_free);
}

GType gtk_blastn_params_get_type(void)
{
  static GType blastn_params_type = 0;

  if (!blastn_params_type) {
    const GTypeInfo blastn_params_info =
    {
      sizeof (GtkBlastnParamsClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      NULL, /*(GClassInitFunc) */
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (GtkBlastnParams),
      0, /* n_preallocs */
      (GInstanceInitFunc) gtk_blastn_params_init,
    };
    blastn_params_type = g_type_register_static(GTK_TYPE_VBOX,
                                                "GtkBlastnParams",
                                                &blastn_params_info, 0);
  }
  return blastn_params_type;
}

GtkWidget* gtk_blastn_params_new()
{
  GtkBlastnParams *blastpar;
  blastpar = gtk_type_new(GTK_BLASTN_PARAMS_TYPE);
  g_signal_connect(G_OBJECT(blastpar), "destroy",
                   G_CALLBACK(gtk_blastn_params_destroy), NULL);
  return GTK_WIDGET(blastpar);
}
