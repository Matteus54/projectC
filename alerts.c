#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <math.h>
#include "compte.h"
#include "alerts.h"
#include "bdd.h"
#include "gui.h"

extern GtkWidget *window;
extern GtkWidget *grid;
extern char login[30];

void check_alerts() {
  printf("check_alerts\n");
  account_t** listAccounts = bdd_get_list_account();
  if (listAccounts != NULL) {
    char alertText[1024];
    strcpy(alertText, "");
    char request[1024];

    alert_t** listAlerts = bdd_get_list_alerts();

    if (listAlerts != NULL) {
      char *time_text;
      double result= 0;
      char inAccounts[1024];
      strcpy(inAccounts, "('");
      int i = 0;
      while(listAccounts[i] != NULL) {
        strcat(inAccounts, (listAccounts[i]->iban));
        if (listAccounts[i+1] != NULL)
          strcat(inAccounts, "','");
        i++;
      }
      strcat(inAccounts, "')");

      while (*listAlerts != NULL) {
        printf("hello 0\n");
        strcpy(request, "");

        if (strcmp((*listAlerts)->type_temps, "monthly") == 0) {
          //time_text = " AND date >= ADDMONTHS(SYSDATE, -1)";
          time_text = "";
        } else if (strcmp((*listAlerts)->type_temps, "yearly") == 0) {
          //time_text = " AND date >= ADDMONTHS(SYSDATE, -12)";
          time_text = "";
        } else if (strcmp((*listAlerts)->type_temps, "total") == 0) {
          time_text = "";
        }

        if ( strcmp((*listAlerts)->type, "max_cat") == 0) {
          printf("hello 11\n");
          sprintf(request, "SELECT SUM(T.montant) "\
          "FROM transactionCompte T "\
          "WHERE type = '%s' "\
          "%s"\
          "AND compte_iban IN %s;", (*listAlerts)->compte_cat, time_text,  inAccounts);
        } else if ( strcmp((*listAlerts)->type, "min_account") == 0) {
          sprintf(request, "SELECT solde "\
          "FROM compte "\
          "WHERE iban = '%s';", bdd_get_iban_from_libelle((*listAlerts)->compte_cat));
        }
        printf("hello 2\n");

        result = bdd_get_sum(request);

        if (result != NAN) {
          printf("%f | %f\n", result, (*listAlerts)->montant);

          //cette condition est à changer si on ajoute d'autre type d'alert
          if ((result > (*listAlerts)->montant && strcmp((*listAlerts)->type, "max_cat") == 0) ||\
             (result < (*listAlerts)->montant && strcmp((*listAlerts)->type, "min_account") == 0) ) {
            printf("coucou\n");
            if (strcmp((*listAlerts)->type, "max_cat") == 0) {
              strcat(alertText, "La categorie ");
              strcat(alertText, (*listAlerts)->compte_cat);
              strcat(alertText, " depasse le montant alloue");
            } else if (strcmp((*listAlerts)->type, "min_account") == 0) {
              printf("coucou\n");
              strcat(alertText, "Le compte ");
              printf("coucou\n");
              strcat(alertText, (*listAlerts)->compte_cat);
              printf("coucou\n");
              strcat(alertText, " est en dessous du seuil autorise");
              printf("coucou\n");
            }
            if ( strcmp((*listAlerts)->type_temps, "monthly") == 0) {
              strcat(alertText, " pour le mois");
            } else if (strcmp((*listAlerts)->type_temps, "yearly") == 0) {
              strcat(alertText, " pour l'annee");
            }
            strcat(alertText, "\n");
          }
        }

        listAlerts++;
      }
      if (strcmp(alertText, "") != 0)
        alert_dialog(window, alertText);
    }
  }
}


void create_alert(GtkWidget *widget,alert_entry_t *entries) {
  UNUSED(widget);
  char request[1024];
  alert_t *alert = malloc(sizeof(alert_t));

  strcpy(alert->type, entries->type);
  strcpy(alert->type_temps, gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries->type_temps)));
  strcpy(alert->compte_cat, gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries->compte_cat)));
  char *montant_text = (char*)gtk_entry_get_text(GTK_ENTRY(entries->montant));
  alert->montant = atof(montant_text);

  strcpy(request, "INSERT INTO alerts (proprietaire, type, type_temps, compte_cat, montant) VALUES ('");
  strcat(request, login);
  strcat(request, "', '");
  strcat(request, alert->type);
  strcat(request, "', '");
  strcat(request, alert->type_temps);
  strcat(request, "', '");
  strcat(request, alert->compte_cat);
  strcat(request, "', '");
  strcat(request, montant_text);
  strcat(request, "');");

  bdd_execute(request);
}

