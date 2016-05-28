#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <ctype.h>
#include "gui.h"
#include "compte.h"
#include "bdd.h"
#include "bdd_updates.h"
#include "transactions.h"

extern GtkWidget *app;
extern GtkWidget *activeWindow;
extern GtkWidget *window;
extern GtkWidget *grid;

const char* getfield(const char* line, int num) // a reecrire (copier coller d'internet)
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

void create_categorie(GtkWidget *widget, GtkWidget *field) {
	UNUSED(widget);

	char *newCat = (char*)gtk_entry_get_text(GTK_ENTRY(field));

	bdd_insert_categorie(newCat);
}

void create_categorie_form(GtkWidget *widget) {
	UNUSED(widget);
	GtkWidget *windowCategorieForm, *grid;
	GtkWidget *name_field;
	GtkWidget *button_OK, *button_exit;

	windowCategorieForm = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(windowCategorieForm), "New category form");
  //gtk_window_set_default_size(GTK_WINDOW(windowCategorieForm), 400,200);
  gtk_window_set_position(GTK_WINDOW(windowCategorieForm), GTK_WIN_POS_CENTER);

	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(windowCategorieForm), grid);

	name_field = gtk_entry_new();

	button_OK = gtk_button_new_with_label("OK");
	g_signal_connect(G_OBJECT(button_OK), "clicked", G_CALLBACK(create_categorie), name_field);

	button_exit = gtk_button_new_with_label("Return");
	g_signal_connect(G_OBJECT(button_exit), "clicked", G_CALLBACK(close_window), windowCategorieForm);

	gtk_grid_attach(GTK_GRID(grid), gtk_label_new("New category"), 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), name_field, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), button_OK, 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), button_exit, 0, 3, 1, 1);

	gtk_widget_show_all(windowCategorieForm);

}

transaction_t* extract_transaction_from_import(transaction_entry_creation_t *entries) {
	UNUSED(entries);
	return NULL;
}


void create_transaction(GtkWidget *widget, transaction_entry_creation_t *entries) {
	UNUSED(widget);

	transaction_entry_creation_t* transaction_entry = (transaction_entry_creation_t*) entries;
  GtkWidget *compte = GTK_WIDGET(transaction_entry->compte);
  GtkWidget *date = GTK_WIDGET(transaction_entry->date);
  GtkWidget *libelle = GTK_WIDGET(transaction_entry->libelle);
  GtkWidget *montant = GTK_WIDGET(transaction_entry->montant);
  GtkWidget *commission = GTK_WIDGET(transaction_entry->commission);
  GtkWidget *categorie = GTK_WIDGET(transaction_entry->categorie);
  GtkWidget *commentaire = GTK_WIDGET(transaction_entry->commentaire);


	char *compte_text;
	if (GTK_IS_LABEL(compte)) {
		compte_text = (char*)gtk_label_get_text(GTK_LABEL(compte));
	} else {
		compte_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(compte));
	}
	const char *iban = bdd_get_iban_from_libelle(compte_text);

	const char *libelle_text;
	if (GTK_IS_ENTRY(libelle)) {
  	libelle_text = gtk_entry_get_text(GTK_ENTRY(libelle));
	} else {
		libelle_text = gtk_label_get_text(GTK_LABEL(libelle));
	}

	const char *montant_text;
	if (GTK_IS_ENTRY(montant)) {
		montant_text = gtk_entry_get_text(GTK_ENTRY(montant));
	} else {
		montant_text = gtk_label_get_text(GTK_LABEL(montant));
	}

	const char *commission_text;
	if(GTK_IS_ENTRY(commission)) {
		commission_text = gtk_entry_get_text(GTK_ENTRY(commission));
	} else {
		commission_text = gtk_label_get_text(GTK_LABEL(commission));
	}

	const char *categorie_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(categorie));
	const char *commentaire_text = gtk_entry_get_text(GTK_ENTRY(commentaire));

	char *date_text = malloc(sizeof(char)*10);
	if (GTK_IS_CALENDAR(date)) {
		guint *year, *month, *day;
		year = malloc(sizeof(guint));
		month = malloc(sizeof(guint));
		day = malloc(sizeof(guint));
		gtk_calendar_get_date(GTK_CALENDAR(date), year, month, day);

		//char buffer[10];

		sprintf(date_text, "%d/%d/%d", *year, *month, *day);
		/*
		sprintf(buffer, "%d", *year);
		strcat(date_text, buffer);
		strcat(date_text, "/");
		sprintf(buffer, "%d", *month);
		strcat(date_text, buffer);
		strcat(date_text, "/");
		sprintf(buffer, "%d", *day);
		strcat(date_text, buffer);
		*/
	} else {
		strcpy(date_text, gtk_label_get_text(GTK_LABEL(date)));
	}

	if(strlen(libelle_text) <= 255) {
		if(isNumeric(montant_text, 1)) {
			if(isNumeric(commission_text, 1)) {
				if(strlen(commentaire_text) <= 255) {
					char request[1024] = "INSERT INTO transactionCompte "\
				  "(compte_iban, date, libelle, montant, negatif, commission, type, commentaire) "\
				  "VALUES('";
				  strcat(request, iban);
				  strcat(request, "', '");
				  strcat(request, date_text);
				  strcat(request, "', '");
				  strcat(request, libelle_text);
				  strcat(request, "', '");
					strcat(request,	montant_text);
				  strcat(request, "', ");
				  strcat(request, "'FALSE'"); //n'est peut être pas necessaire
				  strcat(request, ", '");
					strcat(request, commission_text);
				  strcat(request, "', '");
				  strcat(request, categorie_text);
				  strcat(request, "', '");
				  strcat(request, commentaire_text);
				  strcat(request, "');");

				  bdd_execute(request);
				}
				else {
					alert_dialog("Commentaire must be between 0 and 255");
				}
			}
			else {
				alert_dialog("Commission must be a numeric");
			}
		}
		else {
			alert_dialog("Montant must be a numeric");
		}
	}
	else {
		alert_dialog("Libelle between 0 and 255");
	}
}

