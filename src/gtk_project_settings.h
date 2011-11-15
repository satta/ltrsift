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

#ifndef __GTK_PROJECT_SETTINGS_H__
#define __GTK_PROJECT_SETTINGS_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include "genometools.h"

#define GTK_PROJECT_SETTINGS_TYPE\
        gtk_project_settings_get_type()
#define GTK_PROJECT_SETTINGS(obj)\
        G_TYPE_CHECK_INSTANCE_CAST((obj), GTK_PROJECT_SETTINGS_TYPE,\
                                   GtkProjectSettings)
#define GTK_PROJECT_SETTINGS_CLASS(klass)\
        G_TYPE_CHECK_CLASS_CAST((klass), GTK_PROJECT_SETTINGS_TYPE,\
                                GtkProjectSettingsClass)
#define IS_GTK_PROJECT_SETTINGS(obj)\
        G_TYPE_CHECK_INSTANCE_TYPE((obj), GTK_PROJECT_SETTINGS_TYPE)
#define IS_GTK_PROJECT_SETTINGS_CLASS(klass)\
        G_TYPE_CHECK_CLASS_TYPE((klass), GTK_PROJECT_SETTINGS_TYPE)

#define USED_DEFAULT "Used default."

typedef struct _GtkProjectSettings GtkProjectSettings;
typedef struct _GtkProjectSettingsClass GtkProjectSettingsClass;

struct _GtkProjectSettings
{
  GtkWindow window;
  GtkWidget *label_projectfile;
  GtkWidget *label_gff3files;
  GtkWidget *label_indexname;
  GtkWidget *label_didclustering;
  GtkWidget *label_evalue;
  GtkWidget *label_dust;
  GtkWidget *label_gapopen;
  GtkWidget *label_gapextend;
  GtkWidget *label_xdrop;
  GtkWidget *label_penalty;
  GtkWidget *label_reward;
  GtkWidget *label_threads;
  GtkWidget *label_wordsize;
  GtkWidget *label_seqidentity;
  GtkWidget *label_moreblast;
  GtkWidget *label_psmall;
  GtkWidget *label_plarge;
  GtkWidget *label_didclassification;
  GtkWidget *label_ltrtolerance;
  GtkWidget *label_candtolerance;
  GtkWidget *label_usedfeatures;
  GtkWidget *notebook;
  GError *err;
};

struct _GtkProjectSettingsClass
{
  GtkWindowClass parent_class;
  void (* gtk_project_settings) (GtkProjectSettings *projset);
};

GType        gtk_project_settings_get_type(void);

GtkWidget*   gtk_project_settings_new();

const gchar* gtk_project_settings_get_indexname(GtkProjectSettings *projset);

void         gtk_project_settings_update_indexname(GtkProjectSettings *projset,
                                                   const gchar *indexname);

void         gtk_project_settings_update_projectfile(
                                                    GtkProjectSettings *projset,
                                                     const gchar *projectfile);

gint         gtk_project_settings_set_data_from_sqlite(
                                                    GtkProjectSettings *projset,
                                                      const gchar *projectfile);

void         gtk_project_settings_set_data(GtkProjectSettings *projset,
                                           const gchar *projectfile,
                                           gchar **gff3files,
                                           const gchar *indexname,
                                           gboolean clustering,
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
                                           gint psmall,
                                           gint plarge,
                                           gboolean classification,
                                           gdouble ltrtol,
                                           gdouble candtol,
                                           gchar **features);

gint         gtk_project_settings_save_data(GtkProjectSettings *projset);

#endif /* __GTK_PROJECT_SETTINGS_H__ */
