#ifndef COMPTE_H
#define COMPTE_H

#include <gtk/gtk.h>

typedef struct {
  GtkWidget *iban;
  GtkWidget *solde;
  GtkWidget *libelle;
  GtkWidget *livret;
  GtkWidget *plafond;
  GtkWidget *interet;
  GtkWidget *type_livret;
  GtkWidget *window;
} account_entry_creation_t;

typedef struct {
  char *iban;
  double solde;
  char *libelle;
} account_t;

typedef struct {
  char *iban;
  double solde;
  char *libelle;
  double plafond;
  double interet;
  char *type_livret;
} livret_t;

int isNumeric(const char *string, int isDecimal);
void create_account(GtkWidget* widget, gpointer* data);
void create_account_form();
void show_compte (GtkWidget *widget, gpointer* data);

#endif