void create_transaction_form() {
	char **listCompte = bdd_get_libelle_account();
	if (listCompte != NULL) {
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
		while(*listCompte != NULL) {
	    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(compte_list),NULL,*listCompte);
	    listCompte++;
	  }

	  char **listCategorie = bdd_get_categorie();
		if(listCategorie != NULL) {
			while(*listCategorie != NULL) {
		    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(categorie_list),NULL,*listCategorie);
		    listCategorie++;
		  }
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
	  g_signal_connect(GTK_BUTTON(button_create_categorie), "clicked", G_CALLBACK(create_categorie_form), NULL);

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
	} else {
		alert_dialog("Please create accounts before trying to add transactions");
	}
}

void valider_import_transaction(const char *iban, const char *line) {
	//const char* iban,const char* date,const char* libelle,const char* montant,const char* commission) {
	GtkWidget *windowTransactionForm;
  GtkWidget *grid;
  GtkWidget *categorie_list ,*commentaire_field;
	GtkWidget *date_label, *libelle_label, *montant_label, *commission_label;
  GtkWidget *button_create_categorie, *button_OK, *button_exit;

	char* date = (char*)malloc(sizeof(char)*10);
	strcpy(date, getfield(line, 1));
	char* libelle = (char*)malloc(sizeof(char)*255);
	strcpy(libelle, getfield(line, 2));
	char* montant = (char*)malloc(sizeof(char)*13);
	strcpy(montant, getfield(line, 3));
	char* commission = (char*)malloc(sizeof(char)*7);
	strcpy(commission, getfield(line, 4));

  windowTransactionForm = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(windowTransactionForm), "Creation d'une transaction'");
  gtk_window_set_default_size(GTK_WINDOW(windowTransactionForm), 400,200);
  gtk_window_set_position(GTK_WINDOW(windowTransactionForm), GTK_WIN_POS_CENTER);

  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(windowTransactionForm), grid);

  commentaire_field = gtk_entry_new();
  categorie_list = gtk_combo_box_text_new();

	date_label = gtk_label_new(date);
	libelle_label = gtk_label_new(libelle);
	montant_label = gtk_label_new(montant);
	commission_label = gtk_label_new(commission);

  //ajout des choix dans les listes deroulante
  char **listCategorie = bdd_get_categorie();
  while(*listCategorie != NULL) {
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(categorie_list),NULL,*listCategorie);
    listCategorie++;
  }

  gtk_combo_box_set_active(GTK_COMBO_BOX(categorie_list), 0);


  //declaration de la structure qui sert de formumlaire
  transaction_entry_creation_t *transaction_entries = malloc(sizeof(transaction_entry_creation_t));
	transaction_entries->compte = gtk_label_new(iban);
  transaction_entries->date = date_label;
  transaction_entries->libelle = libelle_label;
  transaction_entries->montant = montant_label;
  transaction_entries->commission = commission_label;
  transaction_entries->categorie = categorie_list;
  transaction_entries->commentaire = commentaire_field;

  // creation of the buttons
  button_create_categorie = gtk_button_new_with_label("New categorie");
  g_signal_connect(GTK_BUTTON(button_create_categorie), "clicked", G_CALLBACK(create_categorie_form), NULL);

  button_OK = gtk_button_new_with_label("Create transaction");
  g_signal_connect(GTK_BUTTON(button_OK), "clicked", G_CALLBACK(create_transaction), transaction_entries);

  button_exit = gtk_button_new_with_label("Exit form");
  g_signal_connect(GTK_BUTTON(button_exit), "clicked", G_CALLBACK(close_window), windowTransactionForm);

  // packing GUI
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Date"), 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), date_label, 0, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Libelle"), 0, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), libelle_label, 0, 5, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Amount"), 0, 6, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), montant_label, 0, 7, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Commission"), 0, 8, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), commission_label, 0, 9, 1, 1);
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
  char line[1024], iban[34];

  printf("path to selected releve : %s\n", filename);

  releve = fopen(filename, "r");

	fgets(iban, 34, releve);

  fgets(line, 1024, releve); //on saute la ligne des titres des colonnes
  while(fgets(line, 1024, releve)) {
		valider_import_transaction(iban, line);
		/*
															getfield(line, 1),\
															getfield(line, 3),\
															getfield(line, 4),\
															getfield(line, 5));
		*/
  	/*
  	printf("libelle : %s \n", getfield(line, 3));
  	printf("date : %s \n", getfield(line, 1));
  	printf("date valeur : %s \n", getfield(line, 2));
  	printf("montant : %s \n\n", getfield(line, 4));
		*/
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
