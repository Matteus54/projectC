#ifndef STATISTIQUE_H
#define STATISTIQUE_H

#include <gtk/gtk.h>

typedef struct {
  GtkTreeSelection *account;
  GtkTreeSelection *releve;
} tree_selection_t;

void statistique_window(GtkWidget *widget, gpointer* data);
void releve_statistique_window(GtkWidget *widget, gpointer *data);
void tree_select_releve(GtkTreeSelection *selection, gpointer data);
void check_stat(GtkWidget *widget, tree_selection_t *tree);

#endif
