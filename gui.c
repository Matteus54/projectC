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


GtkApplication *app;
GtkWidget *window;
GtkWidget *grid;


void show_hide_button_box(GtkWidget *button, GtkWidget *box) {
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button))){
    gtk_widget_show(box);
  }
  else {
    gtk_widget_hide(box);
  }
}

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
