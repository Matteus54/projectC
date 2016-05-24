#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include "gui.h"
#include "user_entries_structs.h"
#include "create_user.h"
#include "bdd.h"

/*
  Fonction qui permet de creer un utilisateur dans la BDD
*/
void create_user(GtkWidget* widget, gpointer* data) {
  UNUSED(widget);
  user_entry_creation_t* user_data = (user_entry_creation_t*)data;
  GtkWidget *login = GTK_WIDGET(user_data->login);
  GtkWidget *password = GTK_WIDGET(user_data->password);
  GtkWidget *password_check = GTK_WIDGET(user_data->password_check);
  const char *login_text = gtk_entry_get_text(GTK_ENTRY(login));
  const char *password_text = gtk_entry_get_text(GTK_ENTRY(password));
  const char *password_check_text = gtk_entry_get_text(GTK_ENTRY(password_check));


  if(strlen(login_text) >= 6 && strlen(login_text) <= 30 && strlen(password_text) >= 6 && strlen(password_text) <= 30) {
    if(strcmp(password_text, password_check_text) == 0) {

      //on recupere le hash du password et on le met dans un buffer pour le passer en String pour la concatenation
      int password_hash = hash(password_text);
      char buffer[8];
      sprintf(buffer, "%d", password_hash);

      //on cree la requete
      char request[1024] = "INSERT INTO utilisateur VALUES ('";
      strcat(request, login_text);
      strcat(request, "', '");
      strcat(request, buffer);
      strcat(request, "');");
      if(bdd_execute(request)) {
        GtkWidget *windowAlert;
        windowAlert = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(windowAlert), "Alerte");

        GtkWidget *textAlert;
        textAlert = gtk_label_new("Utilisateur bien enregistré !");

        gtk_container_add(GTK_CONTAINER(windowAlert), textAlert);
        gtk_window_set_default_size(GTK_WINDOW(windowAlert), 200,100);
        gtk_window_set_position(GTK_WINDOW(windowAlert), GTK_WIN_POS_CENTER);

        gtk_widget_show_all(windowAlert);
      }
      else {
        GtkWidget *windowAlert;
        windowAlert = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(windowAlert), "Alerte");

        GtkWidget *textAlert;
        textAlert = gtk_label_new("Erreur SQL, impossible de s'enregistrer (Login surement déjà utilisé)");

        gtk_container_add(GTK_CONTAINER(windowAlert), textAlert);
        gtk_window_set_default_size(GTK_WINDOW(windowAlert), 200,100);
        gtk_window_set_position(GTK_WINDOW(windowAlert), GTK_WIN_POS_CENTER);

        gtk_widget_show_all(windowAlert);
      }
    }
    else {
      GtkWidget *windowAlert;
      windowAlert = gtk_window_new(GTK_WINDOW_TOPLEVEL);
      gtk_window_set_title(GTK_WINDOW(windowAlert), "Alerte");

      GtkWidget *textAlert;
      textAlert = gtk_label_new("Mot de passe differents ! Veuillez recommencer");

      gtk_container_add(GTK_CONTAINER(windowAlert), textAlert);
      gtk_window_set_default_size(GTK_WINDOW(windowAlert), 200,100);
      gtk_window_set_position(GTK_WINDOW(windowAlert), GTK_WIN_POS_CENTER);

      gtk_widget_show_all(windowAlert);
    }
  }
  else {
    GtkWidget *windowAlert;
    windowAlert = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(windowAlert), "Alerte");

    GtkWidget *textAlert;
    textAlert = gtk_label_new("Le login et le mot de passe doivent faire entre 6 et 30 caracteres");

    gtk_container_add(GTK_CONTAINER(windowAlert), textAlert);
    gtk_window_set_default_size(GTK_WINDOW(windowAlert), 200,100);
    gtk_window_set_position(GTK_WINDOW(windowAlert), GTK_WIN_POS_CENTER);

    gtk_widget_show_all(windowAlert);
  }

}

/*
  Quand lutilisateur appuie sur "Create new user" a la page login, la fonction create_user_form est appelee
  Elle creer une fenetre externe qui propose un champ login, un mdp et un check.
  Quand lutilisateur submit ses donnees, elle appelle la fonction "create_user" qui valide laction
*/
void create_user_form() {
  GtkWidget *windowUserForm;
  GtkWidget *login_field, *password_field, *password_2_field;
  GtkWidget *create_user_button, *exit_button;
  GtkWidget *vbox;

  windowUserForm = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(windowUserForm), "Formulaire d'inscription");
  gtk_window_set_default_size(GTK_WINDOW(windowUserForm), 400,400);
  gtk_window_set_position(GTK_WINDOW(windowUserForm), GTK_WIN_POS_CENTER);

  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
  gtk_container_add(GTK_CONTAINER(windowUserForm), vbox);

  login_field = gtk_entry_new();
  password_field = gtk_entry_new();
  password_2_field = gtk_entry_new();
  gtk_entry_set_visibility(GTK_ENTRY(password_field), FALSE);
  gtk_entry_set_visibility(GTK_ENTRY(password_2_field), FALSE);

  create_user_button = gtk_button_new_with_label("Create User");

  /*Creation dun passeur de variable */
  user_entry_creation_t* user_data = malloc(sizeof(user_entry_creation_t));
  user_data->login = login_field;
  user_data->password = password_field;
  user_data->password_check = password_2_field;
  g_signal_connect(GTK_BUTTON(create_user_button), "clicked", G_CALLBACK(create_user), user_data);

  exit_button = gtk_button_new_with_label("Exit form");
  g_signal_connect(GTK_BUTTON(exit_button), "clicked", G_CALLBACK(close_window), G_OBJECT(windowUserForm));

  gtk_box_pack_start(GTK_BOX(vbox), gtk_label_new("Login"), 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), login_field, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), gtk_label_new("Password"), 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), password_field, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), gtk_label_new("Password again"), 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), password_2_field, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), create_user_button, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), exit_button, 0, 0, 0);

  gtk_widget_show_all(windowUserForm);
}
