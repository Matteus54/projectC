#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include "gui.h"
#include "user_entries_structs.h"
#include "create_user.h"
#include "login.h"
#include "compte.h"
#include "bdd.h"


void create_account() {

}

void create_account_form() {
  GtkWidget *windowAccountForm;
  GtkWidget *vbox, *livretVbox;
  GtkWidget *id_field, *libelle_field, *booleanLivret;
  GtkWidget *create_account_button, *exit_button;

  windowAccountForm = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(windowAccountForm), "Creation de Compte");
  gtk_window_set_default_size(GTK_WINDOW(windowAccountForm), 400,200);
  gtk_window_set_position(GTK_WINDOW(windowAccountForm), GTK_WIN_POS_CENTER);

  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
  gtk_container_add(GTK_CONTAINER(windowAccountForm), vbox);

  id_field = gtk_entry_new();
  libelle_field = gtk_entry_new();
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

  //liste déroulante du type
  gtk_box_pack_start(GTK_BOX(livretVbox), gtk_label_new("plafond"), 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(livretVbox), plafond, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(livretVbox), gtk_label_new("interet"), 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(livretVbox), interet, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(livretVbox), gtk_label_new("type de livret"), 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(livretVbox), type_livret_list, 0, 0, 0);
  /* FIN LIVRET FORM */


  create_account_button = gtk_button_new_with_label("Create the account");
  g_signal_connect(GTK_BUTTON(create_account_button), "clicked", G_CALLBACK(create_account), NULL);

  exit_button = gtk_button_new_with_label("Exit form");
  g_signal_connect(GTK_BUTTON(exit_button), "clicked", G_CALLBACK(close_window), G_OBJECT(windowAccountForm));

  gtk_box_pack_start(GTK_BOX(vbox), gtk_label_new("Id"), 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), id_field, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), gtk_label_new("Libelle"), 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), libelle_field, 0, 0, 0);
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
  GtkWidget *windowCompte;
  GtkWidget *gridBox;
  GtkWidget *scrollbar;

  windowCompte = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(windowCompte), "Compte");
  gtk_window_set_default_size(GTK_WINDOW(windowCompte), 800,600);
  gtk_window_set_position(GTK_WINDOW(windowCompte), GTK_WIN_POS_CENTER);

  //AJOUT SCROLLBAR
  scrollbar = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(windowCompte),scrollbar);

  gridBox = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(scrollbar), gridBox);

  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  //FIN SCROLLBAR

  GtkWidget *listBoxAccount;
  listBoxAccount = gtk_list_box_new();

  GtkWidget *listBoxTransaction;
  listBoxTransaction = gtk_list_box_new();

  GtkWidget *createCompteButton;
  createCompteButton = gtk_button_new_with_label("Create new account");
  g_signal_connect(createCompteButton, "clicked", G_CALLBACK(create_account_form), NULL);

  gtk_grid_attach(GTK_GRID(gridBox), listBoxAccount, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBox), listBoxTransaction, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBox), createCompteButton, 1, 1, 1, 1);
  //ICI ON RECUPERE LES DONNEES DE TOUS LES COMPTES EXISTANT A UN MEC


  gtk_widget_show_all(windowCompte);
}
