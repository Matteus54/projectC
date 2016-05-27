#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <ctype.h>
#include "gui.h"
#include "compte.h"
#include "bdd.h"
#include "transactions.h"

extern GtkWidget *app;
extern GtkWidget *activeWindow;
extern GtkWidget *window;
extern GtkWidget *grid;

const char* getfield(char* line, int num) // a reecrire (copier coller d'internet)
{
	char copy[1024];

	strcpy(copy, line);

	const char* tok;
	for (tok = strtok(copy, ",");
			tok && *tok;
			tok = strtok(NULL, ",\n"))
	{
		if (!--num)
			return tok;
	}
	return NULL;
}

void create_transaction_form() {}

void import_releve() {
  FILE *releve;
  char* filename = file_browser(activeWindow);
  char line[1024];

  printf("path to selected releve : %s\n", filename);

  releve = fopen(filename, "r");

  fgets(line, 1024, releve); //on saute la premiere ligne (celle avec les titres des colonnes)
  while(fgets(line, 1024, releve)) {

  	//
  	printf("libelle : %s \n", getfield(line, 3));
  	printf("date : %s \n", getfield(line, 1));
  	printf("date valeur : %s \n", getfield(line, 2));
  	printf("montant : %s \n\n", getfield(line, 4));
  }


  fclose(releve);
  g_free(filename);
}

void transaction_window(GtkWidget *widget, gpointer* data) {
  UNUSED(widget);
  UNUSED(data);
  clean_window();
  GtkWidget *button_ajout_transaction, *button_import_releve, *button_retour;

  activeWindow = window;
  gtk_window_set_title(GTK_WINDOW(window), "Transaction");
  gtk_window_set_default_size(GTK_WINDOW(window), 800,600);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  button_ajout_transaction = gtk_button_new_with_label("Nouvelle transaction");
  g_signal_connect(button_ajout_transaction, "clicked", G_CALLBACK(create_transaction_form), NULL);

  button_import_releve = gtk_button_new_with_label("Importer un releve");
  g_signal_connect(button_import_releve, "clicked", G_CALLBACK(import_releve), NULL);

  button_retour = gtk_button_new_with_label("Retour");
  g_signal_connect(button_retour, "clicked", G_CALLBACK(main_window), NULL);

  gtk_grid_attach(GTK_GRID(grid), button_ajout_transaction, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button_import_releve, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button_retour, 0, 2, 1, 1);

  gtk_widget_show_all(window);
}
