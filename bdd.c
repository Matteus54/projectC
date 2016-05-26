#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include "compte.h"

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

account_t** bdd_get_list_account() {
  int i = 0;
  sqlite3_stmt *stmt;
  char request[1024] = "SELECT * FROM compte WHERE proprietaire = '";
  strcat(request, login);
  strcat(request, "';");

  account_t **listAccount = (account_t**) calloc(100, sizeof(account_t*));
  if(sqlite3_prepare_v2(db, request, -1, &stmt, 0) == SQLITE_OK) {
    int res_stmt = sqlite3_step(stmt);
    if(res_stmt == SQLITE_ROW) {
      while(res_stmt == SQLITE_ROW) {
        char *iban = (char *) sqlite3_column_text(stmt, 0);
        char *solde = (char *) sqlite3_column_text(stmt, 1);
        char *libelle = (char*) sqlite3_column_text(stmt, 2);

        account_t* account = malloc(sizeof(account_t*));
        account->iban = malloc(sizeof(iban));
        memcpy(account->iban, iban, strlen(iban));
        account->solde = malloc(sizeof(solde));
        memcpy(account->solde, solde, strlen(solde));
        account->libelle = malloc(sizeof(libelle));
        memcpy(account->libelle, libelle, strlen(libelle));

        listAccount[i] = account;

        printf("Account: %s | %s | %s \n", listAccount[i]->iban, listAccount[i]->solde, listAccount[i]->libelle);
        i++;
        res_stmt = sqlite3_step(stmt);
      }
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

char** bdd_get_type_livret() {
  char **listText = (char **) calloc (30,sizeof(char*));
  char *text;
  int i = 0;
  sqlite3_stmt *stmt;
  char *request = "SELECT type_livret FROM livret_type;";
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
      printf("Unable to get type of livret\n");
      sqlite3_finalize(stmt);
      return NULL;
    }
  }
  else {
      printf("SQL ERROR LOGIN\n");
      return NULL;
  }
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
    fprintf(stdout, "Records created successfully\n");
    return 1;
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

  request = "CREATE TABLE IF NOT EXISTS utilisateur (login VARCHAR2(30) PRIMARY KEY, password INT(8) NOT NULL);";
  bdd_execute(request);

  request = "CREATE TABLE IF NOT EXISTS compte (iban VARCHAR2(34) PRIMARY KEY CHECK (length(iban) >= 14 AND length(iban) <= 34), solde NUMBER(12,2) NOT NULL,"\
          "libelle VARCHAR2(255), booleanLivret BOOLEAN NOT NULL, proprietaire VARCHAR2(30) NOT NULL, CONSTRAINT compte_fk FOREIGN KEY (proprietaire) REFERENCES utilisateur(login));";
  bdd_execute(request);

  request = "CREATE TABLE IF NOT EXISTS livret_type (type_livret VARCHAR2(255) PRIMARY KEY, libelle VARCHAR2(255));";
  bdd_execute(request);

  request = "INSERT OR IGNORE INTO livret_type VALUES ('A', 'Livret A');";
  bdd_execute(request);
  request = "INSERT OR IGNORE INTO livret_type VALUES ('Jeune', 'Livret A');";
  bdd_execute(request);

  request = "CREATE TABLE IF NOT EXISTS livret (iban VARCHAR2(34) PRIMARY KEY, "\
          "plafond NUMBER(12,2) NOT NULL, interet NUMBER(3,2) NOT NULL, type VARCHAR2(255) NOT NULL,"\
          "CONSTRAINT livret_id_fk FOREIGN KEY (iban) REFERENCES compte(iban)"\
          "CONSTRAINT livret_type_fk FOREIGN KEY (type) REFERENCES livret_type(type_livret));";
  bdd_execute(request);

  request = "CREATE TABLE IF NOT EXISTS typeTransaction (type_trans VARCHAR2(255) PRIMARY KEY, libelle VARCHAR2(255) NOT NULL)";
  bdd_execute(request);

  request = "CREATE TABLE IF NOT EXISTS transactionCompte (id_transaction INTEGER PRIMARY KEY AUTOINCREMENT, libelle VARCHAR2(255) NOT NULL, montant NUMBER(12,2) NOT NULL,"\
          "negatif BOOLEAN NOT NULL, commission NUMBER (6,2) NOT NULL DEFAULT '0', date DATE NOT NULL, commentaire VARCHAR2(255), type VARCHAR2(255) NOT NULL, compte_iban VARCHAR2(34) NOT NULL,"\
          "CONSTRAINT transaction_compte_fk FOREIGN KEY (compte_iban) REFERENCES compte(iban),"\
          "CONSTRAINT transaction_type_fk FOREIGN KEY (type) REFERENCES typeTransaction(type_trans));";
  bdd_execute(request);


}
