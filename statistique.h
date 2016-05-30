#ifndef STATISTIQUE_H
#define STATISTIQUE_H

#include <gtk/gtk.h>

void statistique_window(GtkWidget *widget, gpointer* data);
void releve_statistique_window(GtkWidget *widget, gpointer *data);
void tree_select_releve(GtkTreeSelection *selection, gpointer data);

#endif
