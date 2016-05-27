#ifndef BDD_H
#define BDD_H

#include "compte.h"

int hash (const char* word);
int callback(void *NotUsed, int argc, char **argv, char **azColName);
livret_t** bdd_get_list_livret();
account_t** bdd_get_list_account();
char** bdd_get_type_livret();
int bdd_login(char* request);
int bdd_execute(char *sql);
void bdd_init();

#endif
