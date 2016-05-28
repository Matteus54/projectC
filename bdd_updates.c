#include <string.h>
#include "bdd.h"
#include "bdd_updates.h"
#include "transactions.h"

void bdd_insert_categorie(char *categorie) {
  char request[300];
  sprintf(request, "INSERT INTO typeTransaction VALUES ('%s');", categorie);

  bdd_execute(request);
}


void bdd_apply_transaction(transaction_t *transaction) {
  char buffer[34];
  char request[1024] = "UPDATE compte SET solde = (solde + CAST('";
  sprintf(buffer, "%f", transaction->montant);
  strcat(request, buffer);
  strcat(request, "' AS FLOAT) ) WHERE iban = '");

  strcat(request, transaction->compte);
  strcat(request, "';");

  bdd_execute(request);
}
