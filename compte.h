#include <gtk/gtk.h>


typedef struct {
  GtkWidget *iban;
  GtkWidget *solde;
  GtkWidget *libelle;
  GtkWidget *livret;
  GtkWidget *plafond;
  GtkWidget *interet;
  GtkWidget *type_livret;
} account_entry_creation_t;


void create_account();
void create_account_form();
void show_compte (GtkWidget *widget, gpointer* data);
