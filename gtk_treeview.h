#ifndef __gtk_doctree_h__
#define __gtk_doctree_h__  1

#include <gtk/gtk.h>
#include <errno.h>

#include "gtk_appdata.h"
#include "gtk_statusbar.h"

/** function prototypes */
void tree_set_selection (GtkWidget *widget, gpointer data);
GtkTreeIter *tree_get_iter_from_view (gpointer data);
gchar *treeview_getname (gpointer data);
void treeview_setname (gpointer data, kinst_t *inst);
void check_untitled_remove (gpointer data);
// gchar *treeview_setname (gpointer data, kinst_t *inst);
kinst_t *treeview_append (mainwin_t *app, const gchar *filename);
GtkTreeModel *treemodel_init (mainwin_t *app, gchar **argv);
// GtkTreeModel *treemodel_init (mainwin_t *app, gchar **argv);
GtkTreeModel *create_and_fill_model (mainwin_t *app);
GtkWidget *create_view_and_model (mainwin_t *app, gchar **argv);
// gint doctree_get_count (GtkWidget *treeview);
kinst_t *inst_get_selected (gpointer data);
void doctree_for_each (GtkWidget *widget, mainwin_t *app);
void doctree_newfile (mainwin_t *app, const gchar *filename);
gboolean doctree_remove_iter (gpointer data, GtkTreeIter *iter);
gboolean treeview_remove_selected (gpointer data);
gint treeview_count_unsaved (gpointer data);

/** doctree callbacks */
void treeview_changed (GtkWidget *widget, gpointer data);

#endif

