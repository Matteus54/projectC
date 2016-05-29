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
  char *compte;
  char *date;
  char *libelle;
  char *negatif;
  double montant;
  double commission;
  char *categorie;
  char *commentaire;
  //char *devise;
} transaction_t;

typedef struct {
  char compte[35];
  char date_debut[11];
  char date_fin[11];
} releve_t;

void transaction_window(GtkWidget *widget, gpointer* data);
void show_transaction();

#endif
