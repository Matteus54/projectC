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
#include "transactions.h"

extern GtkApplication *app;
extern GtkWidget *window;
extern GtkWidget *grid;



void try_login(GtkWidget* widget, gpointer* data) {
  UNUSED(widget);
  user_entry_login_t* user_data_entry = (user_entry_login_t*) data;
  
  //extraction of content from fields
  GtkWidget *login = GTK_WIDGET(user_data_entry->login);
  GtkWidget *password = GTK_WIDGET(user_data_entry->password);

  const char *login_text = gtk_entry_get_text(GTK_ENTRY(login));
  const char *password_text = gtk_entry_get_text(GTK_ENTRY(password));
  
  int password_hash = hash(password_text);
  char buffer[8];
  sprintf(buffer, "%d", password_hash);

  char request[1024] = "SELECT login FROM utilisateur WHERE login = '";
  strcat(request, login_text);
  strcat(request, "' AND password = '");
  strcat(request, buffer);
  strcat(request, "';");
  if(bdd_login(request)) {

    clean_window();

    GtkWidget *button_compte, *button_transaction, *button_exit;

    //setting window
    gtk_window_set_title(GTK_WINDOW(window), "Welcome");

    button_compte = gtk_button_new_with_label("Compte");
    g_signal_connect(button_compte, "clicked", G_CALLBACK(show_compte), NULL);

    gtk_grid_attach(GTK_GRID(grid), button_compte, 0,0,1,1);

    button_transaction = gtk_button_new_with_label("Transaction");
    g_signal_connect(button_transaction, "clicked", G_CALLBACK(show_transaction), NULL);

    gtk_grid_attach(GTK_GRID(grid), button_transaction, 1,0,1,1);

    button_exit = gtk_button_new_with_label("Exit");
    g_signal_connect(button_exit, "clicked", G_CALLBACK(close_window), window);

    gtk_grid_attach(GTK_GRID(grid), button_exit, 0,1,2,1);

    gtk_widget_show_all(window);
  }
  else {
    GtkWidget *windowAlert;
    windowAlert = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(windowAlert), "Alerte");

    GtkWidget *textAlert;
    textAlert = gtk_label_new("Can't able to log, login or password incorrect");

    gtk_container_add(GTK_CONTAINER(windowAlert), textAlert);
    gtk_window_set_default_size(GTK_WINDOW(windowAlert), 200,100);
    gtk_window_set_position(GTK_WINDOW(windowAlert), GTK_WIN_POS_CENTER);

    gtk_widget_show_all(windowAlert);
  }
}



/*
  Donne a lapplication la fenetre window qui propose soit de se logger, soit de creer un nouvel utilisateur
*/
void login_page(GtkApplication *app) {
  window = gtk_application_window_new(app);

  //Construction of grid
  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);

  /* Definition et personnalisation de la fenetre */
  gtk_window_set_title(GTK_WINDOW(window), "Gestionnaire bancaire");
  gtk_window_set_default_size(GTK_WINDOW(window), 640,480);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  GtkWidget *login_field;
  GtkWidget *password_field;
  GtkWidget *submit_button, *create_user_button;


  /* CREER LES CHAMPS POUR LE LOGIN ET LE MOT DE PASSE A RENTRER */
  login_field = gtk_entry_new();
  password_field = gtk_entry_new();
  gtk_entry_set_visibility(GTK_ENTRY(password_field), FALSE);

  submit_button = gtk_button_new_with_label("Submit");
  user_entry_login_t* user_data_entry = malloc(sizeof(user_entry_login_t));
  user_data_entry->login = login_field;
  user_data_entry->password = password_field;
  g_signal_connect(GTK_BUTTON(submit_button), "clicked", G_CALLBACK(try_login), user_data_entry);

  create_user_button = gtk_button_new_with_label("Create new user");
  g_signal_connect(GTK_BUTTON(create_user_button), "button-press-event", G_CALLBACK(create_user_form), NULL);

  /* AJOUTE AU LAYOUT LES ELEMENTS */
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Login"), 0,0,1,1);
  gtk_grid_attach(GTK_GRID(grid), login_field, 0,1,1,1);
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Password"), 0,2,1,1);
  gtk_grid_attach(GTK_GRID(grid), password_field, 0,3,1,1);
  gtk_grid_attach(GTK_GRID(grid), submit_button, 0,4,1,1);
  gtk_grid_attach(GTK_GRID(grid), create_user_button, 0,5,1,1);

  /* Affiche tous les elements */
  gtk_widget_show_all(window);
}
