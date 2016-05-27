#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <gtk/gtk.h>

typedef struct {
  int   id;
  char *libelle;
  double montant;
  char *negatif;
  double commission;
  char *date;
  char *commentaire;
  char *type;
  char *compte_iban;
} transaction_t;


void transaction_window(GtkWidget *widget, gpointer* data);
void show_transaction();

#endif
