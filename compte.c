#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <ctype.h>
#include "gui.h"
#include "bdd.h"
#include "compte.h"
#include "transactions.h"
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
  GtkWidget *window = GTK_WIDGET(account_entries->window);

  const char *iban_text = gtk_entry_get_text(GTK_ENTRY(iban));
  const char *libelle_text = gtk_entry_get_text(GTK_ENTRY(libelle));
  const char *solde_text = gtk_entry_get_text(GTK_ENTRY(solde));


  if(strlen(iban_text) >= 14 && strlen(iban_text) <= 35) {              // TEST SI IBAN CORRECT
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
                    alert_dialog_then_close(window, "Savings account created !");
                  }
                  else {
                    alert_dialog(window, "ERROR: Can't create savings account");
                  }
                }
                else {
                  alert_dialog(window, "Can't create the account before the savings account");
                }
              }
              else {
                alert_dialog(window, "You must choose a savings account type");
              }
            }
            else {
              alert_dialog(window, "Interet is not a numeric");
            }
          }
          else {
            alert_dialog(window, "Plafond is not a numeric");
          }
        }
        else {
          //CREATION DUN COMPTE
          char request[1024] = "INSERT INTO compte VALUES ('";
          strcat(request, iban_text);
          strcat(request, "', ");
          strcat(request, solde_text);
          strcat(request, ", '");
          strcat(request, libelle_text);
          strcat(request, "', 'FALSE', '");
          strcat(request, login);
          strcat(request, "');");

          if(bdd_execute(request)) {
            alert_dialog_then_close(window, "Account has been created !");
          }
          else {
            alert_dialog(window, "ERROR: Can't create the account");
          }
        }
      }
      else {
        alert_dialog(window, "Error: Libelle must be between 0 and 255");
      }
    }
    else {
      alert_dialog(window, "Solde is not numeric");
    }
  }
  else {
    alert_dialog(window, "Error: IBAN must be between 14 and 35");
  }
}

