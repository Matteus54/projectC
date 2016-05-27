#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <ctype.h>
#include "gui.h"
#include "bdd.h"
#include "compte.h"
#include "bdd_checks.h"

extern char login[30];
extern GtkWidget *activeWindow;
extern GtkWidget *window;
extern GtkWidget *grid;

void create_account(GtkWidget *widget, gpointer* data) {
  UNUSED(widget);

  account_entry_creation_t* account_entries = (account_entry_creation_t*) data;
  GtkWidget *iban = GTK_WIDGET(account_entries->iban);
  GtkWidget *solde = GTK_WIDGET(account_entries->solde);
  GtkWidget *libelle = GTK_WIDGET(account_entries->libelle);
  GtkWidget *livret = GTK_WIDGET(account_entries->livret);
  GtkWidget *plafond = GTK_WIDGET(account_entries->plafond);
  GtkWidget *interet = GTK_WIDGET(account_entries->interet);
  GtkWidget *type_livret = GTK_WIDGET(account_entries->type_livret);

  const char *iban_text = gtk_entry_get_text(GTK_ENTRY(iban));
  const char *libelle_text = gtk_entry_get_text(GTK_ENTRY(libelle));
  const char *solde_text = gtk_entry_get_text(GTK_ENTRY(solde));


  if(strlen(iban_text) >= 14 && strlen(iban_text) <= 34) {              // TEST SI IBAN CORRECT
    if(isNumeric(solde_text, 1)) {                                      // TEST SI SOLDE EST BIEN NUMERIC
      if(strlen(libelle_text) <= 255) {                                 // TEST SI LIBELLE ENTRE 0 ET 255
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(livret))){    // TEST SI BUTTON CHECK
          //CREATION DUN LIVRET
          const char *plafond_text = gtk_entry_get_text(GTK_ENTRY(plafond));
          const char *interet_text = gtk_entry_get_text(GTK_ENTRY(interet));
          const char *type_livret_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(type_livret));

          printf("type livret: %s\n", type_livret_text);

          if(isNumeric(plafond_text, 0)) {
            if(isNumeric(interet_text, 1)) {
              if(type_livret_text != NULL) {
                //ON CREE LE LIVRET
                char requestAccount[1024] = "INSERT INTO compte VALUES ('";
                strcat(requestAccount, iban_text);
                strcat(requestAccount, "', '");
                strcat(requestAccount, solde_text);
                strcat(requestAccount, "', '");
                strcat(requestAccount, libelle_text);
                strcat(requestAccount, "', 'TRUE', '");
                strcat(requestAccount, login);
                strcat(requestAccount, "');");

                if(bdd_execute(requestAccount)) {
                  char requestSavings[1024] = "INSERT INTO livret VALUES ('";
                  strcat(requestSavings, iban_text);
                  strcat(requestSavings, "', '");
                  strcat(requestSavings, plafond_text);
                  strcat(requestSavings, "', '");
                  strcat(requestSavings, interet_text);
                  strcat(requestSavings, "', '");
                  strcat(requestSavings, type_livret_text);
                  strcat(requestSavings, "');");

                  if(bdd_execute(requestSavings)) {
                    alert_dialog("Savings account created !");
                  }
                  else {
                    alert_dialog("ERROR: Can't create savings account");
                  }
                }
                else {
                  alert_dialog("Can't create the account before the savings account");
                }
              }
              else {
                alert_dialog("You must choose a savings account type");
              }
            }
            else {
              alert_dialog("Interet is not a numeric");
            }
          }
          else {
            alert_dialog("Plafond is not a numeric");
          }
        }
        else {
          //CREATION DUN COMPTE
          char request[1024] = "INSERT INTO compte VALUES ('";
          strcat(request, iban_text);
          strcat(request, "', '");
          strcat(request, solde_text);
          strcat(request, "', '");
          strcat(request, libelle_text);
          strcat(request, "', 'FALSE', '");
          strcat(request, login);
          strcat(request, "');");

          if(bdd_execute(request)) {
            alert_dialog("Account has been created !");
          }
          else {
            alert_dialog("ERROR: Can't create the account");
          }
        }
      }
      else {
        alert_dialog("Error: Libelle must be between 0 and 255");
      }
    }
    else {
      alert_dialog("Solde is not numeric");
    }
  }
  else {
    alert_dialog("Error: IBAN must be between 14 and 34");
  }
}

