/*
  Copyright (c) 2011-2012 Sascha Kastens <sascha.kastens@studium.uni-hamburg.de>
  Copyright (c) 2011-2012 Center for Bioinformatics, University of Hamburg

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GTK_PROJECT_SETTINGS_H
#define GTK_PROJECT_SETTINGS_H

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

typedef struct _GtkProjectSettings GtkProjectSettings;
typedef struct _GtkProjectSettingsClass GtkProjectSettingsClass;

struct _GtkProjectSettings
{
  GtkWindow window;
  GtkWidget *label_projectfile;
  GtkWidget *label_gff3files;
  GtkWidget *label_indexname;
  GtkWidget *label_didclustering;
  GtkWidget *label_xgapped;
  GtkWidget *label_xgapless;
  GtkWidget *label_xfinal;
  GtkWidget *label_mscoregapless;
  GtkWidget *label_mscoregapped;
  GtkWidget *label_gapopen;
  GtkWidget *label_gapextend;
  GtkWidget *label_matchscore;
  GtkWidget *label_mismatchcost;
  GtkWidget *label_stepsize;
  GtkWidget *label_morelast;
  GtkWidget *label_psmall;
  GtkWidget *label_plarge;
  GtkWidget *label_didclassification;
  GtkWidget *label_ltrtolerance;
  GtkWidget *label_candtolerance;
  GtkWidget *label_usedfeatures;
  GtkWidget *notebook;
  GtRDB *rdb;
};

struct _GtkProjectSettingsClass
{
  GtkWindowClass parent_class;
  void (* gtk_project_settings) (GtkProjectSettings *projset);
};

GType        gtk_project_settings_get_type(void);

const gchar* gtk_project_settings_get_indexname(GtkProjectSettings *projset);

void         gtk_project_settings_update_indexname(GtkProjectSettings *projset,
                                                   const gchar *indexname);

gint         gtk_project_settings_set_data_from_sqlite(GtkProjectSettings *proj,
                                                       const gchar *projectfile,
                                                       GtRDB *rdb,
                                                       GtError *err);

void          gtk_project_settings_set_data(GtkProjectSettings *projset,
                                             const gchar *projectfile,
                                             gchar **gff3files,
                                             const gchar *indexname,
                                             gboolean clustering,
                                             gint xgapped,
                                             gint xgapless,
                                             gint xfinal,
                                             gint mscoregapped,
                                             gint mscoregapless,
                                             gint gapopen,
                                             gint gapextend,
                                             gint matchscore,
                                             gint mismatchcost,
                                             gint stepsize,
                                             const gchar *morelast,
                                             gint psmall,
                                             gint plarge,
                                             gboolean classification,
                                             gdouble ltrtol,
                                             gdouble candtol,
                                             gchar **features);

gint         gtk_project_settings_save_data(GtkProjectSettings *projset,
                                            GtRDB *rdb,
                                            GtError *err);

GtkWidget*   gtk_project_settings_new(GtRDB *rdb);

#endif