void tree_selection(GtkTreeSelection *selection, gpointer data) {
  UNUSED(data);
  GtkTreeIter iter;
  GtkTreeModel* model;
  GtkListStore* modelTransaction = data;
  char *iban;
  if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
      gtk_tree_model_get (model, &iter, 0, &iban, -1);

      gtk_list_store_clear(modelTransaction);

      transaction_t **listTransaction = bdd_get_list_transaction(iban, NULL, NULL);
      if(listTransaction != NULL) {
        while(*listTransaction != NULL) {
          gtk_list_store_insert_with_values(modelTransaction, NULL, -1,
                                          0, (*listTransaction)->id,
                                          1, (*listTransaction)->date,
                                          2, (*listTransaction)->libelle,
                                          3, (*listTransaction)->montant,
                                          4, (*listTransaction)->commission,
                                          5, (*listTransaction)->categorie,
                                          6, (*listTransaction)->commentaire,
                                          -1);
          listTransaction++;
        }
      }
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
  account_entries->window = windowAccountForm;

  create_account_button = gtk_button_new_with_label("Create the account");
  g_signal_connect(GTK_BUTTON(create_account_button), "clicked", G_CALLBACK(create_account), account_entries);

  exit_button = gtk_button_new_with_label("Exit form");
  g_signal_connect(GTK_BUTTON(exit_button), "clicked", G_CALLBACK(close_window), G_OBJECT(windowAccountForm));

  gtk_box_pack_start(GTK_BOX(vbox), gtk_label_new("IBAN"), 0, 0, 0);
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
  //GtkWidget *scrollbar;

  clean_window();

  gtk_window_set_title(GTK_WINDOW(window), "Compte");
  gtk_window_set_default_size(GTK_WINDOW(window), 800,600);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  //AJOUT SCROLLBAR
  GtkWidget *scrollbar = gtk_scrolled_window_new(NULL, NULL);

  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  //FIN SCROLLBAR


  //ON AJOUTE LA LISTE DES COMPTES A CETTE LIST BOX listBoxAccount
  GtkListStore*      model;
  GtkListStore*      modelTransaction;
  GtkWidget*         view;
  GtkWidget*         viewTransaction;
  GtkTreeViewColumn* column;
  GtkTreeSelection*  select;

  model = gtk_list_store_new(6, G_TYPE_STRING, G_TYPE_DOUBLE, G_TYPE_STRING, G_TYPE_DOUBLE, G_TYPE_DOUBLE, G_TYPE_STRING);
  modelTransaction = gtk_list_store_new(7, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_DOUBLE, G_TYPE_DOUBLE, G_TYPE_STRING, G_TYPE_STRING);

  account_t **listCompte = bdd_get_list_account();
  if(listCompte != NULL) {
    while(*listCompte != NULL) {
      gtk_list_store_insert_with_values(model, NULL, -1,
                                      0, (*listCompte)->iban,
                                      1, (*listCompte)->solde,
                                      2, (*listCompte)->libelle,
                                      -1);
      listCompte++;
    }
  }

  livret_t **listLivret = bdd_get_list_livret();
  if(listLivret != NULL) {
    while(*listLivret != NULL) {
      gtk_list_store_insert_with_values(model, NULL, -1,
                                      0, (*listLivret)->iban ,
                                      1, (*listLivret)->solde,
                                      2, (*listLivret)->libelle,
                                      3, (*listLivret)->plafond,
                                      4, (*listLivret)->interet,
                                      5, (*listLivret)->type_livret,
                                      -1);
      listLivret++;
    }
  }

  view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(model));
  g_object_unref(model);

  column = gtk_tree_view_column_new_with_attributes("Iban", gtk_cell_renderer_text_new(), "text", 0, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);
  column = gtk_tree_view_column_new_with_attributes("Solde", gtk_cell_renderer_text_new(), "text", 1, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);
  column = gtk_tree_view_column_new_with_attributes("Libelle", gtk_cell_renderer_text_new(), "text", 2, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);
  column = gtk_tree_view_column_new_with_attributes("Plafond", gtk_cell_renderer_text_new(), "text", 3, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);
  column = gtk_tree_view_column_new_with_attributes("Interet", gtk_cell_renderer_text_new(), "text", 4, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);
  column = gtk_tree_view_column_new_with_attributes("Type", gtk_cell_renderer_text_new(), "text", 5, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);


  //ONCLICK sur un compte, on applique la fonction tree_selection
  select = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
  gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);
  g_signal_connect(G_OBJECT(select), "changed", G_CALLBACK(tree_selection), modelTransaction);

  viewTransaction = gtk_tree_view_new_with_model(GTK_TREE_MODEL(modelTransaction));
  g_object_unref(modelTransaction);

  column = gtk_tree_view_column_new_with_attributes("Id", gtk_cell_renderer_text_new(), "text", 0, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(viewTransaction), column);
  column = gtk_tree_view_column_new_with_attributes("date", gtk_cell_renderer_text_new(), "text", 1, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(viewTransaction), column);
  column = gtk_tree_view_column_new_with_attributes("libelle", gtk_cell_renderer_text_new(), "text", 2, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(viewTransaction), column);
  column = gtk_tree_view_column_new_with_attributes("montant", gtk_cell_renderer_text_new(), "text", 3, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(viewTransaction), column);
  column = gtk_tree_view_column_new_with_attributes("commission", gtk_cell_renderer_text_new(), "text", 4, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(viewTransaction), column);
  column = gtk_tree_view_column_new_with_attributes("Categorie", gtk_cell_renderer_text_new(), "text", 5, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(viewTransaction), column);
  column = gtk_tree_view_column_new_with_attributes("commentaire", gtk_cell_renderer_text_new(), "text", 6, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(viewTransaction), column);


  GtkWidget *createCompteButton, *button_retour;

  createCompteButton = gtk_button_new_with_label("Create new account");
  g_signal_connect(createCompteButton, "clicked", G_CALLBACK(create_account_form), NULL);

  button_retour = gtk_button_new_with_label("Retour");
  g_signal_connect(button_retour, "clicked", G_CALLBACK(main_window), NULL);

  widget_set_margins(createCompteButton, 0, 5, 0, 0);
  widget_set_margins(button_retour, 0, 5, 0, 0);

  gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrollbar), 100);


  int h = 30;
  gtk_grid_attach(GTK_GRID(grid), view, 0, 0, 1, h);
  gtk_container_add(GTK_CONTAINER(scrollbar), viewTransaction);
  gtk_grid_attach(GTK_GRID(grid), scrollbar, 2, 0, 1, h);
  //gtk_grid_attach(GTK_GRID(grid), viewTransaction, 2, 0, 1, 10);
  gtk_grid_attach(GTK_GRID(grid), createCompteButton, 1, 0, 1, 1);
  int i;
  gtk_grid_attach(GTK_GRID(grid), button_retour, 1, 1, 1, 1);
  for (i = 2; i<h; i++) {
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new(" "), 1, i, 1, 1);
  }

  //ICI ON RECUPERE LES DONNEES DE TOUS LES COMPTES EXISTANT A UN MEC

  gtk_widget_show_all(window);
}
