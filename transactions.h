#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <gtk/gtk.h>

typedef struct {
  GtkWidget *compte;
  GtkWidget *date;
  GtkWidget *libelle;
  GtkWidget *montant;
  GtkWidget *commission;
  GtkWidget *categorie;
  GtkWidget *commentaire;
  //GtkWidget *devise;
} transaction_entry_creation_t;

typedef struct {
  char *compte;
  char *date;
  char *libelle;
  char *montant;
  char *commission;
  char *categorie;
  char *commentaire;
  //char *devise;
} transaction_t;

typedef struct {
  char *compte;
  char *date_debut;
  char *date_fin;
} releve_t;

void transaction_window(GtkWidget *widget, gpointer* data);
void show_transaction();

#endif
