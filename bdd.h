#ifndef BDD_H
#define BDD_H

#include "compte.h"
#include "transactions.h"
#include "releve.h"
#include "alerts.h"

int hash (const char* word);
int callback(void *NotUsed, int argc, char **argv, char **azColName);

int bdd_execute(char *sql);

releve_t** bdd_get_list_releve(char *iban);
transaction_t** bdd_get_list_transaction (char *iban, char *date_debut, char *date_fin);
livret_t** bdd_get_list_livret();
account_t** bdd_get_list_account();
alert_t** bdd_get_list_alerts();

char** bdd_get_type_livret();
char** bdd_get_categorie();
char** bdd_get_libelle_account();

double bdd_get_sum(char *request);

int bdd_iban_exists(char *iban);

int bdd_login(char* request);

void bdd_init();
char* bdd_get_iban_from_libelle(char* libelle);
char* bdd_get_libelle_from_iban(char* iban);

#endif
