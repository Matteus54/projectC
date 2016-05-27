#include <string.h>
#include "bdd.h"
#include "bdd_inserts.h"
#include "transactions.h"


void bdd_insert_transaction (transaction_t *transaction) {


  char request[1024] = "INSERT INTO transactionCompte "\
  "(compte_iban, date, libelle, montant, negatif, commission, type, commentaire) "\
  "VALUES('";
  strcat(request, transaction->compte);
  //strcat(request, "', CONVERT(varchar, '");
  strcat(request, "', '");
  strcat(request, transaction->date);
  //strcat(request, "', 102), '");
  strcat(request, "', '");
  strcat(request, transaction->libelle);
  strcat(request, "', ");
  strcat(request, transaction->montant);
  strcat(request, ", ");
  strcat(request, "'FALSE'"); //n'est peut être pas necessaire
  strcat(request, ", ");
  strcat(request, transaction->commission);
  strcat(request, ", '");
  strcat(request, transaction->categorie);
  strcat(request, "', '");
  strcat(request, transaction->commentaire);
  strcat(request, "');");

  printf("%s\n", request);
  bdd_execute(request);
}