void create_alert_form(GtkWidget *widget, char* type) {
  UNUSED(widget);
  GtkWidget *windowAlertForm;
  GtkWidget *compte_cat_list, *type_temps_list;
  GtkWidget *montant_field;
  GtkWidget *button_create_alert, *button_exit;
  GtkWidget *grid;

  windowAlertForm = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(windowAlertForm), "Creation d'alerte");
  gtk_window_set_default_size(GTK_WINDOW(windowAlertForm), 400,400);
  gtk_window_set_position(GTK_WINDOW(windowAlertForm), GTK_WIN_POS_CENTER);

  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(windowAlertForm), grid);

  montant_field = gtk_entry_new();

  compte_cat_list = gtk_combo_box_text_new();

  char **listCompteCat;

  if (strcmp(type, "max_cat") == 0) {
    listCompteCat = bdd_get_categorie();

    gtk_entry_set_placeholder_text(GTK_ENTRY(montant_field), "maximum autorise");
  } else if (strcmp(type, "min_account") == 0) {
    listCompteCat = bdd_get_libelle_account();

    gtk_entry_set_placeholder_text(GTK_ENTRY(montant_field), "minimum autorise");
  }

  while(*listCompteCat != NULL) {
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(compte_cat_list), NULL, *listCompteCat);
    listCompteCat++;
  }
  gtk_combo_box_set_active(GTK_COMBO_BOX(compte_cat_list), 0);

  type_temps_list = gtk_combo_box_text_new();
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(type_temps_list), NULL, "monthly");
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(type_temps_list), NULL, "yearly");
  gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(type_temps_list), NULL, "total");
  gtk_combo_box_set_active(GTK_COMBO_BOX(type_temps_list), 0);

  //linking GtkWidgets to entries to send to create_alert function
  alert_entry_t *entries = (alert_entry_t*) malloc(sizeof(alert_entry_t));
  strcpy(entries->type, type);
  entries->type_temps = type_temps_list;
  entries->compte_cat = compte_cat_list;
  entries->montant = montant_field;

  button_create_alert = gtk_button_new_with_label("Create Alert");
  g_signal_connect(GTK_BUTTON(button_create_alert), "clicked", G_CALLBACK(create_alert), entries);

  button_exit = gtk_button_new_with_label("Exit form");
  g_signal_connect(GTK_BUTTON(button_exit), "clicked", G_CALLBACK(close_window), windowAlertForm);

  gtk_grid_attach(GTK_GRID(grid), compte_cat_list, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), type_temps_list, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), montant_field, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button_create_alert, 0, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button_exit, 0, 4, 1, 1);

  gtk_widget_show_all(windowAlertForm);
}


void alert_window(GtkWidget *widget) {
  UNUSED(widget);
  check_alerts();
  char **listCompte = bdd_get_libelle_account();
  if (listCompte != NULL) {
    UNUSED(widget);
    clean_window();
    GtkWidget *button_alerte_cat, *button_alert_compte, *button_retour;

    gtk_window_set_title(GTK_WINDOW(window), "Alertes");
    gtk_window_set_default_size(GTK_WINDOW(window), 800,600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    button_alerte_cat = gtk_button_new_with_label("Nouvelle alerte sur une categorie");
    g_signal_connect(button_alerte_cat, "clicked", G_CALLBACK(create_alert_form), "max_cat");

    button_alert_compte = gtk_button_new_with_label("Nouvelle alerte sur un compte");
    g_signal_connect(button_alert_compte, "clicked", G_CALLBACK(create_alert_form), "min_account");

    button_retour = gtk_button_new_with_label("Retour");
    g_signal_connect(button_retour, "clicked", G_CALLBACK(main_window), NULL);

    gtk_grid_attach(GTK_GRID(grid), button_alerte_cat, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_alert_compte, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_retour, 0, 2, 1, 1);

    gtk_widget_show_all(window);
  } else {
    alert_dialog(window, "Please create accounts before trying to add alerts");
  }
}
