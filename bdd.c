#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include "compte.h"
#include "transactions.h"
#include "bdd_checks.h"

#define UNUSED(p) ((void)(p))

static sqlite3 *db;


static char *zErrMsg = 0;
static int rc;
static char *request;
char login[30];

//Constante qui sert a definir la taille max du hashing
const int MUST_BE_LESS_THAN = 100000000; // 8 decimal digits max


//Fonction qui hash un String et renvoi son entier sur MUST_BE_LESS_THAN digits max
int hash (const char* word) {
    unsigned int hash = 0;
    int i;
    for (i = 0 ; word[i] != '\0' ; i++)
    {
        hash = 31*hash + word[i];
    }


    return hash % MUST_BE_LESS_THAN;
}


//Fonction qui sert a pas grand chose (utilise sur lapi de la connexion a la BDD)
int callback(void *NotUsed, int argc, char **argv, char **azColName){
  UNUSED(NotUsed);
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

transaction_t** bdd_get_list_transaction () {
  //char *iban en parametr
  /*
  int i = 0;
  sqlite3_stmt *stmt;
  char request[1024] = "SELECT * from transactionCompte WHERE compte_iban = '";
  strcat(request, iban);
  strcat(request, "';");

  transaction_t **listTransaction = (transaction_t**) calloc(500, sizeof(transaction_t*));
  if(sqlite3_prepare_v2(db, request, -1, &stmt, 0) == SQLITE_OK) {
    int res_stmt = sqlite3_step(stmt);
    if(res_stmt == SQLITE_ROW) {
      transaction_t* transaction = malloc(sizeof(transaction_t));

      transaction->libelle = calloc(1,sizeof(transaction->libelle));
      transaction->negatif = calloc(1,sizeof(transaction->negatif));
      transaction->date = calloc(1,sizeof(transaction->libelle));
      transaction->commentaire = calloc(1,sizeof(transaction->libelle));
      transaction->type = calloc(1,sizeof(transaction->libelle));
      transaction->compte_iban = calloc(1,sizeof(transaction->libelle));

      while(res_stmt == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        char *libelle = (char*) sqlite3_column_text(stmt, 1);
        double montant = (double) sqlite3_column_double(stmt, 2);
        char *negatif = (char*) sqlite3_column_text(stmt, 3);
        double commission = sqlite3_column_double(stmt, 4);
        char *date = (char*) sqlite3_column_text(stmt, 5);
        char *commentaire = (char*) sqlite3_column_text(stmt, 6);
        char *type = (char*) sqlite3_column_text(stmt, 7);
        char *compte_iban = (char*) sqlite3_column_text(stmt, 8);

        transaction->id = id;
        memcpy(transaction->libelle, libelle, strlen(libelle));
        transaction->montant = montant;
        memcpy(transaction->negatif, negatif, strlen(negatif));
        transaction->commission = commission;
        memcpy(transaction->date, date, strlen(date));
        memcpy(transaction->commentaire, commentaire, strlen(commentaire));
        memcpy(transaction->type, type, strlen(type));
        memcpy(transaction->compte_iban, compte_iban, strlen(compte_iban));

        listTransaction[i] = transaction;
        i++;

        res_stmt = sqlite3_step(stmt);
      }
      sqlite3_finalize(stmt);
      return listTransaction;
    }
    else {
      printf("Unable to get list of transactions\n");
      sqlite3_finalize(stmt);
      return NULL;
    }
  }
  else {
    printf("SQL ERROR GET TRANSACTIONS\n");
    return NULL;
  }
  */
  return NULL;
}

livret_t** bdd_get_list_livret() {
  int i = 0;
  sqlite3_stmt *stmt;
  char request[1024] = "SELECT * FROM compte c NATURAL JOIN livret t WHERE proprietaire = '";
  strcat(request, login);
  strcat(request, "';");

  livret_t **listSavingsAccount = (livret_t**) calloc(500, sizeof(livret_t*));
  if(sqlite3_prepare_v2(db, request, -1, &stmt, 0) == SQLITE_OK) {
    int res_stmt = sqlite3_step(stmt);
    if(res_stmt == SQLITE_ROW) {
      livret_t* account = malloc(sizeof(livret_t));
      account->iban = calloc(1,sizeof(account->iban));
      account->libelle = calloc(1,sizeof(account->libelle));
      account->type_livret = calloc(1,sizeof(account->type_livret));

      while(res_stmt == SQLITE_ROW) {
        char *iban = (char *) sqlite3_column_text(stmt, 0);
        double solde = (double) sqlite3_column_double(stmt, 1);
        char *libelle = (char*) sqlite3_column_text(stmt, 2);
        double plafond = (double) sqlite3_column_double(stmt, 5);
        double interet = (double) sqlite3_column_double(stmt, 6);
        char *type_livret = (char*) sqlite3_column_text(stmt, 7);

        memcpy(account->iban, iban, strlen(iban));
        account->solde = solde;
        memcpy(account->libelle, libelle, strlen(libelle));
        account->plafond = plafond;
        account->interet = interet;
        memcpy(account->type_livret, type_livret, strlen(type_livret));

        listSavingsAccount[i] = account;
        i++;

        res_stmt = sqlite3_step(stmt);
      }
      sqlite3_finalize(stmt);
      return listSavingsAccount;
    }
    else {
      printf("Unable to get list of savings accounts\n");
      sqlite3_finalize(stmt);
      return NULL;
    }
  }
  else {
    printf("SQL ERROR GET SAVINGS ACCOUNTS\n");
    return NULL;
  }
}

account_t** bdd_get_list_account() {
  int i = 0;
  sqlite3_stmt *stmt;
  char request[1024] = "SELECT * FROM compte WHERE proprietaire = '";
  strcat(request, login);
  strcat(request, "' AND booleanLivret = 'FALSE';");

  account_t **listAccount = (account_t**) calloc(500, sizeof(account_t*));
  if(sqlite3_prepare_v2(db, request, -1, &stmt, 0) == SQLITE_OK) {
    int res_stmt = sqlite3_step(stmt);
    if(res_stmt == SQLITE_ROW) {
      account_t* account = malloc(sizeof(account_t));
      account->iban = calloc(1,sizeof(account->iban));
      account->libelle = calloc(1,sizeof(account->libelle));

      while(res_stmt == SQLITE_ROW) {
        char *iban = (char *) sqlite3_column_text(stmt, 0);
        double solde = (double) sqlite3_column_double(stmt, 1);
        char *libelle = (char*) sqlite3_column_text(stmt, 2);

        memcpy(account->iban, iban, strlen(iban));
        account->solde = solde;
        memcpy(account->libelle, libelle, strlen(libelle));

        listAccount[i] = account;

        i++;
        res_stmt = sqlite3_step(stmt);
      }
      sqlite3_finalize(stmt);
      return listAccount;
    }
    else {
      printf("Unable to get list of accounts\n");
      sqlite3_finalize(stmt);
      return NULL;
    }
  }
  else {
    printf("SQL ERROR GET ACCOUNTS\n");
    return NULL;
  }
}

char** bdd_get_field_from_table(char *table, char *field) {
  char **listText = (char **) calloc (30,sizeof(char*));
  char *text;
  int i = 0;
  sqlite3_stmt *stmt;
  char request[150];
  sprintf(request, "SELECT %s FROM %s;", field, table);

  if (sqlite3_prepare_v2(db, request, -1, &stmt, 0) == SQLITE_OK) {
    int res_stmt = sqlite3_step(stmt);
    if(res_stmt == SQLITE_ROW) {
      while(res_stmt == SQLITE_ROW) {
        text = (char*)sqlite3_column_text(stmt,0);
        listText[i] = (char *)calloc(strlen(text)+2,sizeof(char));
        memcpy(listText[i], text, strlen(text));
        i++;
        res_stmt = sqlite3_step(stmt);
      }
      return listText;
    }
    else {
      printf("Unable to get list of field %s\n", field);
      sqlite3_finalize(stmt);
      return NULL;
    }
  }
  else {
      printf("SQL ERROR LOGIN\n");
      return NULL;
  }
}

char** bdd_get_categorie() {
  return bdd_get_field_from_table("typeTransaction", "type_trans");
}

char** bdd_get_type_livret() {
  return bdd_get_field_from_table("livret_type", "type_livret");
}

int bdd_login(char* request) {
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, request, -1, &stmt, 0) == SQLITE_OK) {
    int res_stmt = sqlite3_step(stmt);
    if(res_stmt == SQLITE_ROW) {
      char* text;
      text = (char*)sqlite3_column_text(stmt,0);
      memcpy(login, text, strlen(text));
      sqlite3_finalize(stmt);
      return 1;
    }
    else{
      printf("Unable to login, login or password incorrect\n");
      sqlite3_finalize(stmt);
      return 0;
    }
  }
  else {
    printf("SQL ERROR LOGIN\n");
    return 0;
  }
}

