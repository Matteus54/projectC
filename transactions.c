#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <ctype.h>
#include "gui.h"
#include "compte.h"
#include "bdd.h"
#include "transactions.h"

extern GtkWidget *app;
extern GtkWidget *window;
extern GtkWidget *grid;

void create_transaction_form() {}

void import_releve() {

}

void transaction_window(GtkWidget *widget, gpointer* data) {
  UNUSED(widget);
  UNUSED(data);
  GtkWidget *windowTransaction;
  GtkWidget *grid;
  GtkWidget *button_ajout_transaction, *button_import_releve;

  windowTransaction = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(windowTransaction), "Transaction");
  gtk_window_set_default_size(GTK_WINDOW(windowTransaction), 800,600);
  gtk_window_set_position(GTK_WINDOW(windowTransaction), GTK_WIN_POS_CENTER);

  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(windowTransaction), grid);

  button_ajout_transaction = gtk_button_new_with_label("Nouvelle transaction");
  g_signal_connect(button_ajout_transaction, "clicked", G_CALLBACK(create_transaction_form), NULL);

  button_import_releve = gtk_button_new_with_label("Importer un releve");
  g_signal_connect(button_import_releve, "clicked", G_CALLBACK(import_releve), NULL);

  gtk_grid_attach(GTK_GRID(grid), button_ajout_transaction, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button_import_releve, 0, 1, 1, 1);

  gtk_widget_show_all(windowTransaction);
}
