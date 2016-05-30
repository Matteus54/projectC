#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include "gui.h"
#include "create_user.h"
#include "login.h"
#include "compte.h"
#include "bdd.h"
#include "transactions.h"
#include "statistique.h"
#include "alerts.h"


GtkApplication *app;
GtkWidget *window;
GtkWidget *grid;

void widget_set_margins(GtkWidget *widget, int top, int bottom, int left, int right) {
  gtk_widget_set_margin_top(widget, top);
  gtk_widget_set_margin_bottom(widget, bottom);
  gtk_widget_set_margin_left(widget, left);
  gtk_widget_set_margin_right(widget, right);
}

void widget_set_margin(GtkWidget *widget, int margin) {
  widget_set_margins(widget, margin, margin, margin, margin);
}

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

char* file_browser() {
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;
  char *filename = NULL;

  dialog = gtk_file_chooser_dialog_new ("Open File",
                                        GTK_WINDOW(window),
                                        action,
                                        ("Cancel"),
                                        GTK_RESPONSE_CANCEL,
                                        ("Open"),
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);

  res = gtk_dialog_run (GTK_DIALOG (dialog));
  if (res == GTK_RESPONSE_ACCEPT)
    {
      GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
      filename = gtk_file_chooser_get_filename (chooser);

      printf("path to selected file : %s\n", filename);
    }


  gtk_widget_destroy (dialog);
  return(filename);
}

void open_picture(char *filename) {
  GtkWidget *windowPicture;
  GtkWidget *grid;
  GtkWidget *button_exit;
  GtkWidget *picture;

  windowPicture = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(windowPicture), "Graphique");
  gtk_window_set_position(GTK_WINDOW(windowPicture), GTK_WIN_POS_CENTER);

  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(windowPicture), grid);

  picture = gtk_image_new_from_file(filename);

  button_exit = gtk_button_new_with_label("Exit");
  g_signal_connect(GTK_BUTTON(button_exit), "clicked", G_CALLBACK(close_window), windowPicture);

  gtk_grid_attach(GTK_GRID(grid), picture, 0, 0, 5, 1);
  gtk_grid_attach(GTK_GRID(grid), button_exit, 4, 1, 1, 1);

  gtk_widget_show_all(windowPicture);

}

void alert_dialog(GtkWidget *activeWindow, gchar *text) {
   GtkWidget *dialog, *textAlert, *content_area;

   /* Create the widgets */
   dialog = gtk_dialog_new_with_buttons ("Alert",
                                         GTK_WINDOW(activeWindow),
                                         GTK_DIALOG_DESTROY_WITH_PARENT,
                                         ("OK"),
                                         GTK_RESPONSE_NONE,
                                         NULL);

   content_area = gtk_dialog_get_content_area(GTK_DIALOG (dialog));

   textAlert = gtk_label_new (text);

   /* Ensure that the dialog box is destroyed when the user responds */
   g_signal_connect_swapped (dialog, "response", G_CALLBACK (gtk_widget_destroy), dialog);

   widget_set_margin(textAlert, 10);

   gtk_container_add (GTK_CONTAINER (content_area), textAlert);
   gtk_widget_show_all(dialog);
   gtk_dialog_run(GTK_DIALOG(dialog));
}

void alert_dialog_then_close(GtkWidget *activeWindow, gchar *text) {
  alert_dialog(activeWindow, text);
  close_window(NULL, activeWindow);
}

void main_window() {
  //printf("main window\n");
  clean_window();

  GtkWidget *button_compte, *button_transaction, *button_statistique, *button_alert, *button_exit;

  //setting window
  gtk_window_set_title(GTK_WINDOW(window), "Welcome");

  button_compte = gtk_button_new_with_label("Compte");
  g_signal_connect(button_compte, "clicked", G_CALLBACK(show_compte), NULL);

  gtk_grid_attach(GTK_GRID(grid), button_compte, 0,0,1,1);

  button_transaction = gtk_button_new_with_label("Transaction");
  g_signal_connect(button_transaction, "clicked", G_CALLBACK(transaction_window), NULL);

  gtk_grid_attach(GTK_GRID(grid), button_transaction, 1,0,1,1);


  button_statistique = gtk_button_new_with_label("Statistique");
  g_signal_connect(button_statistique, "clicked", G_CALLBACK(statistique_window), NULL);

  gtk_grid_attach(GTK_GRID(grid), button_statistique, 2, 0, 1, 1);

  button_alert = gtk_button_new_with_label("Alert");
  g_signal_connect(button_alert, "clicked", G_CALLBACK(alert_window), NULL);

  gtk_grid_attach(GTK_GRID(grid), button_alert, 3, 0, 1, 1);


  button_exit = gtk_button_new_with_label("Exit");
  g_signal_connect(button_exit, "clicked", G_CALLBACK(close_window), window);

  gtk_grid_attach(GTK_GRID(grid), button_exit, 0,1,2,1);

  gtk_widget_show_all(window);
}

void main_handler(GtkApplication *app) {
  //printf("main handler\n");
  /* Definition et personnalisation de la fenetre */
  window = gtk_application_window_new(app);
  gtk_window_set_default_size(GTK_WINDOW(window), 640,480);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  //Construction of grid
  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);

  login_window();
}

/*
  Fonction qui initialise l'interface graphique de l'application
  Celle ci creer une application, et lui initialise certaines donnees (comme la fermeture lors de lappui sur la croix)
  Elle appelle ensuite la fonction "login_page" en premier
*/
int gui_init(int argc, char **argv) {
  //printf("init\n");
  /* Initialisation de GTK+ */
  gtk_init(&argc, &argv);

  int status = 0;

  //creer lapplication
  app = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
  //quitte lapplication si click sur la croix

  //lance la fonction login_page au lancement
  g_signal_connect(app, "activate", G_CALLBACK(main_handler), NULL);
  //defini le status de lapplication

  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
