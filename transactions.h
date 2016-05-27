#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <gtk/gtk.h>

typedef struct {
  char *compte;
  char *date;
  char *libelle;
  char *montant;
  char *comission;
} transaction_t;

typedef struct {
  char *compte;
  char *date_debut;
  char *date_fin;
} releve_t;

void transaction_window(GtkWidget *widget, gpointer* data);
void show_transaction();

#endif
