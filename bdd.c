#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include "compte.h"
#include "transactions.h"
#include "bdd_checks.h"
#include "releve.h"

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

//Fonction qui permet dexecuter une requete SQL en parametre
int bdd_execute(char *sql) {
  //printf("%s\n", sql);
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

releve_t** bdd_get_list_releve(char *iban) {
  int i = 0;
  sqlite3_stmt *stmt;
  char request[1024] = "SELECT * FROM releve WHERE compte = '";
  strcat(request, iban);
  strcat(request, "';");

  releve_t **listReleve = (releve_t**) calloc(500, sizeof(releve_t*));

  if(sqlite3_prepare_v2(db, request, -1, &stmt, 0) == SQLITE_OK) {
    int res_stmt = sqlite3_step(stmt);
    if(res_stmt == SQLITE_ROW) {
      while(res_stmt == SQLITE_ROW && i < 500) {
          releve_t* releve = malloc(sizeof(releve_t));

          strcpy(releve->compte, (char *) sqlite3_column_text(stmt, 0));
          strcpy(releve->date_debut, (char *) sqlite3_column_text(stmt, 1));
          strcpy(releve->date_fin, (char *) sqlite3_column_text(stmt, 2));
          releve->montant_initial = (double) sqlite3_column_double(stmt, 3);
          releve->montant_final = (double) sqlite3_column_double(stmt, 4);

          listReleve[i] = releve;
          i++;

          res_stmt = sqlite3_step(stmt);
      }
      sqlite3_finalize(stmt);
      return listReleve;
    }
    else {
      printf("Unable to get list of releve, or no one exists\n");
      sqlite3_finalize(stmt);
      return NULL;
    }
  }
  else {
    printf("SQL ERROR GET TRANSACTIONS\n");
    return NULL;
  }
}

transaction_t** bdd_get_list_transaction (char *iban) {
  int i = 0;
  sqlite3_stmt *stmt;
  char request[1024] = "SELECT * FROM transactionCompte WHERE compte_iban = '";
  strcat(request, iban);
  strcat(request, "';");

  transaction_t **listTransaction = (transaction_t**) calloc(500, sizeof(transaction_t*));

  if(sqlite3_prepare_v2(db, request, -1, &stmt, 0) == SQLITE_OK) {
    int res_stmt = sqlite3_step(stmt);
    if(res_stmt == SQLITE_ROW) {
      while(res_stmt == SQLITE_ROW && i<500) {
        transaction_t* transaction = malloc(sizeof(transaction_t));

        transaction->id = sqlite3_column_int(stmt, 0);
        strcpy(transaction->libelle, (char*) sqlite3_column_text(stmt, 3));
        transaction->montant = (double) sqlite3_column_double(stmt, 4);
        transaction->commission = (double) sqlite3_column_double(stmt, 6);
        strcpy(transaction->date, (char*) sqlite3_column_text(stmt, 2));
        strcpy(transaction->commentaire, (char*) sqlite3_column_text(stmt, 8));
        strcpy(transaction->categorie, (char*) sqlite3_column_text(stmt, 7));
        strcpy(transaction->compte, (char*) sqlite3_column_text(stmt, 1));

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
  free(listTransaction);
  free(request);
}

livret_t** bdd_get_list_livret() {
  int i = 0;
  sqlite3_stmt *stmt;
  char request[1024] = "SELECT * FROM compte c NATURAL JOIN livret t WHERE proprietaire = '";
  strcat(request, login);
  strcat(request, "';");

  livret_t **listSavingsAccount = (livret_t**) calloc(100, sizeof(livret_t*));

  if(sqlite3_prepare_v2(db, request, -1, &stmt, 0) == SQLITE_OK) {
    int res_stmt = sqlite3_step(stmt);
    if(res_stmt == SQLITE_ROW) {
      while(res_stmt == SQLITE_ROW && i<100) {
        livret_t* account = (livret_t*) malloc(sizeof(livret_t));

        strcpy(account->iban, (char*) sqlite3_column_text(stmt, 0));
        account->solde = (double) sqlite3_column_double(stmt, 1);
        strcpy(account->libelle, (char*) sqlite3_column_text(stmt, 2));
        account->plafond = (double) sqlite3_column_double(stmt, 5);
        account->interet = (double) sqlite3_column_double(stmt, 6);
        strcpy(account->type_livret, (char*) sqlite3_column_text(stmt, 7));

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
    free(listSavingsAccount);
    free(request);
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

  account_t **listAccount = (account_t**) calloc(100, sizeof(account_t*));
  if(sqlite3_prepare_v2(db, request, -1, &stmt, 0) == SQLITE_OK) {
    int res_stmt = sqlite3_step(stmt);
    if(res_stmt == SQLITE_ROW) {
      while(res_stmt == SQLITE_ROW && i<100) {
        account_t* account = malloc(sizeof(account_t));

        strcpy(account->iban, (char*) sqlite3_column_text(stmt, 0));
        account->solde = (double) sqlite3_column_double(stmt, 1);
        strcpy(account->libelle, (char*) sqlite3_column_text(stmt, 2));

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

char** bdd_get_field_from_table(char *field, char *table) {
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
  free(listText);
  free(request);
  free(text);
}

char** bdd_get_categorie() {
  return bdd_get_field_from_table("type_trans", "typeTransaction");
}

char** bdd_get_type_livret() {
  return bdd_get_field_from_table("type_livret", "livret_type");
}

char** bdd_get_libelle_account() {
  return bdd_get_field_from_table("libelle", "compte");
}

int bdd_line_where_is_in_table(char *field, char *value, char *table) {
  sqlite3_stmt *stmt;

  char request[150];
  sprintf(request, "SELECT * FROM %s WHERE %s = '%s'", table, field, value);

  if (sqlite3_prepare_v2(db, request, -1, &stmt, 0) == SQLITE_OK) {
    int res_stmt = sqlite3_step(stmt);
    if(res_stmt == SQLITE_ROW) {
      sqlite3_finalize(stmt);
      return 1;
    }
    else {
      sqlite3_finalize(stmt);
      return 0;
    }
  }
  else {
      printf("SQL ERROR LOGIN\n");
      sqlite3_finalize(stmt);
      return 0;
  }
}

int bdd_iban_exists(char *iban) {
  return bdd_line_where_is_in_table("iban", iban, "compte");
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

char* bdd_get_iban_from_libelle(char* libelle) {
  char request[1024] = "SELECT iban FROM compte WHERE libelle = '";
  strcat(request, libelle);
  strcat(request, "';");
  char *iban = malloc(sizeof(char)*34);
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, request, -1, &stmt, 0) == SQLITE_OK) {
    int res_stmt = sqlite3_step(stmt);
    if(res_stmt == SQLITE_ROW) {
      char* text = (char*)sqlite3_column_text(stmt,0);
      memcpy(iban, text, strlen(text));
    }
    sqlite3_finalize(stmt);
    return iban;
  } else {
    printf("SQL ERROR GET IBAN FROM LIBELLE\n");
    sqlite3_finalize(stmt);
    return NULL;
  }
  free(iban);
  free(request);
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

  request = "CREATE TABLE IF NOT EXISTS releve ("\
            "compte VARCHAR2(34) REFERENCES compte(iban),"\
            "date_debut DATE,"\
            "date_fin DATE,"\
            "montant_initial NUMBER(12,2),"\
            "montant_final NUMBER(12,2),"
            "CONSTRAINT releve_pk PRIMARY KEY (compte, date_debut, date_fin));";
  bdd_execute(request);
}
