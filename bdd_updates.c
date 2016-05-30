#include <string.h>
#include "bdd.h"
#include "bdd_updates.h"
#include "transactions.h"
#include "releve.h"

void bdd_insert_categorie(char *categorie) {
  char request[300];
  sprintf(request, "INSERT INTO typeTransaction VALUES ('%s');", categorie);

  bdd_execute(request);
}

void bdd_insert_transaction (transaction_t *transaction) {

  char buffer[20];

  char request[1536] = "INSERT INTO transactionCompte "\
  "(compte_iban, date, libelle, montant, negatif, commission, type, commentaire) "\
  "VALUES('";
  strcat(request, transaction->compte);
  strcat(request, "', '");
  strcat(request, transaction->date);
  strcat(request, "', '");
  strcat(request, transaction->libelle);
  strcat(request, "', '");
  sprintf(buffer, "%f", transaction->montant);
  strcat(request, buffer);
  strcat(request, "', ");
  strcat(request, "'FALSE'"); //n'est peut être pas necessaire
  strcat(request, ", '");
  sprintf(buffer, "%f", transaction->commission);
  strcat(request, buffer);
  strcat(request, "', '");
  strcat(request, transaction->categorie);
  strcat(request, "', '");
  strcat(request, transaction->commentaire);
  strcat(request, "');");

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


void bdd_insert_releve(releve_t *releve) {
  char request[255] = "INSERT INTO releve VALUES ('";
  strcat(request, releve->compte);
  strcat(request, "','");
  strcat(request, releve->date_debut);
  strcat(request, "','");
  strcat(request, releve->date_fin);
  strcat(request, "');");
  //bdd_execute(request);
}
