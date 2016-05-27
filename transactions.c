#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <ctype.h>
#include "gui.h"
#include "compte.h"
#include "bdd.h"
#include "bdd_inserts.h"
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

void create_categorie() {}

void create_transaction(GtkWidget *widget, transaction_entry_creation_t *entries) {
  UNUSED(widget);
  guint *year, *month, *day;
  year = malloc(sizeof(guint));
  month = malloc(sizeof(guint));
  day = malloc(sizeof(guint));
  char date[8];

  const char *compte = bdd_get_iban_from_libelle(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries->compte)));
  gtk_calendar_get_date(GTK_CALENDAR(entries->date), year, month, day);
  const char *libelle = gtk_entry_get_text(GTK_ENTRY(entries->libelle));
  const char *montant = gtk_entry_get_text(GTK_ENTRY(entries->montant));
  const char *commission = gtk_entry_get_text(GTK_ENTRY(entries->commission));
  const char *categorie = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries->categorie));
  const char *commentaire = gtk_entry_get_text(GTK_ENTRY(entries->commentaire));

  transaction_t *transaction = (transaction_t*)malloc(sizeof(transaction_t));

  (*month)++;
  if (*month < 10)
    sprintf(date, "%i0%i%i", *year, *month, *day);
  else
    sprintf(date, "%i%i%i", *year, *month, *day);

  transaction->compte = compte;
  transaction->date = date;
  transaction->libelle = libelle;
	transaction->montant = atof(montant);
	transaction->commission = atof(commission);
  transaction->categorie = categorie;
  transaction->commentaire = commentaire;

  bdd_insert_transaction(transaction);
}

void create_transaction_form() {
  GtkWidget *windowTransactionForm;
  GtkWidget *grid;
  GtkWidget *compte_list, *date_calendar, *libelle_field, *montant_field, *commission_field, *categorie_list ,*commentaire_field;
  GtkWidget *button_create_categorie, *button_OK, *button_exit;

  windowTransactionForm = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(windowTransactionForm), "Creation d'une transaction'");
  gtk_window_set_default_size(GTK_WINDOW(windowTransactionForm), 400,200);
  gtk_window_set_position(GTK_WINDOW(windowTransactionForm), GTK_WIN_POS_CENTER);

  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(windowTransactionForm), grid);

  compte_list = gtk_combo_box_text_new();
  date_calendar = gtk_calendar_new();
  libelle_field = gtk_entry_new();
  montant_field = gtk_entry_new();
  commission_field = gtk_entry_new();
  commentaire_field = gtk_entry_new();
  categorie_list = gtk_combo_box_text_new();

  //ajout des choix dans les listes deroulante
  account_t **listCompte = bdd_get_list_account();
  while(*listCompte != NULL) {
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(compte_list),NULL,(*listCompte)->libelle);
    listCompte++;
  }

  char **listCategorie = bdd_get_categorie();
  while(*listCategorie != NULL) {
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(categorie_list),NULL,*listCategorie);
    listCategorie++;
  }

  gtk_combo_box_set_active(GTK_COMBO_BOX(compte_list), 0);
  gtk_combo_box_set_active(GTK_COMBO_BOX(categorie_list), 0);


  //declaration de la structure qui sert de formumlaire
  transaction_entry_creation_t *transaction_entries = malloc(sizeof(transaction_entry_creation_t));
  transaction_entries->compte = compte_list;
  transaction_entries->date = date_calendar;
  transaction_entries->libelle = libelle_field;
  transaction_entries->montant = montant_field;
  transaction_entries->commission = commission_field;
  transaction_entries->categorie = categorie_list;
  transaction_entries->commentaire = commentaire_field;

  // creation of the buttons
  button_create_categorie = gtk_button_new_with_label("New categorie");
  g_signal_connect(GTK_BUTTON(button_create_categorie), "clicked", G_CALLBACK(create_categorie), NULL);

  button_OK = gtk_button_new_with_label("Create transaction");
  g_signal_connect(GTK_BUTTON(button_OK), "clicked", G_CALLBACK(create_transaction), transaction_entries);

  button_exit = gtk_button_new_with_label("Exit form");
  g_signal_connect(GTK_BUTTON(button_exit), "clicked", G_CALLBACK(close_window), windowTransactionForm);

  // packing GUI
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Linked account"), 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), compte_list, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Date"), 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), date_calendar, 0, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Libelle"), 0, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), libelle_field, 0, 5, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Amount"), 0, 6, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), montant_field, 0, 7, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), gtk_label_new("utiliser la ',' comme separation"), 1, 7, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Commission"), 0, 8, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), commission_field, 0, 9, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Categorie"), 0, 10, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), categorie_list, 0, 11, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button_create_categorie, 1, 11, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Commentaire"), 0, 12, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), commentaire_field, 0, 13, 2, 1);
  gtk_grid_attach(GTK_GRID(grid), button_OK, 0, 14, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button_exit, 1, 14, 1, 1);

  gtk_widget_show_all(windowTransactionForm);
}


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
