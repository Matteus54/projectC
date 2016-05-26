#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "gui.h"
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

  gtk_window_set_title(GTK_WINDOW(window), "Gestionnaire bancaire");
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
    // passe a la page principal si le login est valide
    main_window();
  }
  else {
    alert_window("Unable to login, login or password incorrect");
  }
}

/*
  Donne a lapplication la fenetre window qui propose soit de se logger, soit de creer un nouvel utilisateur
*/
void login_window() {
  GtkWidget *login_field;
  GtkWidget *password_field;
  GtkWidget *submit_button, *create_user_button, *exit_button;


  /* CREER LES CHAMPS POUR LE LOGIN ET LE MOT DE PASSE A RENTRER */
  login_field = gtk_entry_new();
  password_field = gtk_entry_new();
  gtk_entry_set_visibility(GTK_ENTRY(password_field), FALSE);

  user_entry_login_t* user_data_entry = malloc(sizeof(user_entry_login_t));
  user_data_entry->login = login_field;
  user_data_entry->password = password_field;

  submit_button = gtk_button_new_with_label("Submit");
  g_signal_connect(GTK_BUTTON(submit_button), "clicked", G_CALLBACK(try_login), user_data_entry);

  create_user_button = gtk_button_new_with_label("Create new user");
  g_signal_connect(GTK_BUTTON(create_user_button), "button-press-event", G_CALLBACK(create_user_form), NULL);

  exit_button = gtk_button_new_with_label("Exit");
  g_signal_connect(exit_button, "clicked", G_CALLBACK(close_window), window);

  /* AJOUTE AU LAYOUT LES ELEMENTS */
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Login"), 0,0,1,1);
  gtk_grid_attach(GTK_GRID(grid), login_field, 0,1,1,1);
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Password"), 0,2,1,1);
  gtk_grid_attach(GTK_GRID(grid), password_field, 0,3,1,1);
  gtk_grid_attach(GTK_GRID(grid), submit_button, 0,4,1,1);
  gtk_grid_attach(GTK_GRID(grid), create_user_button, 0,5,1,1);
  gtk_grid_attach(GTK_GRID(grid), exit_button, 0,6,1,1);

  /* Affiche tous les elements */
  gtk_widget_show_all(window);
}
