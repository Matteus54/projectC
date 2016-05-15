#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include "gui.h"
#include "bdd.h"

#define UNUSED(p) ((void)(p))

GtkApplication *app;
GtkWidget *window;
GtkWidget *grid;


void clean_window() {
  GList *children, *iter;
  children = gtk_container_get_children(GTK_CONTAINER(grid));
  for(iter = children; iter != NULL; iter = g_list_next(iter)) {
    gtk_widget_destroy(GTK_WIDGET(iter->data));
  }
  g_list_free(children);
}

void close_window(GtkWidget *widget, gpointer window) {
  UNUSED(widget);
  gtk_widget_destroy(GTK_WIDGET(window));
}


void show_compte () {

}

void show_transaction() {

}


void try_login(GtkWidget* widget, gpointer* data) {
  UNUSED(widget);
  user_entry_login_t* user_data_entry = (user_entry_login_t*) data;

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
}

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
      bdd_execute(request);
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


/*
  Fonction qui initialise l'interface graphique de l'application
  Celle ci creer une application, et lui initialise certaines donnees (comme la fermeture lors de lappui sur la croix)
  Elle appelle ensuite la fonction "login_page" en premier
*/
int gui_init(int argc, char **argv) {
  /* Initialisation de GTK+ */
  gtk_init(&argc, &argv);

  int status;

  //creer lapplication
  app = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
  //quitte lapplication si click sur la croix

  //lance la fonction login_page au lancement
  g_signal_connect(app, "activate", G_CALLBACK(login_page), NULL);
  //defini le status de lapplication
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