//Fonction qui permet dexecuter une requete SQL en parametre
int bdd_execute(char *sql) {
  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if( rc != SQLITE_OK) {
    fprintf(stderr, "SQL ERROR: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return 0;
  }
  else {
    fprintf(stdout, "SQL request executed successfully\n");
    return 1;
  }
}

char* bdd_get_iban_from_libelle(char* libelle) {
  char request[1024] = "SELECT iban FROM compte WHERE libelle = '";
  strcat(request, libelle);
  strcat(request, "';");
  char *iban = malloc(sizeof(char)*34);
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, request, -1, &stmt, 0) == SQLITE_OK) {
    int res_stmt = sqlite3_step(stmt);
    if(res_stmt == SQLITE_ROW) {
      iban = (char*)sqlite3_column_text(stmt,0);
    }
    sqlite3_finalize(stmt);
    return iban;
  } else {
    printf("SQL ERROR GET IBAN FROM LIBELLE\n");
    sqlite3_finalize(stmt);
    return NULL;
  }
}

/* Initialise la BDD lors du lancement de lapplication
  1- Creer le fichier banqueDB.db si il nexiste pas ou accede a la BDD si il existe
  2- Creer les tables si elles nexistent pas
    (utilisateur)
  3- Execute les requetes de creation des tables
*/
void bdd_init() {
  printf("Initialisation de la BDD \n");
  rc = sqlite3_open("banqueDB.db", &db);

  if(rc){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    exit(0);
  }
  else {
    fprintf(stdout, "Opened database successfully\n");
  }

  request = "CREATE TABLE IF NOT EXISTS utilisateur ("\
            "login VARCHAR2(30) PRIMARY KEY,"\
            "password INT(8) NOT NULL);";
  bdd_execute(request);

  request = "CREATE TABLE IF NOT EXISTS compte ("\
            "iban VARCHAR2(34) PRIMARY KEY CHECK (length(iban) >= 14 AND length(iban) <= 34),"\
            "solde NUMBER(12,2) NOT NULL,"\
            "libelle VARCHAR2(255) UNIQUE,"\
            "booleanLivret BOOLEAN NOT NULL,"\
            "proprietaire VARCHAR2(30) NOT NULL,"\
            "CONSTRAINT compte_fk FOREIGN KEY (proprietaire) REFERENCES utilisateur(login));";
  bdd_execute(request);

  request = "CREATE TABLE IF NOT EXISTS livret_type ("\
            "type_livret VARCHAR2(255) PRIMARY KEY,"\
            "libelle VARCHAR2(255));";
  bdd_execute(request);

  request = "INSERT OR IGNORE INTO livret_type VALUES ('A', 'Livret A');";
  bdd_execute(request);
  request = "INSERT OR IGNORE INTO livret_type VALUES ('Jeune', 'Livret Jeune');";
  bdd_execute(request);

  request = "CREATE TABLE IF NOT EXISTS livret (iban VARCHAR2(34) PRIMARY KEY, "\
          "plafond NUMBER(12,2) NOT NULL, interet NUMBER(3,2) NOT NULL, type VARCHAR2(255) NOT NULL,"\
          "CONSTRAINT livret_id_fk FOREIGN KEY (iban) REFERENCES compte(iban)"\
          "CONSTRAINT livret_type_fk FOREIGN KEY (type) REFERENCES livret_type(type_livret));";
  bdd_execute(request);

  request = "CREATE TABLE IF NOT EXISTS typeTransaction ("\
            "type_trans VARCHAR2(255) PRIMARY KEY);";
  bdd_execute(request);

  request = "INSERT OR IGNORE INTO typeTransaction VALUES ('Work');";
  bdd_execute(request);
  request = "INSERT OR IGNORE INTO typeTransaction VALUES ('Food');";
  bdd_execute(request);
  request = "INSERT OR IGNORE INTO typeTransaction VALUES ('Clothes');";
  bdd_execute(request);
  request = "INSERT OR IGNORE INTO typeTransaction VALUES ('Vacation');";
  bdd_execute(request);
  request = "INSERT OR IGNORE INTO typeTransaction VALUES ('Hobies');";
  bdd_execute(request);
  request = "INSERT OR IGNORE INTO typeTransaction VALUES ('Transfert');";
  bdd_execute(request);


  request = "CREATE TABLE IF NOT EXISTS transactionCompte ("\
          "id_transaction INTEGER PRIMARY KEY AUTOINCREMENT,"\
          "compte_iban VARCHAR2(34) NOT NULL,"\
          "date DATE NOT NULL,"\
          "libelle VARCHAR2(255) NOT NULL,"\
          "montant NUMBER(12,2) NOT NULL,"\
          "negatif BOOLEAN NOT NULL,"\
          "commission NUMBER (6,2) NOT NULL DEFAULT '0',"\
          "type VARCHAR2(255) NOT NULL,"\
          "commentaire VARCHAR2(255),"\
          "CONSTRAINT transaction_compte_fk FOREIGN KEY (compte_iban) REFERENCES compte(iban),"\
          "CONSTRAINT transaction_type_fk FOREIGN KEY (type) REFERENCES typeTransaction(type_trans));";
  bdd_execute(request);


}
