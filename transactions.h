#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <gtk/gtk.h>

typedef struct {
  GtkWidget *id;
  GtkWidget *compte;
  GtkWidget *date;
  GtkWidget *libelle;
  GtkWidget *montant;
  GtkWidget *commission;
  GtkWidget *categorie;
  GtkWidget *commentaire;
  GtkWidget *window;
  //GtkWidget *devise;
} transaction_entry_creation_t;

typedef struct {
  int id;
  char compte[35];
  char date[11];
  char libelle[256];
  char negatif[6];
  double montant;
  double commission;
  char categorie[256];
  char commentaire[256];
  //char *devise;
} transaction_t;


void transaction_window(GtkWidget *widget, gpointer* data);
void show_transaction();

#endif