void create_account_form() {
  GtkWidget *windowAccountForm;
  GtkWidget *vbox, *livretVbox;
  GtkWidget *iban_field, *solde_field, *libelle_field, *booleanLivret;
  GtkWidget *create_account_button, *exit_button;

  windowAccountForm = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(windowAccountForm), "Creation de Compte");
  gtk_window_set_default_size(GTK_WINDOW(windowAccountForm), 400,200);
  gtk_window_set_position(GTK_WINDOW(windowAccountForm), GTK_WIN_POS_CENTER);

  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
  gtk_container_add(GTK_CONTAINER(windowAccountForm), vbox);

  iban_field = gtk_entry_new();
  libelle_field = gtk_entry_new();
  solde_field = gtk_entry_new();
  booleanLivret = gtk_check_button_new_with_label("Livret");

  /* LIVRET FORM */
  livretVbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,10);

  g_signal_connect(GTK_BUTTON(booleanLivret), "clicked", G_CALLBACK(show_hide_button_box), livretVbox);

  GtkWidget *plafond, *interet, *type_livret_list;
  plafond = gtk_entry_new();
  interet = gtk_entry_new();
  type_livret_list = gtk_combo_box_text_new();

  char **listType = bdd_get_type_livret();

  while(*listType != NULL) {
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(type_livret_list),NULL,*listType);
    listType++;
  }

  gtk_combo_box_set_active(GTK_COMBO_BOX(type_livret_list), 0);

  //liste déroulante du type
  gtk_box_pack_start(GTK_BOX(livretVbox), gtk_label_new("plafond"), 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(livretVbox), plafond, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(livretVbox), gtk_label_new("interet"), 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(livretVbox), interet, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(livretVbox), gtk_label_new("type de livret"), 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(livretVbox), type_livret_list, 0, 0, 0);
  /* FIN LIVRET FORM */

  account_entry_creation_t* account_entries = malloc(sizeof(account_entry_creation_t));
  account_entries->iban = iban_field;
  account_entries->solde = solde_field;
  account_entries->libelle = libelle_field;
  account_entries->livret = booleanLivret;
  account_entries->plafond = plafond;
  account_entries->interet = interet;
  account_entries->type_livret = type_livret_list;

  create_account_button = gtk_button_new_with_label("Create the account");
  g_signal_connect(GTK_BUTTON(create_account_button), "clicked", G_CALLBACK(create_account), account_entries);

  exit_button = gtk_button_new_with_label("Exit form");
  g_signal_connect(GTK_BUTTON(exit_button), "clicked", G_CALLBACK(close_window), G_OBJECT(windowAccountForm));

  gtk_box_pack_start(GTK_BOX(vbox), gtk_label_new("Id"), 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), iban_field, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), gtk_label_new("Libelle"), 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), libelle_field, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), gtk_label_new("Solde"), 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), solde_field, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), booleanLivret, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), livretVbox, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), create_account_button, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), exit_button, 0, 0, 0);


  gtk_widget_show_all(windowAccountForm);
  gtk_widget_hide(livretVbox);
}


void show_compte (GtkWidget *widget, gpointer* data) {
  UNUSED(widget);
  UNUSED(data);
  GtkWidget *scrollbar;

  clean_window();

  gtk_window_set_title(GTK_WINDOW(window), "Compte");
  gtk_window_set_default_size(GTK_WINDOW(window), 800,600);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  //AJOUT SCROLLBAR
  scrollbar = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(window),scrollbar);

  gtk_container_add(GTK_CONTAINER(scrollbar), grid);

  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  //FIN SCROLLBAR


  //ON AJOUTE LA LISTE DES COMPTES A CETTE LIST BOX listBoxAccount
  GtkListStore*      model;
  GtkWidget*         view;
  GtkTreeViewColumn* column;

  model = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

  account_t **listType = bdd_get_list_account();

  if(listType != NULL) {
    account_t* account = (account_t*)malloc(sizeof(account_t));
    while(*listType != NULL) {
      account->iban = (*listType)->iban;
      account->solde = (*listType)->solde;
      account->libelle = (*listType)->libelle;
      gtk_list_store_insert_with_values(model, NULL, -1,
                                      0, account->iban,
                                      1, account->solde,
                                      2, account->libelle,
                                      -1);
      listType++;
    }
  }

  view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(model));
  g_object_unref(model);
  column = gtk_tree_view_column_new_with_attributes("Iban",
                                                  gtk_cell_renderer_text_new(),
                                                  "text", 0,
                                                  NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);
  column = gtk_tree_view_column_new_with_attributes("Solde",
                                                  gtk_cell_renderer_text_new(),
                                                  "text", 1,
                                                  NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

  column = gtk_tree_view_column_new_with_attributes("Libelle",
                                                  gtk_cell_renderer_text_new(),
                                                  "text", 2,
                                                  NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

  GtkWidget *createCompteButton, *button_retour;

  createCompteButton = gtk_button_new_with_label("Create new account");
  g_signal_connect(createCompteButton, "clicked", G_CALLBACK(create_account_form), NULL);

  button_retour = gtk_button_new_with_label("Retour");
  g_signal_connect(button_retour, "clicked", G_CALLBACK(main_window), NULL);

  widget_set_margins(createCompteButton, 0, 5, 0, 0);
  widget_set_margins(button_retour, 0, 5, 0, 0);

  gtk_grid_attach(GTK_GRID(grid), createCompteButton, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button_retour, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), view, 0, 1, 2, 1);

  //ICI ON RECUPERE LES DONNEES DE TOUS LES COMPTES EXISTANT A UN MEC


  gtk_widget_show_all(window);
}
