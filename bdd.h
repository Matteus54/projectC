#ifndef BDD_H
#define BDD_H

#include "compte.h"
#include "transactions.h"

int hash (const char* word);
int callback(void *NotUsed, int argc, char **argv, char **azColName);
transaction_t** bdd_get_list_transaction();
livret_t** bdd_get_list_livret();
account_t** bdd_get_list_account();
char** bdd_get_type_livret();
char** bdd_get_categorie();
int bdd_login(char* request);
int bdd_execute(char *sql);
void bdd_init();
char* bdd_get_iban_from_libelle(char* libelle);

#endif
