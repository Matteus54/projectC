#ifndef ALERTS_H
#define ALERTS_H

typedef struct {
  char type[32];
  GtkWidget *type_temps;
  GtkWidget *compte_cat;
  GtkWidget *montant;
} alert_entry_t;

typedef struct {
  int id;
  char type[35];
  char type_temps[35];
  char compte_cat[35];
  double montant;
} alert_t;

void check_alerts();
void create_alert_form(GtkWidget *widget, char* type);
void alert_window();

#endif
