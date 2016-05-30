#ifndef BDD_UPDATES_H
#define BDD_UPDATES_H

#include "transactions.h"
#include "releve.h"

void bdd_insert_transaction(transaction_t *transaction);
void bdd_insert_categorie(char *categorie);
void bdd_insert_releve(releve_t *releve);

void bdd_apply_transaction(transaction_t *transaction);

#endif
